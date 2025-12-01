#version 330 core
// Ces layout correspondent aux "glVertexAttribPointer" dans Mesh.cpp
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

// Variables uniformes (envoyées depuis le C++)
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Transformation classique : Projection * Vue * Modèle * Position
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
}