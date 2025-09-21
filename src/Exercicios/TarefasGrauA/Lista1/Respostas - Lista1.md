## IMPORTANTE: Arquivo markdown, acesse corretamente para facilitar a leitura !!!

### 1. O que é a GLSL? Quais os dois tipos de shaders são obrigatórios no pipeline programável da versão atual que trabalhamos em aula e o que eles processam?

GLSL (**OpenGL Shading Language**) é a linguagem de programação usada para escrever **shaders** no OpenGL.

* É baseada em **C**, mas possui recursos específicos para computação gráfica.
* Permite ao programador escrever **programas que rodam na GPU**, controlando como vértices e pixels são processados.
* Facilita efeitos avançados, como iluminação dinâmica, texturização, deformações e pós-processamento.

Em resumo, GLSL é **a linguagem que você usa para programar o pipeline gráfico da GPU** no OpenGL.

Já os Shaders obrigatórios no pipeline programável moderno do OpenGL são:

1. **Vertex Shader (Shader de Vértice)**

   * É o primeiro estágio do pipeline programável.
   * **Entrada:** cada vértice do objeto (posição, cor, textura, normais etc.)
   * **Função principal:** processar e transformar os vértices do espaço local para o espaço de tela (projeção, modelagem, view).
   * Pode gerar atributos para o próximo estágio, como coordenadas de textura ou cores interpoladas.

2. **Fragment Shader (Shader de Fragmento / Pixel Shader)**

   * É o estágio que define a cor final de cada pixel que vai para a tela.
   * **Entrada:** interpolação de atributos gerados pelo vertex shader (cor, textura, normais etc.)
   * **Função principal:** calcular a cor final, aplicar texturas, iluminação, efeitos de transparência ou pós-processamento.

---

### 2. O que são primitivas gráficas? Como fazemos o armazenamento dos vértices na OpenGL? 


Primitivas gráficas são os **elementos básicos que formam qualquer objeto renderizado na GPU**.
Elas são usadas para construir formas mais complexas a partir de vértices.

Alguns exemplos comuns no OpenGL:

| Primitiva                                                             | Descrição                                                         |
| --------------------------------------------------------------------- | ----------------------------------------------------------------- |
| **Ponto (GL\_POINTS)**                                                | Um único vértice desenhado como ponto.                            |
| **Linha (GL\_LINES, GL\_LINE\_STRIP, GL\_LINE\_LOOP)**                | Conecta dois ou mais vértices formando linhas.                    |
| **Triângulo (GL\_TRIANGLES, GL\_TRIANGLE\_STRIP, GL\_TRIANGLE\_FAN)** | Três vértices formam um triângulo. É a primitiva mais usada.      |
| **Polígono (GL\_QUADS, GL\_POLYGON)**                                 | Formado por 4 ou mais vértices (menos usado em versões modernas). |

As primitivas são **“blocos de construção”** da geometria 3D ou 2D que a GPU renderiza. No contexto de OpenGL e programação gráfica, as primitivas gráficas são os elementos mais básicos que a GPU consegue desenhar. Tudo que é aparece na tela — modelos 3D, sprites, formas 2D — é construído a partir dessas primitivas.

No OpenGL moderno, os vértices não ficam “soltos” na CPU, eles são armazenados na GPU usando **buffers de vértices**. Pois os vértices definem a geometria dos objetos 2D ou 3D. Para a GPU renderizar, eles precisam estar na memória da GPU, não apenas na CPU. Isso permite que a GPU acesse os dados rapidamente durante o pipeline gráfico.

O VBO (***Vertex Buffer Object***) é a forma principal de armazenar vértices e o VAO (***Vertex Array Object***) armazena a configuração de como os vértices devem ser interpretados pelo Vertex Shader (o VAO é como se fosse um manual de instruções para o Vertex Shader).

---

### 3. Explique o que é VBO, VAO e EBO, e como se relacionam (se achar mais fácil, pode fazer um gráfico representando a relação entre eles).  

Abaixo vou exemplificar (construindo um triângulo) como se relacionam cada uma dessas etapas.

1. **Preparar a janela e o contexto gráfico**

   * Criar uma janela para desenhar (usando GLFW, SDL ou outra biblioteca).
   * Inicializar o OpenGL para poder desenhar nela.
   * **Resumo:** sem janela e contexto, a GPU não sabe onde desenhar.

2. **Definir os vértices do triângulo**

   * Escolher 3 pontos no espaço 2D ou 3D que formam o triângulo.
   * Cada vértice tem coordenadas (x, y, z).
   * **Resumo:** o triângulo é definido apenas pelos seus 3 vértices.

3. **Armazenar os vértices na GPU**

   * **VBO (Vertex Buffer Object):**

     * Um buffer que guarda os vértices na memória da GPU.
     * Permite que a GPU acesse os vértices rapidamente durante o desenho.
   * **EBO (Element Buffer Object) – opcional:**

     * Armazena **índices de vértices**, permitindo reutilizar os mesmos vértices para múltiplas primitivas (útil em objetos complexos).
     * Ex.: se um quadrado é formado por 2 triângulos, você pode usar apenas 4 vértices e um EBO para indicar a ordem de desenho.
   * **Resumo:** VBO guarda os vértices; EBO guarda índices para desenhar múltiplas primitivas sem repetir dados.

