#version 330 core

layout(location = 0) in vec3 inPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 direction;

void main() {
    vec3 endPoint = direction;
    vec3 orthogonal = normalize(cross(direction, vec3(0.0, 0.0, 1.0))) * 0.01; // Orthogonal vector scaled to 2px

    // Calculate the position in the local space of the arrow
    vec3 localPos = inPosition.x * endPoint + inPosition.y * orthogonal;

    // Transform the position to the world space using the model matrix
    vec4 worldPos = projection * view * model * vec4(localPos, 1.0);

    gl_Position = worldPos;
}
