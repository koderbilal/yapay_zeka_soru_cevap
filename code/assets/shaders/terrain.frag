#version 330 core

in vec2 TexCoord;
in float Shade;
in vec3 Tint;
in float Material;
in float Alpha;
out vec4 FragColor;

uniform sampler2D atlas;

void main() {
    vec4 tex = texture(atlas, TexCoord);
    // 1 = cutout (leaves, sapling, cactus edge, dead bush). 2 = grass mask.
    if (Material > 0.5 && Material < 1.5 && tex.a < 0.5) discard;
    float mask = Material > 1.5 ? tex.a : 1.0;
    vec3 rgb = tex.rgb * mix(vec3(1.0), Tint, mask) * Shade;
    FragColor = vec4(rgb, Alpha);
}
