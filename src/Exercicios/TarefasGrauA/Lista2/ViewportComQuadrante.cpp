#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupShader();
int setupGeometry();

// Dimensões da janela
const GLuint WIDTH = 800, HEIGHT = 600;
const int CIRCLE_SEGMENTS = 100;

// Código fonte do Vertex Shader (GLSL)
const GLchar *vertexShaderSource = R"(
 #version 400
 layout (location = 0) in vec3 position;
 uniform float u_width;
 uniform float u_height;
 void main() {
     float x_ndc = (position.x / u_width) * 2.0 - 1.0;
     float y_ndc = (position.y / u_height) * 2.0 - 1.0;
     gl_Position = vec4(x_ndc, y_ndc, position.z, 1.0);
 }
 )";

// Código fonte do Fragment Shader (GLSL)
const GLchar *fragmentShaderSource = R"(
 #version 400
 uniform vec4 inputColor;
 out vec4 color;
 void main() {
     color = inputColor;
 }
 )";

// Função de callback de teclado
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

// Compilação e linkagem dos shaders
int setupShader()
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

// Criação do círculo
int setupGeometry()
{
    float cx = 400.0f, cy = 300.0f, r = 100.0f;
    float vertices[(CIRCLE_SEGMENTS + 2) * 3];

    vertices[0] = cx;
    vertices[1] = cy;
    vertices[2] = 0.0f;

    for (int i = 0; i <= CIRCLE_SEGMENTS; ++i)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(CIRCLE_SEGMENTS);
        float x = cx + r * cosf(theta);
        float y = cy + r * sinf(theta);
        vertices[(i + 1) * 3 + 0] = x;
        vertices[(i + 1) * 3 + 1] = y;
        vertices[(i + 1) * 3 + 2] = 0.0f;
    }

    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

// MAIN
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Circulo no quadrante superior direito", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Falha ao criar a janela GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Falha ao inicializar GLAD" << std::endl;
        return -1;
    }

    GLuint shaderID = setupShader();
    GLuint VAO = setupGeometry();

    GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");
    GLint widthLoc = glGetUniformLocation(shaderID, "u_width");
    GLint heightLoc = glGetUniformLocation(shaderID, "u_height");

    glUseProgram(shaderID);
    glUniform1f(widthLoc, 800.0f);
    glUniform1f(heightLoc, 600.0f);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Limpa a tela inteira
        glViewport(0, 0, WIDTH, HEIGHT);
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Desenhar linhas pontilhadas cinza para dividir os quadrantes (viewport total)
        glUseProgram(shaderID);
        glUniform1f(widthLoc, 800.0f);
        glUniform1f(heightLoc, 600.0f);
        glUniform4f(colorLoc, 0.5f, 0.5f, 0.5f, 1.0f); // cinza
        float dash = 10.0f, gap = 10.0f;
        // Linha vertical (x = 400)
        for (float y = 0; y < HEIGHT; y += dash + gap)
        {
            float vline[] = {
                400.0f, y, 0.0f,
                400.0f, std::min(y + dash, (float)HEIGHT), 0.0f};
            GLuint vbo, vao;
            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vline), vline, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);
            glEnableVertexAttribArray(0);
            glDrawArrays(GL_LINES, 0, 2);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            glDeleteBuffers(1, &vbo);
            glDeleteVertexArrays(1, &vao);
        }
        // Linha horizontal (y = 300)
        for (float x = 0; x < WIDTH; x += dash + gap)
        {
            float hline[] = {
                x, 300.0f, 0.0f,
                std::min(x + dash, (float)WIDTH), 300.0f, 0.0f};
            GLuint vbo, vao;
            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(hline), hline, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);
            glEnableVertexAttribArray(0);
            glDrawArrays(GL_LINES, 0, 2);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            glDeleteBuffers(1, &vbo);
            glDeleteVertexArrays(1, &vao);
        }

        // Agora desenha o círculo só no quadrante superior direito
        glViewport(WIDTH / 2, HEIGHT / 2, WIDTH / 2, HEIGHT / 2);
        glUniform1f(widthLoc, 800.0f);
        glUniform1f(heightLoc, 600.0f);
        glBindVertexArray(VAO);
        glUniform4f(colorLoc, 0.2f, 0.8f, 1.0f, 1.0f); // azul claro
        glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_SEGMENTS + 2);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glfwTerminate();
    return 0;
}
