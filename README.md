# Trabalho de processamento gráfico - raytracing e webgl
Esse repositório contém a implementação do PP2 relacionado a raytracing e o segundo trabalho referente a webgl.


## PP2 - Ray tracing
O trabalho consiste em utilizar o raytracing para visualização de duas imagens.
### Detalhes sobre a implementação e execução
* As imagens são geradas por meio da execução no terminal do arquivo main.cc por meio dos seuintes comandos em sequência:
  * <strong>g++ main.cpp -o raytracer</strong> - para compilar o arquivo;
  * <strong>./raytracer > final_scene.ppm</strong> - para rodar o executável gerado no passo anterior e gerar como saída a imagem no arquivo final_scene.ppm;
  * <strong>convert final_scene.ppm final_scene.png</strong> - transforma a imagem em PNG utilizando o ImageMagick (necessário instalação à parte);
* A imagem de saída gerada apresenta objetos relacionados ao jogo Minecraft, desenvolvido pela Mojang, contendo assim LISTAR OBJETOS. Além disso, são realizados alguns movimentos na imagem sendo um deles na câmera e outro no COLOCAR OBJETO.
* Dessa forma, as imagens geradas no PP2 podem ser verificadas abaixo.


## Trabalho WebGL
Esse trabalho visa, por meio de uma implementação em WebGL, mostrar na tela do usuário 2 objetos 3D com texturas, sendo um OBJETO1 e um OBJETO2, em que tem-se movimentos independentes e uma iluminação variável de acordo com o movimento realizado pelo objeto.
### Detalhes sobre a implementação e a execução
* Para executar o trabalho WebGL é necessário carregar o arquivo main.html, que é composto pelas importaçõs das bibliotecas utilizadas e pelo canvas que é utilizado para inicializar o WebGL, por meio de um servidor http;
* O arquivo main.js contém
* Os arquivos .obj correspondem aos objetos que são mostrados na tela do usuário;
* Desse modo, o resultado desse trabalho pode ser visualizado por meio do GIF abaixo.

🚀 | Desenvolvido por @guisantoxxx's, @mateusmatsuo, @PedroGCorreia e @Rafael-Mori-2022
