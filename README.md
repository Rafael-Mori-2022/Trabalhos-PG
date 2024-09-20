# Trabalho de Processamento Gráfico - Ray Tracing e WebGL
Esse repositório contém a implementação do PP2 relacionado a Ray Tracing e o trabalho em substituição à prova referente a WebGL.

## PP2 - Ray Tracing
O trabalho consiste em utilizar o Ray Tracing para visualização de três imagens:
1. Imagem padrão
2. Imagem com movimentação de objetos
3. Imagem com movimentação de câmera

### Detalhes sobre a implementação e execução
* As imagens são geradas por meio da execução no terminal do arquivo `main.cc` por meio dos seguintes comandos em sequência:
  * Para compilar o arquivo:

   ```bash
    g++ main.cc -o raytracer
    ```
  * Para rodar o executável gerado no passo anterior e gerar como saída a imagem no arquivo `final_scene.ppm`:
  
  ```bash
    ./raytracer > final_scene.ppm
    ```
  * Transforma a imagem em PNG utilizando o ImageMagick <strong>(instalação descrita abaixo!)</strong>
  ```bash
    convert final_scene.ppm final_scene.png
    ```
    

### Como instalar o ImageMagick

#### Windows
1. Acesse o site oficial do [ImageMagick](https://imagemagick.org/script/download.php#windows).
2. Baixe o instalador adequado para sua versão do Windows.
3. Execute o instalador e siga as instruções para finalizar a instalação.
4. Após a instalação, adicione o caminho do ImageMagick à variável de ambiente `Path`, caso não tenha sido feito automaticamente.

#### Linux
No terminal, use os seguintes comandos (dependendo da distribuição):

- Ubuntu/Debian:
    ```bash
    sudo apt update
    sudo apt install imagemagick
    ```

#### macOS
1. Primeiro, instale o [Homebrew](https://brew.sh/), se ainda não tiver.
2. Depois, execute o seguinte comando no terminal:
   
    ```bash
    brew install imagemagick
    ```

Agora, você poderá utilizar o comando `convert` para converter arquivos `.ppm` para `.png` ou outros formatos.

# Documentação das Cenas

  As imagens de saída geradas para essa etapa apresenta objetos relacionados ao jogo Minecraft, desenvolvido pela Mojang, contendo assim uma árvore, um baú, uma enderpearl e uma fireball. Além disso, são realizados alguns movimentos na imagem sendo um deles na câmera e outro na mudança do posicionamento entre dois objetos, sendo eles a enderpearl e a fireball.

## Definição de WCS e Posicionamento dos Objetos

### Sistema de Coordenadas Mundiais (WCS)

O **World Coordinate System (WCS)** para este projeto é descrito pelas coordenadas fornecidas em relação a uma origem global `(0, 0, 0)` com três eixos principais:
- **Eixo X**: Define a largura (esquerda-direita).
- **Eixo Y**: Define a altura (baixo-cima).
- **Eixo Z**: Define a profundidade (frente-trás).

A posição e o tamanho de cada objeto na cena são especificados usando esse sistema de coordenadas. As câmeras e os objetos no espaço compartilham o mesmo referencial, permitindo a criação de cenas coerentes e realistas.

### Posicionamento dos Objetos nas Cenas

#### Cena 1 - Cena padrão

Na **Cena 1**, uma série de cubos e objetos são posicionados dentro de uma formação 3x3. Os blocos de terra, esferas, e outros objetos são definidos usando o sistema de coordenadas mundiais. 

1. **Blocos de Terra (Formação 3x3)**:
   - Cada bloco de terra é posicionado usando um loop aninhado, calculando as posições `(x, y, z)` com base no índice da linha (`row`) e da coluna (`col`).
   - O tamanho do cubo é definido pela variável `cubo_size`, e há um pequeno espaçamento entre os cubos (`cubo_spacing`).

2. **Enderpearl**:
   - Localizada no canto superior esquerdo da formação de cubos, a posição da esfera é `(0.5, cubo_size + 0.5, 0.5)`. A altura é ajustada para que a esfera fique logo acima de um dos blocos de terra.

3. **Fireball**:
   - Localizada no canto inferior direito, com posição `(2.5, cubo_size + 0.5, 2.5)`, a esfera da Fireball também é posicionada logo acima de um bloco de terra, seguindo o mesmo princípio da Enderpearl.

4. **Baú**:
   - O baú é posicionado no canto superior direito da formação. As coordenadas usadas são calculadas para que ele se alinhe com os cubos já existentes: `(2 * (cubo_size + cubo_spacing), cubo_size, 0)`.

5. **Árvore**:
   - A árvore é posicionada no canto inferior esquerdo da formação, e os troncos e folhas são cuidadosamente centralizados em relação ao bloco de terra com a posição de base da árvore sendo `(0.5 * cubo_size, cubo_size, 2.5 * (cubo_size + cubo_spacing))`.

6. **Fonte de Luz**:
   - Uma esfera emissiva simulando uma fonte de luz é posicionada fora da formação de cubos, com coordenadas `(5, 5, -5)`, criando uma iluminação que incide da direita.

7. **Câmera**:
   - A câmera está posicionada em `(6, 6, 8)` e está orientada para o ponto central da formação de cubos em `(1.5, 1, 1.5)`, com uma orientação de cima para baixo definida por `vup = vec3(0, 1, 0)`.

![Imagem padrão do cenário](https://github.com/Rafael-Mori-2022/Trabalhos-PG/blob/main/PP2/outputs/final_scene1.png)

#### Cena 2 - Cena com Objetos Movimentados

Na **Cena 2**, objetos foram reposicionados, como a inversão das esferas (Enderpearl e Fireball).

1. **Enderpearl**:
   - Movida para o canto superior direito, com a nova posição `(2.5, cubo_size + 0.5, 2.5)`.

2. **Fireball**:
   - Agora posicionada no canto inferior esquerdo, com a nova posição `(0.5, cubo_size + 0.5, 0.5)`.

3. **Baú e Blocos de Terra**:
   - Mantêm-se nas mesmas posições da Cena 1.

![Imagem com movimentação de dois objetos](https://github.com/Rafael-Mori-2022/Trabalhos-PG/blob/main/PP2/outputs/final_scene2.png)

#### Cena 3 - Cena com Movimentação da Câmera

Na **Cena 3**, a maior alteração foi no posicionamento da câmera:

1. **Câmera**:
   - A câmera foi reposicionada e agora tem uma nova orientação para capturar a cena de outro ângulo. As coordenadas da câmera podem variar, mas o princípio de posicionamento segue o WCS, com `lookfrom` e `lookat` redefinidos para focar em diferentes partes da formação de cubos.

![Imagem com movimentação da câmera](https://github.com/Rafael-Mori-2022/Trabalhos-PG/blob/main/PP2/outputs/final_scene3.png)

### Conclusão

 Em suma, foi explorado ferramentas de manipulação dentro do **WCS** permitindo controlar com precisão o posicionamento de objetos em uma cena 3D. Nas cenas apresentadas, os objetos (cubos, esferas, e outros) são posicionados em relação aos eixos X, Y, e Z com base em loops e cálculos de espaçamento, o que garante alinhamento e organização. As câmeras são configuradas para capturar a cena a partir de diferentes pontos de vista, e as fontes de luz são posicionadas para criar uma iluminação adequada, respeitando o sistema de coordenadas global.

## Trabalho WebGL
Esse trabalho visa, por meio de uma implementação em WebGL, mostrar na tela do usuário 2 objetos 3D com texturas, sendo dois cubos com textura de madeira, em que tem-se movimentos independentes e uma iluminação variável de acordo com o movimento realizado pelo objeto.

### Detalhes sobre a implementação e a execução
* Para executar o trabalho WebGL, é necessário carregar o arquivo `main.html`, que é composto pelas importações das bibliotecas utilizadas e pelo canvas inicializando o WebGL, utilizando algum tipo de servidor local. Recomendamos a extensão **Live Server** do Visual Studio Code (VSCode). O Live Server permite iniciar um servidor local para visualizar páginas HTML dinamicamente.

### Como instalar e usar o Live Server no VSCode

1. Abra o **VSCode**.
2. Vá até a aba de extensões (ícone de quadrado à esquerda ou pressione `Ctrl+Shift+X`).
3. Procure por **Live Server** na barra de pesquisa.
4. Clique em **Instalar** na extensão desenvolvida por **Ritwick Dey**.
5. Após instalar, abra o arquivo `main.html` no VSCode.
6. Clique com o botão direito no editor e selecione **Open with Live Server** ou clique no ícone de **Go Live** no canto inferior direito do VSCode.
7. O navegador abrirá automaticamente com a visualização do seu arquivo HTML, refletindo qualquer alteração realizada.

* O arquivo `main.js` contém a lógica de renderização e animação dos objetos.
* Os arquivos `.obj` correspondem aos objetos que são mostrados na tela do usuário.
* Desse modo, o resultado desse trabalho pode ser visualizado por meio do GIF abaixo.

![Visualização de dois objetos 3D texturizados com movimentações independentes](https://github.com/Rafael-Mori-2022/Trabalhos-PG/blob/main/PROVA/output/cube.gif)

## Referências
Shirley, P. (2016). Ray Tracing in One Weekend. Disponível em: https://raytracing.github.io/books/RayTracingInOneWeekend.html

Shirley, P. (2016). Ray Tracing: The Next Week. Disponível em: https://raytracing.github.io/books/RayTracingTheNextWeek.html

GitHub. Ray Tracing in One Weekend Series Books (Repositório). Disponível em: https://github.com/RayTracing/raytracing.github.io

McGuire, G. (2020). WebGL 2 Fundamentals. Disponível em: https://webgl2fundamentals.org/

<hr>

🚀 | Desenvolvido por <a href="https://github.com/guisantoxxx">@guisantoxxx's</a>, <a href="https://github.com/mateusmatsuo">@mateusmatsuo</a>, <a href="https://github.com/PedroGCorreia">@PedroGCorreia</a> e <a href="https://github.com/Rafael-Mori-2022">@Rafael-Mori-2022</a>
