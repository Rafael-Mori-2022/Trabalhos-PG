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

* A imagem de saída gerada para essa etapa apresenta objetos relacionados ao jogo Minecraft, desenvolvido pela Mojang, contendo assim uma árvore, um baú, uma enderpearl e uma fireball. Além disso, são realizados alguns movimentos na imagem sendo um deles na câmera e outro na mudança do posicionamento entre dois objetos, sendo eles a enderpearl e a fireball.

* Dessa forma, as imagens geradas no PP2 podem ser verificadas abaixo.

![Imagem padrão do cenário](https://github.com/Rafael-Mori-2022/Trabalhos-PG/blob/main/PP2/outputs/final_scene1.png)

![Imagem com movimentação de dois objetos](https://github.com/Rafael-Mori-2022/Trabalhos-PG/blob/main/PP2/outputs/final_scene2.png)

![Imagem com movimentação da câmera](https://github.com/Rafael-Mori-2022/Trabalhos-PG/blob/main/PP2/outputs/final_scene3.png)

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

🚀 | Desenvolvido por @guisantoxxx's, @mateusmatsuo, @PedroGCorreia e @Rafael-Mori-2022
