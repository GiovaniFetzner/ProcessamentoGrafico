# 🧩 Exercícios com OpenGL 3.3+

Estes exercícios têm como objetivo **praticar a criação e manipulação de geometria básica** com OpenGL moderna.
Você pode utilizar como base o código do repositório de exemplos fornecido.

---

## 🟢 PARTE 1 — Sem matriz de transformação

### **Exercício 1**

Crie uma função:

```cpp
GLuint createTriangle(float x0, float y0, float x1, float y1, float x2, float y2);
```

Essa função deve **criar um triângulo** com as coordenadas dos vértices `v0`, `v1` e `v2` passadas por parâmetro,
**retornando seu identificador de VAO**.

---

### **Exercício 2**

Instancie **5 triângulos** na tela utilizando a função criada anteriormente.

**Sugestão:**

* Utilize um `array` ou `std::vector` para armazenar os **VAOs** retornados pela função.
* Para desenhá-los, **itere sobre o array/vector** no loop de renderização.

---

## 🔵 PARTE 2 — Com matriz de transformação

A partir deste exercício, **sugere-se utilizar a biblioteca [GLM](https://github.com/g-truc/glm)** para cálculos matemáticos.

---

### **Exercício 3**

Crie uma **estrutura (struct ou classe)** chamada `Triangle` que armazene:

* A **posição do triângulo** (`x`, `y`)
* A **cor do triângulo** (componentes `R`, `G`, `B`)

Utilizando a função criada anteriormente, gere **um único VAO** para um triângulo padrão com os seguintes vértices:

```
v0(-0.1, -0.1)
v1(0.1, -0.1)
v2(0.0, 0.1)
```

Usando um `array`, `vector` ou `list` de estruturas `Triangle` e o **VAO criado**, o programa deverá:

* Criar **novos triângulos posicionados a partir do clique do mouse** na tela.
* A **cor de cada triângulo deve variar**, sorteando valores para as componentes RGB da cor.

---
## 🚀 Bom trabalho!