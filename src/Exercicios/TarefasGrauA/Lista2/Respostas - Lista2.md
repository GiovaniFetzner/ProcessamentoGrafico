## IMPORTANTE: Arquivo markdown, acesse corretamente para facilitar a leitura!

---

### 1. Modifique a janela do mundo (window/ortho) para os limites: `xmin=-10`, `xmax=10`, `ymin=-10`, `ymax=10`.

O desenho correspondente está em:

```
src/Exercicios/TarefasGrauA/Lista2/Exercicio1Circulo.cpp
```

[Abrir Exercicio1Circulo.cpp](./Exercicio1Circulo.cpp)

---

### 2. Agora modifique para: `xmin=0`, `xmax=800`, `ymin=600`, `ymax=0`.

O desenho correspondente está em:

```
src/Exercicios/TarefasGrauA/Lista2/Exercicio2Circulo.cpp
```

[Abrir Exercicio2Circulo.cpp](./Exercicio2Circulo.cpp)

---

### 3. Utilizando a câmera 2D do exercício anterior, desenhe algo na tela. O que acontece quando posicionamos os objetos? Por que é útil essa configuração?

**O que acontece ao posicionar objetos:**

- Os objetos aparecem exatamente nas posições especificadas, pois a projeção ortográfica mapeia as coordenadas do mundo diretamente para pixels da janela.
- Fica fácil alinhar, centralizar e posicionar elementos gráficos.

**Por que é útil essa configuração:**

- Permite desenhar interfaces, HUDs e gráficos 2D com precisão, já que as coordenadas correspondem diretamente à tela.
- Facilita o desenvolvimento de aplicações 2D, pois elimina a necessidade de conversão de coordenadas.

---

### 4. Modifique o viewport para desenhar a cena apenas no seguinte quadrante da janela da aplicação:

O desenho correspondente está em:

```
src/Exercicios/TarefasGrauA/Lista2/ViewportComQuadrante.cpp
```

[Abrir ViewportComQuadrante.cpp](./ViewportComQuadrante.cpp)

---

### 5. Agora, desenhe a mesma cena nos 4 quadrantes.

O desenho correspondente está em:

```
src/Exercicios/TarefasGrauA/Lista2/ViewportFull.cpp
```

[Abrir ViewportFull.cpp](./ViewportFull.cpp)

---

### 6. Crie triângulos a partir do clique do mouse. Para isso, seu programa deve atender os seguintes requisitos:

- Ao clicar na tela, você agora estará criando apenas **1 vértice**
- A cada **3 vértices criados**, você criará um **triângulo**
- Para cada novo triângulo criado, você deve usar uma **cor nova**
