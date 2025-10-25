OpenGL & C++ Roadmap - Component-Based Helper Guide
===================================================

This document extracts the key concepts, code examples, and explanations from a step-by-step OpenGL learning progression.
It is designed as a reference for your ongoing improvement.

---

1. **Setup & Dependencies**
-------------------------
- Install essential libraries:
  ```bash
  sudo apt update
  sudo apt install build-essential cmake libglfw3-dev libglew-dev libglm-dev
  ```
- Use CLion for C++ IDE support.

- GLFW: Window/context management.
- GLEW: OpenGL extension loader.
- GLM: Mathematics library for transformations.
- STB Image: Image loading (textures).

---

2. **Minimal OpenGL Window**
----------------------------
**main.cpp:**
```cpp
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewInit();

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
}
```
- Key points: Create context, clear window, render loop.
- `glClearColor` sets background color.

---

3. **Shaders**
---------------
**Vertex Shader:**
```glsl
#version 330 core
layout (location = 0) in vec3 aPos;
void main(){ gl_Position = vec4(aPos,1.0); }
```
**Fragment Shader:**
```glsl
#version 330 core
out vec4 FragColor;
void main(){ FragColor = vec4(1.0,0.5,0.2,1.0); }
```

**C++ helper:**
```cpp
GLuint compileShader(GLenum type, const char* src){...}
GLuint createProgram(const char* vs, const char* fs){...}
```
- Compile vertex and fragment shaders, link into shader program.

---

4. **VBO & VAO**
-----------------
```cpp
float vertices[] = { 0.0f,0.5f,0.0f, -0.5f,-0.5f,0.0f, 0.5f,-0.5f,0.0f };
GLuint VBO, VAO;
glGenBuffers(1,&VBO);
glGenVertexArrays(1,&VAO);
glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER,VBO);
glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
glEnableVertexAttribArray(0);
```
- **VBO**: GPU storage for vertex data.
- **VAO**: Stores vertex attribute configuration.

---

5. **EBO (Element Buffer Object)**
----------------------------------
```cpp
unsigned int indices[] = {0,1,3, 1,2,3};
GLuint EBO;
glGenBuffers(1,&EBO);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
```
- Avoid vertex duplication.
- Useful for rectangles or complex meshes.

---

6. **Uniforms**
----------------
**Fragment Shader with uniform:**
```glsl
uniform vec4 ourColor;
out vec4 FragColor;
void main(){ FragColor = ourColor; }
```
**C++:**
```cpp
int loc = glGetUniformLocation(shaderProgram,"ourColor");
glUseProgram(shaderProgram);
glUniform4f(loc,0.0f,greenValue,0.0f,1.0f);
```
- Animate colors or other parameters at runtime.

---

7. **Transformations**
----------------------
- Use GLM for matrices.
```cpp
glm::mat4 model = glm::mat4(1.0f);
model = glm::rotate(model,(float)glfwGetTime(),glm::vec3(0,0,1));
glm::mat4 view = glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);
glm::mat4 projection = glm::perspective(glm::radians(45.0f),width/height,0.1f,100.0f);
glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
```
- Transform vertices: Model → World → Camera → Clip space.

---

8. **Textures**
----------------
- Load image using **STB**:
```cpp
int width,height,nrChannels;
unsigned char* data = stbi_load("image.jpg", &width, &height, &nrChannels, 0);
glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
stbi_image_free(data);
```
- Vertex shader passes `aTexCoord` to fragment shader.
- Fragment shader samples texture:
```glsl
uniform sampler2D ourTexture;
FragColor = texture(ourTexture, TexCoord);
```

---

9. **Basic Lighting (Phong)**
-----------------------------
- Add normals to vertices:
```cpp
float vertices[] = { x,y,z, nx,ny,nz, ... };
```
- Vertex shader passes position and normal.
- Fragment shader computes:
```glsl
ambient + diffuse + specular
```
- C++ sets uniforms: `lightPos`, `viewPos`, `lightColor`, `objectColor`.

---

10. **Camera & User Interaction**
----------------------------------
- Camera vectors: `cameraPos`, `cameraFront`, `cameraUp`
- Update view matrix:
```cpp
view = glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);
```
- Keyboard (WASD) moves camera.
- Mouse rotates camera by updating `yaw` and `pitch`.
- Scroll adjusts FOV (zoom).

---

11. **Advanced OpenGL Concepts**
---------------------------------
- **Instancing**: `glDrawArraysInstanced` for many objects.
- **Framebuffers**: render to texture (`glBindFramebuffer`).
- **Multiple lights/materials**: directional, point, spot.
- **Advanced shaders**: normal mapping, specular maps.
- **Performance**: reuse VAOs/VBOs, culling, LOD.
- Structure code with **Shader, Texture, Mesh, Camera classes**.

---

**Experiments / Practice**
--------------------------
1. Rotate colored triangle, then rectangle.
2. Map textures and animate UV coordinates.
3. Apply lighting and move light position.
4. Implement interactive camera with WASD + mouse.
5. Try instancing multiple objects.
6. Explore framebuffers and post-processing.

---

This guide condenses **key code snippets, explanations, and best practices** into a single reference file for ongoing learning and experimentation.