4. **Configurar como os vértices serão lidos pelo shader (VAO)**

   * **VAO (Vertex Array Object):**

     * Armazena a configuração de **atributos dos vértices**, como posição, cor e coordenadas de textura.
     * Define como o Vertex Shader deve interpretar os dados do VBO (e EBO, se houver).

5. **Escrever os shaders (GLSL)**

   * **Vertex Shader:** pega cada vértice e calcula sua posição na tela.
   * **Fragment Shader:** define a cor de cada pixel do triângulo.
   * **Resumo:** shaders dizem à GPU **como transformar e colorir os vértices**.

6. **Desenhar o triângulo**

   * Ligar o VAO e chamar a função de desenho (`glDrawArrays` ou `glDrawElements` se usar EBO).
   * A GPU processa os vértices pelo Vertex Shader, depois pelo Fragment Shader, e finalmente aparece na tela.
   * **Resumo:** é aqui que tudo que você configurou realmente aparece na janela.

7. **Mostrar na tela e atualizar**

   * Trocar o buffer da janela (double buffering) para mostrar o triângulo.
   * Manter o loop de desenho enquanto a janela estiver aberta.
   * **Resumo:** sem esse passo, você não vê nada ou só um quadro congelado.

---

💡 **Resumo visual da relação entre VBO, VAO e EBO:**

```
Vertices (CPU) 
    │
    ▼
VBO (guarda os vértices na GPU)
    │
    ├─> VAO (define como o Vertex Shader lê os vértices)
    │
    └─> EBO (opcional, define a ordem de desenho dos vértices)
    │
    ▼
Vertex Shader → Fragment Shader → Tela
```

---

### 4. Analise o código fonte do projeto Hello Triangle. Localize e relacione os conceitos de shaders, VBOs e VAO apresentados até então. Não precisa entregar nada neste exercício.  

Shaders: São pequenos programas que rodam na GPU:

* **Vertex Shader**: `vertexShaderSource`

  ```glsl
  // Código fonte do Vertex Shader (em GLSL): ainda hardcoded

  #version 400
  layout (location = 0) in vec3 position;
  void main()
  {
      gl_Position = vec4(position.x, position.y, position.z, 1.0);
  }
  ```

  * **Função:** transforma os vértices da geometria (coordenadas x, y, z) para o espaço de tela.
  * Recebe os atributos do vértice definidos no VAO/VBO.

* **Fragment Shader**: `fragmentShaderSource`

  ```glsl
  // Código fonte do Fragment Shader (em GLSL): ainda hardcoded

  #version 400
  uniform vec4 inputColor;
  out vec4 color;
  void main()
  {
      color = inputColor;
  }
  ```

  * **Função:** define a cor de cada pixel do triângulo.
  * Recebe a cor via uniform (`inputColor`) e aplica no fragmento.

* **Relacionamento no código:**

  * Os shaders são compilados e ligados em `setupShader()`, retornando `shaderID`.
  * Esse `shaderID` é usado no loop principal para renderizar o triângulo.

---

Vertex Buffer Object (VBO): Buffer na GPU que guarda os dados dos vértices.
* **No código:** dentro de `setupGeometry()`

  ```cpp
    int setupGeometry()
    {
	/*Aqui setamos as coordenadas x, y e z do triângulo e as armazenamos de forma sequencial, já visando mandar para o VBO (Vertex Buffer Objects)
	 Cada atributo do vértice (coordenada, cores, coordenadas de textura, normal, etc)
	 Pode ser armazenado em um VBO único ou em VBOs separados
    */
	GLfloat vertices[] = {
		// x   y     z
		// T0
		-0.5, -0.5, 0.0, // v0
		0.5, -0.5, 0.0,	 // v1
		0.0, 0.5, 0.0,	 // v2
	};

	GLuint VBO, VAO;
	// Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	// Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  ```

  * Os vértices do triângulo são enviados para a GPU.
  * `GL_STATIC_DRAW` indica que os dados **não mudam frequentemente**.

Vertex Array Object (VAO): Guarda a configuração de como os vértices estão organizados no VBO, incluindo quais atributos o shader deve ler.
* **No código:**

  ```cpp
	GLuint VBO, VAO;

    // Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértice e os ponteiros para os atributos
	glBindVertexArray(VAO);
	/* Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando:
	    Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	    Numero de valores que o atributo tem (por ex, 3 coordenadas xyz)
	    Tipo do dado
	    Se está normalizado (entre zero e um)
	    Tamanho em bytes
	    Deslocamento a partir do byte zero
    */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);
  ```

  * `glVertexAttribPointer(0, 3, GL_FLOAT, ...)` indica que o **atributo 0** do shader (`layout(location = 0)`) terá **3 floats** por vértice (x, y, z).
  * O VAO vincula o VBO e armazena essa configuração.
