#version 450

layout(location = 0) in vec2 position;       // position.xy
layout(location = 1) in vec3 color;          // RGB vertex color
layout(location = 2) in vec2 texCoord;       // UV coordinates

layout(location = 0) out vec3 fragColor;     // pass to fragment shader
layout(location = 1) out vec2 fragTexCoord;  // pass to fragment shader

void main() {
    gl_Position = vec4(position, 0.0, 1.0);  // 2D position to clip space
    fragColor = color;
    fragTexCoord = texCoord;
}
