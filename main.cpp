// main.cpp
#include <iostream>
#include<sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLFWwindow * pWin = NULL;
bool gWireframe = false;
const GLchar* vertexShaderSrc =
"#version 330 core\n"
"layout (location = 0) in vec3 pos;"
"void main()"
"{"
"   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);"
"}";
const GLchar* fragmentShaderSrc =
"#version 330 core\n"
"out vec4 frag_color;"
"void main()"
"{"
"   frag_color = vec4(0.8f, 0.0f, 0.0f, 1.0f);"
"}";
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

    // Vertex Array Object
    GLfloat vertices[] =
    {
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    GLuint indices[] =
    {
        0, 1, 2, // tri 0
        0, 2, 3  // tri 1
    };

    GLuint vbo, ibo, vao;

    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    // Vertex Shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSrc, NULL);
    glCompileShader(vs);
    GLint result;
    GLchar infoLog[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
        std::cout << "Vertex Shader Compilation error : " << infoLog << std::endl;
    }

    // Fragment shader
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fs);

    glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
        std::cout << "Fragment shader Compilation error : " << infoLog << std::endl;
    }

    // Shader Program Linking
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
        std::cout << "Shader Program Linking Failure : " << infoLog << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    // Main loop
    while (!glfwWindowShouldClose(pWin)) {
        showFPS(pWin);
        glfwPollEvents();

        // clear the color buffer by setting the whole window with clearColor
        glClear(GL_COLOR_BUFFER_BIT);

        // Shader Program
        glUseProgram(shaderProgram);

        // Vertex Array Object Binding
        glBindVertexArray(vao);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // swap buffers
        glfwSwapBuffers(pWin);
    }

    glDeleteProgram(shaderProgram);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    endOpenGL();
    return 0;
}