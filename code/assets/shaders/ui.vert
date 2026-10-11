#version 330 core

layout (location = 0) in vec2 aPos;

out vec2 TexCoord;

uniform mat4 projection;
uniform vec4 rect;   // x, y, width, height in pixels, origin top-left
uniform vec4 uvRect; // u, v, width, height in texture space

void main() {
    vec2 pixel = rect.xy + aPos * rect.zw;
    gl_Position = projection * vec4(pixel, 0.0, 1.0);
    TexCoord = uvRect.xy + aPos * uvRect.zw;
}
