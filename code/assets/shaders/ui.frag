#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D image;
uniform vec4 color;
uniform int useTexture;

void main() {
    vec4 tex = useTexture == 1 ? texture(image, TexCoord) : vec4(1.0);
    if (useTexture == 1 && tex.a < 0.1) discard;
    FragColor = vec4(tex.rgb * color.rgb, tex.a * color.a);
}
