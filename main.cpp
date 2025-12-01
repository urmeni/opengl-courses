// main.cpp
#include <iostream>
#include<sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Camera.h>
#include <Mesh.h>
#include <ShaderProgram.h>
#include <Texture2D.h>

// --- GLOBAL VARIABLES ---
const char* APP_TITLE = "Ma Scene OpenGL - Etape 3";
int gWindowWidth = 1024;
int gWindowHeight = 768;
GLFWwindow * gWindow = nullptr;
bool gWireframe = false;

// Background Color
glm::vec4 gClearColor(0.1f, 0.1f, 0.2f, 1.0f); // Dark blue

// Camera Configuration
FPSCamera fpsCamera(glm::vec3(0.0f, 2.0f, 10.0f));
const float MOVE_SPEED = 5.0f;
const float MOUSE_SENSITIVITY = 0.1f;

// --- PROTOTYPES ---
bool initOpenGL();
void update(double elapsedTime);
void endOpenGL();
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void showFPS(GLFWwindow* window);

// -- main ---
int main() {
    if (!initOpenGL()) {return -1;}

    // --- LOADING SHADERS ---
    ShaderProgram lightingShader;
    if (!lightingShader.loadShaders("shaders/lighting_dir.vert", "shaders/lighting_dir.frag")) {
        std::cerr << "Erreur chargement shaders !" << std::endl;
        return -1;
    }

    // --- LOADING MESHES ---
    Mesh cubeMesh;
    if (!cubeMesh.loadOBJ("models/woodcrate.obj")) {
        std::cerr << "Erreur chargement OBJ !" << std::endl;
        // Keep up without crashing
    }

    Mesh floorMesh;
    if (!floorMesh.loadOBJ("models/floor.obj")) {std::cerr << "Erreur chargement OBJ !" << std::endl;}

    // --- LOADING TEXTURES ---
    Texture2D cubeTexture;
    cubeTexture.loadTexture("textures/woodcrate_diffuse.jpg", true);

    Texture2D floorTexture;
    floorTexture.loadTexture("textures/tile_floor.jpg", true);

    double lastTime = glfwGetTime();

    // --- Main Loop ---
    while (!glfwWindowShouldClose(gWindow)) {
        showFPS(gWindow);

        // Time handeling (DeltaTime)
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Input Management
        glfwPollEvents();
        update(deltaTime);

        // Cleaning screen buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // -- RENDERING ZONE ---
        // Activating the Shader
        lightingShader.use();

        // -- Calculating the Transformation Matrix --
        // VIEW : Camera Position
        glm::mat4 view = fpsCamera.getViewMatrix();

        // PROJECTION : Perspective (FOV, Screen ratio, Near plane, Far plane)
        glm::mat4 projection = glm::perspective(glm::radians(fpsCamera.getFOV()),
                               (float)gWindowWidth / (float)gWindowHeight,
                               0.1f, 100.0f);


        // -- Sending Uniforms to Shader --
        // The shader should be actif
        lightingShader.setUniform("view", view);
        lightingShader.setUniform("projection", projection);


        // Uniforms of Lighting Directional
        lightingShader.setUniform("viewPos", fpsCamera.getPosition());

        // Properties of directional lighting
        lightingShader.setUniform("dirLight.direction", glm::vec3(0.0f, -1.0f, 1.0f)); // Comming from the top and back
        lightingShader.setUniform("dirLight.ambient",   glm::vec3(0.1f, 0.1f, 0.1f));   // stale lighting everywhere
        lightingShader.setUniform("dirLight.diffuse",   glm::vec3(0.9f, 0.9f, 0.9f));   // Shiny colour
        lightingShader.setUniform("dirLight.specular",  glm::vec3(1.0f, 1.0f, 1.0f));   // Reflects in pure white

        // Material Properties
        lightingShader.setUniform("material.ambient",   glm::vec3(1.0f, 1.0f, 1.0f)); // White to let the ambient transperse
        lightingShader.setUniform("material.specular",  glm::vec3(0.7f, 0.7f, 0.7f)); // Force reflection
        lightingShader.setUniform("material.shininess", 64.0f);

        // -- Floor Rendering --
        // Matrix model of the floor placed at y=0 and scaled up to 10x
        glm::mat4 floorModel = glm::mat4(1.0f);
        floorModel = glm::scale(floorModel, glm::vec3(10.0f, 1.0f, 10.0f));

        lightingShader.setUniform("model", floorModel); // Sending the Model to shader

        // Floor Material and Texture
        lightingShader.setUniformSampler("material.diffuseMap", 0);
        floorTexture.bind(0);

        floorMesh.draw(); // Drawing the floor

        // -- Cube Rendering --
        // Matrix Model of the Cube : placed right above the ground at y=1.0f
        glm::mat4 cubeModel = glm::mat4(1.0f);
        cubeModel = glm::translate(cubeModel, glm::vec3(0.0f, 1.0f, 0.0f));
        // Rotation
        cubeModel = glm::rotate(cubeModel, (float)glfwGetTime() * 0.5f, glm::vec3(0.5f, 1.0f, 0.0f));

        lightingShader.setUniform("model", cubeModel); // Sending the model to shader

        // Cube Material and Texture
        cubeTexture.bind(0); // Unit 0 is used but with cube Texture
        // no need to redo setUniformSampler if using the same slot 0)

        cubeMesh.draw(); // Drawing the cube

        // Unbinding
        floorTexture.unbind(0);
        glUseProgram(0);

        // Swap buffers
        glfwSwapBuffers(gWindow);
    }

    endOpenGL();
    return 0;
}

