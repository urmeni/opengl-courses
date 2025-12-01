#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

// Le sampler (la texture)
uniform sampler2D myTexture;

void main()
{
    // On récupère la couleur du pixel sur la texture
    FragColor = texture(myTexture, TexCoords);
}