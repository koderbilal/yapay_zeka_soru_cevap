#version 330 core

in vec2 TexCoord;
in float Shade;
out vec4 FragColor;

uniform sampler2D atlas;

void main() {
    vec4 color = texture(atlas, TexCoord);
    if (color.a < 0.5) discard; // saplings and other cut-out tiles
    FragColor = vec4(color.rgb * Shade, 1.0);
}