// -- UPDATING SCREEN --
void update(double elapsedTime)
{
    // --- CAMERA ROTATION (MOUSE) ---
    double mouseX, mouseY;
    glfwGetCursorPos(gWindow, &mouseX, &mouseY);

    // Calculating the gap between the screen center and mouse position
    float deltaX = (float)(gWindowWidth / 2.0 - mouseX);
    float deltaY = (float)(gWindowHeight / 2.0 - mouseY);

    // Applying rotation to the camera using FPSCamera class rotate method
    if (deltaX != 0 || deltaY != 0) {
        fpsCamera.rotate(deltaX * MOUSE_SENSITIVITY, deltaY * MOUSE_SENSITIVITY);
    }

    // Recentering the mouse to avoid overflowing the screen space
    glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

    // --- KEY MOVEMENT ---
    // Calculating the distance for a given frame (Speed * DeltaTime)
    float distance = MOVE_SPEED * (float)elapsedTime;

    // ZQSD for movement
    if (glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS)
        fpsCamera.move(distance * fpsCamera.getLook());
    else if (glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS)
        fpsCamera.move(distance * -fpsCamera.getLook());

    // Gauche / Droite (Strafe)
    if (glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS)
        fpsCamera.move(distance * -fpsCamera.getRight()); // -Right = Gauche
    else if (glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS)
        fpsCamera.move(distance * fpsCamera.getRight());
}

// -- INITIALISATION OPENGL
bool initOpenGL()
{
    if (!glfwInit()) return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
    if (gWindow == NULL) {
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(gWindow);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) return false;

    // Callbacks
    glfwSetKeyCallback(gWindow, glfw_onKey);

    // Mouse capturing FPS mode
    glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

    glClearColor(gClearColor.r, gClearColor.g, gClearColor.b, gClearColor.a);
    glEnable(GL_DEPTH_TEST); // Z-Buffer activation

    return true;
}

void endOpenGL() {
    glfwDestroyWindow(gWindow);
    glfwTerminate();
}

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // viewport matches the new window dimensions
    glViewport(0, 0, width, height);
}

void showFPS(GLFWwindow* window) {
    static double prevSec = 0.0;
    static int frameCount = 0;
    double currentSec = glfwGetTime();
    double elapsedSec = currentSec - prevSec;
    if (elapsedSec > 0.25) { // updates at 1 / 4 of a sec
        prevSec = currentSec;
        double fps = (double) frameCount / elapsedSec;
        double msPerFrame = 1000.0 / fps;
        std::ostringstream outs;
        outs.precision(3);
        outs << std::fixed
            << "fps : " << fps << " "
             << "ms : " << msPerFrame << "\n";
        glfwSetWindowTitle(window, outs.str().c_str());
        frameCount = 0;
    }
    frameCount++;
}