#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float aShade;
layout (location = 3) in vec3 aTint;
layout (location = 4) in float aMaterial;
layout (location = 5) in float aAlpha;

out vec2 TexCoord;
out float Shade;
out vec3 Tint;
out float Material;
out float Alpha;

uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    Shade = aShade;
    Tint = aTint;
    Material = aMaterial;
    Alpha = aAlpha;
}
