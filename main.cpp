// main.cpp
#include <iostream>
#include<sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLFWwindow * pWin = nullptr;
bool gWireframe = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // viewport matches the new window dimensions
    glViewport(0, 0, width, height);
}
void glfwSetKeys(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        gWireframe = !gWireframe;
        if (gWireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
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
bool initOpenGL() {
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        return false;
    }

    // Request an OpenGL 3.3 core profile (change if your hw supports different)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    pWin = glfwCreateWindow(640, 480, "GL test", nullptr, nullptr);
    if (!pWin) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(pWin);

    // With older GLEW + core profiles you should set this before glewInit
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to init GLEW\n";
        glfwTerminate();
        return false;
    }

    std::cout << "OpenGL: " << reinterpret_cast<const char*>(glGetString(GL_VERSION)) << "\n";
    std::cout << "Renderer: " << reinterpret_cast<const char*>(glGetString(GL_RENDERER)) << "\n";

    // Register callbacks
    glfwSetFramebufferSizeCallback(pWin, framebuffer_size_callback);
    glfwSetKeyCallback(pWin, glfwSetKeys);

    // Initial Viewport
    int width, height;
    glfwGetFramebufferSize(pWin, &width, &height);
    glViewport(0, 0, width, height);

    // set clear color once before the loop (Background color)
    glClearColor(0.5f, 0.0f, 0.5f, 1.0f);

    return true;
}
void endOpenGL() {
    glfwDestroyWindow(pWin);
    glfwTerminate();
}

int main() {
    if (!initOpenGL()) {return -1;}

    // Main loop
    while (!glfwWindowShouldClose(pWin)) {
        showFPS(pWin);
        glfwPollEvents();
        // clear the color buffer by setting the whole window with clearColor
        glClear(GL_COLOR_BUFFER_BIT);
        // swap buffers
        glfwSwapBuffers(pWin);
    }

    endOpenGL();
    return 0;
}