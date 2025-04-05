#ifndef SHADERS_H
#define SHADERS_H
const char* vertexShaderSource = R"(
    #version 460 core
    layout(location = 0) in vec3 aPos;
    void main() {
    gl_Position = vec4(aPos, 1.0); 
}
)";
const char* fragmentShaderSource = R"(
    #version 460 core
    out vec4 FragColor;
    uniform vec4 rectangleColor;
    void main() {
    FragColor = rectangleColor; 
}
)";
#endif#pragma once