* **Função:** quando o VAO é ligado (`glBindVertexArray(VAO)`), a GPU sabe **como ler os vértices e passá-los ao Vertex Shader**.

---

### 5. Faça o desenho de 2 triângulos na tela. Desenhe-os:

Os desenhos abaixo se encontram na pasta:

```bash
src/Exercicios/TarefasGrauA/Lista1
```

a. Apenas com o polígono preenchido:
[PoligonoPreenchido.cpp](src/Exercicios/TarefasGrauA/Lista1/PoligonoPreenchido.cpp)

b. Apenas com contorno:
[ApenasComContorno.cpp](src/Exercicios/TarefasGrauA/Lista1/ApenasComContorno.cpp)

c. Apenas como pontos:
[ApenasComoPontos.cpp](src/Exercicios/TarefasGrauA/Lista1/ApenasComPontos.cpp)

d. Com as 3 formas de desenho juntas:
[TresFormasJuntas.cpp](src/Exercicios/TarefasGrauA/Lista1/TresFormasJuntas.cpp)

---

### 6. Faça o desenho de um círculo na tela, utilizando a equação paramétrica do círculo para gerar os vértices. Depois disso: 

Os desenhos abaixo se encontram na pasta:

```bash
src/Exercicios/TarefasGrauA/Lista1
```

a. Desenhe um octágono:
[Octagono.cpp](src/Exercicios/TarefasGrauA/Lista1/Octagono.cpp)

b. Desenhe um pentágono
[Pentagono.cpp](src/Exercicios/TarefasGrauA/Lista1/Pentagono.cpp)

c. Desenhe um pac-man! 
[PacMan.cpp](src/Exercicios/TarefasGrauA/Lista1/PacMan.cpp)

d. Desenhe uma fatia de pizza 
[FatiaPizza.cpp](src/Exercicios/TarefasGrauA/Lista1/FatiaPizza.cpp)

e. DESAFIO: desenhe uma “estrela” 
[Estrela.cpp](src/Exercicios/TarefasGrauA/Lista1/Estrela.cpp)

### 7. Desenhe uma espiral, assim: 

![Espiral](image.png)

O desenho abaixo se encontra na pasta:

```bash
src/Exercicios/TarefasGrauA/Lista1
```

### 8. Considerando o seguinte triângulo abaixo, formado pelos vértices P1, P2 e P3, respectivamente com as cores vermelho, verde e azul.  
a. Descreva uma possível configuração dos buffers (VBO, VAO e EBO) para representá-lo.  


  **Definição dos vértices (posição + cor):**
   Temos 3 vértices, cada um com **posição (x, y, z)** e **cor (r, g, b)**.
   Supondo coordenadas aproximadas com base no desenho:

   * P1 (0.0, 0.5, 0.0) → vermelho (1.0, 0.0, 0.0)
   * P2 (-0.5, -0.5, 0.0) → verde (0.0, 1.0, 0.0)
   * P3 (0.5, -0.5, 0.0) → azul (0.0, 0.0, 1.0)

   ```cpp
   float vertices[] = {
       // Posição          // Cor
        0.0f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // P1
       -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // P2
        0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // P3
   };
   ```

**Índices (EBO, opcional mas organizado):**

   ```cpp
   unsigned int indices[] = {
       0, 1, 2
   };
   ```

**Configuração de buffers (VAO, VBO e EBO):**

   ```cpp
   unsigned int VBO, VAO, EBO;

   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);
   glGenBuffers(1, &EBO);

   // Vincular VAO
   glBindVertexArray(VAO);

   // Preencher VBO
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   // Preencher EBO
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   // Atributo posição (location = 0)
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

   // Atributo cor (location = 1)
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);

   // Desvincular
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);
   ```

---

b. Como estes atributos seriam identificados no vertex shader? 

No **vertex shader**, cada vértice terá dois atributos: posição e cor.

```glsl
#version 330 core

layout (location = 0) in vec3 aPos;   // posição do vértice
layout (location = 1) in vec3 aColor; // cor do vértice

out vec3 ourColor; // vai passar para o fragment shader

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
}
```

E o **fragment shader** seria algo assim:

```glsl
#version 330 core

in vec3 ourColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(ourColor, 1.0);
}
```

---

Resumindo:

* **VBO** contém posições + cores.
* **EBO** define a ordem dos vértices (0,1,2).
* **VAO** armazena a configuração de como interpretar os atributos.

### 9. Faça um desenho em um papel quadriculado (pode ser no computador mesmo) e reproduza-o utilizando primitivas em OpenGL. Neste exercício você poderá criar mais de um VAO e fazer mais de uma chamada de desenho para poder utilizar primitivas diferentes, se necessário

O desenho abaixo se encontra na pasta:

```bash
src/Exercicios/TarefasGrauA/Lista1/DesenhoCustomizado.cpp
```

[DesenhoCustomizado.cpp](src/Exercicios/TarefasGrauA/Lista1/DesenhoCustomizado.cpp)