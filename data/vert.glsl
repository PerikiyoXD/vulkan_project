#version 450

layout(location = 0) in vec2 inPosition;
layout(push_constant) uniform PushConstants {
    float rotationAngle;
} pushConstants;

void main() {
    // Rotate the position
    float cosAngle = cos(pushConstants.rotationAngle);
    float sinAngle = sin(pushConstants.rotationAngle);
    vec2 rotatedPosition = vec2(
        inPosition.x * cosAngle - inPosition.y * sinAngle,
        inPosition.x * sinAngle + inPosition.y * cosAngle
    );
    gl_Position = vec4(rotatedPosition, 0.0, 1.0);
}
