
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

// Dimensões da janela (pode ser alterado em tempo de execução)

const GLuint WIDTH = 800, HEIGHT = 800;
const int CIRCLE_SEGMENTS = 100;

// Código fonte do Vertex Shader (em GLSL): ainda hardcoded
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

// Código fonte do Fragment Shader (em GLSL): ainda hardcoded
const GLchar *fragmentShaderSource = R"(
 #version 400
 uniform vec4 inputColor;
 out vec4 color;
 void main() {
     color = inputColor;
 }
 )";

// Função de callback de teclado - chamada sempre que uma tecla for pressionada ou solta via GLFW
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

// Esta função está bastante hardcoded - objetivo é compilar e "buildar" um programa de shader simples e único
// O código fonte do vertex e fragment shader está nos arrays vertexShaderSource e fragmentShaderSource no início deste arquivo
// A função retorna o identificador do programa de shader
int setupShader()
{
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Checando erros de compilação (exibição via log no terminal)
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Checando erros de compilação (exibição via log no terminal)
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    // Linkando os shaders e criando o identificador do programa de shader
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Checando por erros de linkagem
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

// Esta função está bastante hardcoded - objetivo é criar os buffers que armazenam a geometria de um círculo
// Apenas atributo coordenada nos vértices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A função retorna o identificador do VAO
int setupGeometry()
{
    // Aqui setamos as coordenadas x, y e z dos pontos do círculo e as armazenamos de forma
    // sequencial, já visando mandar para o VBO (Vertex Buffer Objects)
    // O círculo é desenhado usando a equação paramétrica: x = cx + r*cos(theta), y = cy + r*sin(theta)
    // Círculo centralizado em (400, 300) com raio 100
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
    // Geração do identificador do VBO
    glGenBuffers(1, &VBO);
    // Faz a conexão (vincula) do buffer como um buffer de array
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Envia os dados do array de floats para o buffer da OpenGl
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Geração do identificador do VAO (Vertex Array Object)
    glGenVertexArrays(1, &VAO);
    // Vincula (bind) o VAO primeiro, e em seguida conecta e seta o(s) buffer(s) de vértices
    // e os ponteiros para os atributos
    glBindVertexArray(VAO);
    // Para cada atributo do vértice, criamos um "AttribPointer" (ponteiro para o atributo), indicando:
    //  Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
    //  Numero de valores que o atributo tem (por ex, 3 coordenadas xyz)
    //  Tipo do dado
    //  Se está normalizado (entre zero e um)
    //  Tamanho em bytes
    //  Deslocamento a partir do byte zero
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    // Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice
    // atualmente vinculado - para que depois possamos desvincular com segurança
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
    glBindVertexArray(0);

    return VAO;
}

// Função MAIN
int main()
{
    // Inicialização da GLFW
    glfwInit();

    // Configuração da GLFW - Versão e perfil do OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Criação da janela GLFW
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Circulo", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Falha ao criar a janela GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Fazendo o registro da função de callback para a janela GLFW
    glfwSetKeyCallback(window, key_callback);

    // GLAD: carrega todos os ponteiros das funções da OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Falha ao inicializar GLAD" << std::endl;
        return -1;
    }

    // Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Configurando a janela do mundo (projeção ortográfica)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 800.0, 600.0, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Compilando e buildando o programa de shader
    GLuint shaderID = setupShader();

    // Gerando um buffer com a geometria de um círculo
    GLuint VAO = setupGeometry();

    // Enviando a cor desejada (vec4) para o fragment shader
    // Utilizamos a variável do tipo uniform em GLSL para armazenar esse tipo de info
    // que não está nos buffers
    GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");
    GLint widthLoc = glGetUniformLocation(shaderID, "u_width");
    GLint heightLoc = glGetUniformLocation(shaderID, "u_height");

    glUseProgram(shaderID); // Reseta o estado do shader para evitar problemas futuros
    glUniform1f(widthLoc, 800.0f);
    glUniform1f(heightLoc, 600.0f);

    // Loop da aplicação - "game loop"
    while (!glfwWindowShouldClose(window))
    {
        // Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
        glfwPollEvents();

        // Limpa o buffer de cor
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f); // cor de fundo preto escuro
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO); // Conectando ao buffer de geometria

        // Desenha círculo azul claro no centro
        glUniform4f(colorLoc, 0.2f, 0.8f, 1.0f, 1.0f); // azul claro
        glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_SEGMENTS + 2);

        // Desenha um retângulo vermelho no canto superior esquerdo (0,0)-(100,100)
        float rect[] = {
            0.0f, 0.0f, 0.0f,
            100.0f, 0.0f, 0.0f,
            100.0f, 100.0f, 0.0f,
            0.0f, 100.0f, 0.0f};
        GLuint rectVBO, rectVAO;
        glGenVertexArrays(1, &rectVAO);
        glGenBuffers(1, &rectVBO);
        glBindVertexArray(rectVAO);
        glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);
        glEnableVertexAttribArray(0);
        glUniform4f(colorLoc, 1.0f, 0.2f, 0.2f, 1.0f); // vermelho
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glDeleteBuffers(1, &rectVBO);
        glDeleteVertexArrays(1, &rectVAO);

        // Troca os buffers da tela
        glfwSwapBuffers(window);
    }
    // Pede pra OpenGL desalocar os buffers
    glDeleteVertexArrays(1, &VAO);
    // Finaliza a execução da GLFW, limpando os recursos alocados por ela
    glfwTerminate();
    return 0;
}
