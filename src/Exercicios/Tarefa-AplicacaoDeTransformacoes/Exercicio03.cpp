#include <iostream>
#include <vector>
#include <cstdlib> // rand()
#include <ctime>   // time()
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "createTriangle.h" // usa a função createTriangle

// Estrutura para armazenar os triângulos
struct Triangle
{
    glm::vec2 position;
    glm::vec3 color;
};

// Lista global de triângulos criados pelo usuário
std::vector<Triangle> triangles;

// Shaders
const GLchar *vertexShaderSource = R"(
#version 400
layout (location = 0) in vec3 position;

uniform mat4 model;

void main() {
    gl_Position = model * vec4(position, 1.0);
}
)";

const GLchar *fragmentShaderSource = R"(
#version 400
out vec4 fragColor;

uniform vec3 triangleColor;

void main() {
    fragColor = vec4(triangleColor, 1.0);
}
)";

// Funções auxiliares
GLuint compileShader(const GLchar *source, GLenum type)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
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

// Função callback para clique do mouse
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        int width, height;
        glfwGetCursorPos(window, &xpos, &ypos);
        glfwGetWindowSize(window, &width, &height);

        // Converter para coordenadas Normalized Device Coordinates (NDC)
        float x = (xpos / width) * 2.0f - 1.0f;
        float y = 1.0f - (ypos / height) * 2.0f;

        // Cor aleatória
        float r = static_cast<float>(rand()) / RAND_MAX;
        float g = static_cast<float>(rand()) / RAND_MAX;
        float b = static_cast<float>(rand()) / RAND_MAX;

        triangles.push_back({glm::vec2(x, y), glm::vec3(r, g, b)});
    }
}

int main()
{
    srand(static_cast<unsigned>(time(0)));

    // Inicializa o GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Exercicio 3 - Triangulos com Matriz de Transformacao", nullptr, nullptr);
    if (!window)
        return -1;

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return -1;

    // Callback de clique
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    GLuint shaderProgram = createProgram();

    // Cria o triângulo padrão
    GLuint triangleVAO = createTriangle(-0.1f, -0.1f, 0.1f, -0.1f, 0.0f, 0.1f);

    // Localizações dos uniforms
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint colorLoc = glGetUniformLocation(shaderProgram, "triangleColor");

    // Loop principal
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(triangleVAO);

        for (const auto &t : triangles)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(t.position, 0.0f));

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniform3fv(colorLoc, 1, glm::value_ptr(t.color));

            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
