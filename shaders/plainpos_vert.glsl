#version 450
layout (location = 0) in vec3 aPos;

out vec2 fragCoord;

void main()
{
    fragCoord = vec2(aPos.x, aPos.y);
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}