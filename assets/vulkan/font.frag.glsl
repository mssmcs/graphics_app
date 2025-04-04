#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;
layout(set = 1, binding = 2) uniform sampler2D texSampler;

void main() {
    outColor = texture(texSampler, fragTexCoord).r * fragColor;
}


