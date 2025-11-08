#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "createTriangle.h"

// --- Shaders ---
const GLchar *vertexShaderSource = R"(
#version 400
layout (location = 0) in vec3 position;
void main() {
    gl_Position = vec4(position, 1.0);
}
)";

const GLchar *fragmentShaderSource = R"(
#version 400
out vec4 color;
uniform vec4 inputColor;
void main() {
    color = inputColor;
}
)";

GLuint compileShader(const GLchar *src, GLenum type)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    return shader;
}

GLuint createProgram()
{
    GLuint vs = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fs = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Exercicio 2 - 5 Triangulos", nullptr, nullptr);
    if (!window)
        return -1;
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return -1;

    GLuint shaderProgram = createProgram();
    glUseProgram(shaderProgram);

    // Usa a função importada!
    std::vector<GLuint> triangles;
    triangles.push_back(createTriangle(-0.8f, -0.5f, -0.4f, -0.5f, -0.6f, -0.1f));
    triangles.push_back(createTriangle(-0.3f, -0.5f, 0.1f, -0.5f, -0.1f, -0.1f));
    triangles.push_back(createTriangle(0.2f, -0.2f, 0.6f, -0.2f, 0.4f, 0.2f));
    triangles.push_back(createTriangle(-0.6f, 0.2f, -0.2f, 0.2f, -0.4f, 0.6f));
    triangles.push_back(createTriangle(0.4f, 0.4f, 0.8f, 0.4f, 0.6f, 0.8f));

    GLint colorLoc = glGetUniformLocation(shaderProgram, "inputColor");

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (size_t i = 0; i < triangles.size(); ++i)
        {
            switch (i)
            {
            case 0:
                glUniform4f(colorLoc, 1.0, 0.0, 0.0, 1.0);
                break;
            case 1:
                glUniform4f(colorLoc, 0.0, 1.0, 0.0, 1.0);
                break;
            case 2:
                glUniform4f(colorLoc, 0.0, 0.0, 1.0, 1.0);
                break;
            case 3:
                glUniform4f(colorLoc, 1.0, 1.0, 0.0, 1.0);
                break;
            case 4:
                glUniform4f(colorLoc, 1.0, 0.0, 1.0, 1.0);
                break;
            }
            glBindVertexArray(triangles[i]);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        glfwSwapBuffers(window);
    }

    for (auto vao : triangles)
        glDeleteVertexArrays(1, &vao);

    glfwTerminate();
    return 0;
}
