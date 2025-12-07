// main.cpp
#include <iostream>
#include<sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <Camera.h>
#include <Mesh.h>
#include <ShaderProgram.h>
#include <Texture2D.h>

// --- GLOBAL VARIABLES ---
const char* APP_TITLE = "Ma Scene Finale";
int gWindowWidth = 1024;
int gWindowHeight = 768;
GLFWwindow * gWindow = nullptr;
bool gWireframe = false;

// Background Color
glm::vec4 gClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Dark blue

// Camera Configuration
FPSCamera fpsCamera(glm::vec3(0.0f, 2.0f, 10.0f));
const float MOVE_SPEED = 5.0f;
const float MOUSE_SENSITIVITY = 0.1f;

// Scene Configuration
const int numModels = 25;
Mesh mesh[numModels];
Texture2D texture[numModels];
glm::vec3 modelPos[numModels];
glm::vec3 modelScale[numModels];


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

    // --- LOADING ASSETS ---
    // OBJ 0 : Ground
    mesh[0].loadOBJ("models/ground.obj");
    texture[0].loadTexture("textures/ground.png", true);
    modelPos[0] = glm::vec3(0.0f, 0.0f, 0.0f);
    modelScale[0] = glm::vec3(0.15f, 0.15f, 0.15f);

    // OBJ 1 : Bags
    mesh[1].loadOBJ("models/bags.obj");
    texture[1].loadTexture("textures/bags.png", true);
    modelPos[1] = glm::vec3(2.0f, 0.0f, -2.5f);
    modelScale[1] = glm::vec3(0.5f, 0.5f, 0.5f);

    // OBJ 2 : Barrel
    mesh[2].loadOBJ("models/fire_barrel.obj");
    texture[2].loadTexture("textures/fire_barrel.png", true);
    modelPos[2] = glm::vec3(0.0f, 0.0f, 2.0f);
    modelScale[2] = glm::vec3(1.0f, 1.0f, 1.0f);

    // OBJ 3 : Mattress
    mesh[3].loadOBJ("models/mattress.obj");
    texture[3].loadTexture("textures/mattress.png", true);
    modelPos[3] = glm::vec3(2.0f, 0.0f, 0.0f);
    modelScale[3] = glm::vec3(1.0f, 1.0f, 1.0f);

    // OBJ 4 : Pirozhok
    mesh[4].loadOBJ("models/pirozhok.obj");
    texture[4].loadTexture("textures/pirozhok.png", true);
    modelPos[4] = glm::vec3(-3.0f, 0.0f, 0.0f);
    modelScale[4] = glm::vec3(0.04f, 0.04f, 0.04f);

    // OBJ 5 : Mattress
    mesh[5].loadOBJ("models/mattress.obj");
    texture[5].loadTexture("textures/mattress.png", true);
    modelPos[5] = glm::vec3(2.0f, 0.0f, 4.0f);
    modelScale[5] = glm::vec3(1.0f, 1.0f, 1.0f);

    // OBJ 6 : Pirozhok
    mesh[6].loadOBJ("models/pirozhok.obj");
    texture[6].loadTexture("textures/pirozhok.png", true);
    modelPos[6] = glm::vec3(0.0f, 5.0f, -4.0f);
    modelScale[6] = glm::vec3(0.04f, 0.04f, 0.04f);

    // OBJ 7 : Bags
    mesh[7].loadOBJ("models/bags.obj");
    texture[7].loadTexture("textures/bags.png", true);
    modelPos[7] = glm::vec3(-4.0f, 0.0f, 1.0f);
    modelScale[7] = glm::vec3(0.4f, 0.4f, 0.4f);




    // OBJ 8 : Fences back
    mesh[8].loadOBJ("models/fence.obj");
    texture[8].loadTexture("textures/fence.png", true);
    modelPos[8] = glm::vec3(12.0f, 2.0f, 1.0f);
    modelScale[8] = glm::vec3(1.0f, 1.0f, 1.0f);


    // OBJ 9 : Fence front
    mesh[9].loadOBJ("models/fence.obj");
    texture[9].loadTexture("textures/fence.png", true);
    modelPos[9] = glm::vec3(-12.0f, 2.0f, -6.0f);
    modelScale[9] = glm::vec3(1.0f, 1.0f, 1.0f);


    // OBJ 10 : Fences front
    mesh[10].loadOBJ("models/fence.obj");
    texture[10].loadTexture("textures/fence.png", true);
    modelPos[10] = glm::vec3(-11.0f, 1.8f, 4.0f);
    modelScale[10] = glm::vec3(1.0f, 1.0f, 1.0f);

    // OBJ 11 : Fences back
    mesh[11].loadOBJ("models/fence.obj");
    texture[11].loadTexture("textures/fence.png", true);
    modelPos[11] = glm::vec3(13.0f, 2.0f, -10.0f);
    modelScale[11] = glm::vec3(1.0f, 1.0f, 1.0f);

    // OBJ 12 : Fences back
    mesh[12].loadOBJ("models/fence.obj");
    texture[12].loadTexture("textures/fence.png", true);
    modelPos[12] = glm::vec3(12.75f, 2.0f, 10.0f);
    modelScale[12] = glm::vec3(1.0f, 1.0f, 1.0f);

    // OBJ 13 : Fences front
    mesh[13].loadOBJ("models/fence.obj");
    texture[13].loadTexture("textures/fence.png", true);
    modelPos[13] = glm::vec3(-11.75f, 1.8f, 12.5f);
    modelScale[13] = glm::vec3(1.0f, 1.0f, 1.0f);

    // OBJ 14 : Sign
    mesh[14].loadOBJ("models/sign.obj");
    texture[14].loadTexture("textures/sign.png", true);
    modelPos[14] = glm::vec3(2.0f, 0.0f, 2.0f);
    modelScale[14] = glm::vec3(1.0f, 1.0f, 1.0f);

    // OBJ 15 : Buildings
    mesh[15].loadOBJ("models/building.obj");
    texture[15].loadTexture("textures/building.png", true);
    modelPos[15] = glm::vec3(-30.0f, 0.0f, 2.0f);
    modelScale[15] = glm::vec3(1.0f, 1.0f, 1.0f);

    // OBJ 16 : Buildings
    mesh[16].loadOBJ("models/building.obj");
    texture[16].loadTexture("textures/building.png", true);
    modelPos[16] = glm::vec3(30.0f, 0.0f, -40.0f);
    modelScale[16] = glm::vec3(1.0f, 1.0f, 1.0f);

    // OBJ 17 : Buildings
    mesh[17].loadOBJ("models/building.obj");
    texture[17].loadTexture("textures/building.png", true);
    modelPos[17] = glm::vec3(0.0f, 0.0f, -30.0f);
    modelScale[17] = glm::vec3(1.0f, 1.0f, 1.0f);

    // OBJ 18 : ferris
    mesh[18].loadOBJ("models/ferris.obj");
    texture[18].loadTexture("textures/ferris.jpg", true);
    modelPos[18] = glm::vec3(0.0f, 0.0f, 30.0f);
    modelScale[18] = glm::vec3(10.0f, 10.0f, 10.0f);


    // OBJ 19 : Energetic
    mesh[19].loadOBJ("models/energetic.obj");
    texture[19].loadTexture("textures/energetic.jpg", true);
    modelPos[19] = glm::vec3(30.0f, 0.0f, 0.0f);
    modelScale[19] = glm::vec3(11.0f, 11.0f, 11.0f);



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

        // Properties of directional lighting (SUN)
        lightingShader.setUniform("dirLight.direction", glm::vec3(0.0f, -1.0f, -1.0f)); // Comming from the top and back
        lightingShader.setUniform("dirLight.ambient",   glm::vec3(0.001f, 0.001f, 0.001f));   // stale lighting everywhere
        lightingShader.setUniform("dirLight.diffuse",   glm::vec3(0.9f, 0.9f, 0.9f));   // Shiny colour
        lightingShader.setUniform("dirLight.specular",  glm::vec3(1.0f, 1.0f, 1.0f));   // Reflects in pure white

        // --- CONFIGURATION PIROZHOK 6 LIGHT (Point Light) ---
        lightingShader.setUniform("pointLight.position", modelPos[6]);

        lightingShader.setUniform("pointLight.ambient",  glm::vec3(2.0f, 2.0f, 2.0f)); // Faible lueur jaune
        lightingShader.setUniform("pointLight.diffuse",  glm::vec3(1.0f, 0.8f, 0.6f)); // Éclairage chaud fort
        lightingShader.setUniform("pointLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        // Atténuation 50 unit distance
        lightingShader.setUniform("pointLight.constant",  1.0f);
        lightingShader.setUniform("pointLight.linear",    0.09f);
        lightingShader.setUniform("pointLight.quadratic", 0.032f);

        // -- Drawing Loop --
        for (int i = 0; i < numModels; i++)
        {
            // Calculating model Matrix for given object
            glm::mat4 model = glm::mat4(1.0f);

            // Position
            model = glm::translate(model, modelPos[i]);

            // Rotation simple animation for pirozhok
            if (i == 6) {
                model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.3f, 1.0f, 0.7f));
                lightingShader.setUniform("material.ambient", glm::vec3(2.0f, 2.0f, 2.0f));
                lightingShader.setUniform("material.specular", glm::vec3(0.0f, 0.0f, 0.0f));
            } else {
                // Pour tous les autres objets, comportement normal
                lightingShader.setUniform("material.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
            }

            // scaling
            model = glm::scale(model, modelScale[i]);

            lightingShader.setUniform("model", model);


            // If the floor (i==0), makes it less shiny
            if (i == 0) {
                lightingShader.setUniform("material.specular", glm::vec3(0.6f, 0.6f, 0.6f));
                lightingShader.setUniform("material.shininess", 30.0f);
            } else {
                lightingShader.setUniform("material.specular", glm::vec3(0.6f, 0.6f, 0.6f));
                lightingShader.setUniform("material.shininess", 32.0f);
            }

            // Texture
            lightingShader.setUniformSampler("material.diffuseMap", 0);
            texture[i].bind(0);

            // Draw
            mesh[i].draw();
            texture[i].unbind(0);
        }


        // Unbinding
        texture[0].unbind(0);
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