"use strict";

// Função que transforma o objeto para dados que podem ser utilizados na aplicação
function parseOBJ(text) {

  // Arrays para armazenar posições de vértices, coordenadas de textura e normais de vértices
  const objPositions = [[0, 0, 0]];
  const objTexcoords = [[0, 0]];
  const objNormals = [[0, 0, 0]];
  const objVertexData = [objPositions, objTexcoords, objNormals];
  
  let webglVertexData = [[], [], []];

  // Função que adiciona os vértices ao array `webglVertexData`
  function addVertex(vert) {

    const ptn = vert.split('/');

    ptn.forEach((objIndexStr, i) => {
      if (!objIndexStr) return;
      const objIndex = parseInt(objIndexStr);
      const index = objIndex + (objIndex >= 0 ? 0 : objVertexData[i].length);
      webglVertexData[i].push(...objVertexData[i][index]);
    });

  }

  // Função para mapeamento de palavras-chave
  /* As keywords são as letras v, vn, vt, e f, que correspondem a elementos específicos do formato OBJ:
      - v (vertex) - Define os vértices de um objeto, ou seja, os pontos no espaço tridimensional (XYZ) que compõem a geometria.
      - vn (vertex normals) - Define as normais de cada vértice, que são vetores utilizados para calcular como a luz incide sobre a superfície, afetando a renderização.
      - vt (vertex texture coordinates) - Define as coordenadas de textura (UV) que são usadas para mapear imagens ou texturas sobre a superfície do objeto.
      - f (face) - Define uma face, que é uma série de vértices conectados que formam uma superfície, geralmente um triângulo ou um quadrilátero.
  */
  const keywords = {
    v(parts) { objPositions.push(parts.map(parseFloat)); },
    vn(parts) { objNormals.push(parts.map(parseFloat)); },
    vt(parts) { objTexcoords.push(parts.map(parseFloat)); },
    f(parts) {
      const numTriangles = parts.length - 2;
      for (let tri = 0; tri < numTriangles; ++tri) {
        addVertex(parts[0]);
        addVertex(parts[tri + 1]);
        addVertex(parts[tri + 2]);
      }
    },
  };

  // Divide o arquivo OBJ em linhas e processa cada linha
  const lines = text.split('\n');
  const keywordRE = /(\w*)(?: )*(.*)/;  // Regex para identificar a palavra-chave e seus argumentos.
  
  // Aplicação do tratamento para todas as linhas
  lines.forEach(line => {
    line = line.trim();
    if (line === '' || line.startsWith('#')) return;
    const [, keyword, unparsedArgs] = keywordRE.exec(line) || [];
    const parts = line.split(/\s+/).slice(1);
    const handler = keywords[keyword];
    if (handler) handler(parts, unparsedArgs);  // Chama a função correspondente, se existir
  });

  // Retorna os dados dos vértices no formato necessário para WebGL
  return {
    position: webglVertexData[0],
    texcoord: webglVertexData[1],
    normal: webglVertexData[2],
  };

}

// Função para carregamento das texturas
function loadTexture(gl, url) {
  return twgl.createTexture(gl, {
    src: url,
    crossOrigin: '',
  });
}

// Função que converte de graus para radianos
function degToRad(d) {
  return d * Math.PI / 180;
}

// Vertex Shader
const vertexShader = `#version 300 es
  in vec4 a_position;
  in vec3 a_normal;
  in vec2 a_texcoord;
  uniform mat4 u_projection;
  uniform mat4 u_view;
  uniform mat4 u_world;
  out vec3 v_normal;
  out vec2 v_texcoord;
  void main() {
    gl_Position = u_projection * u_view * u_world * a_position;
    v_normal = mat3(u_world) * a_normal;
    v_texcoord = a_texcoord;
}`;

// Fragment Shader
const fragmentShader = `#version 300 es
  precision highp float;
  in vec3 v_normal;
  in vec2 v_texcoord;
  uniform vec4 u_diffuse;
  uniform sampler2D u_texture;
  uniform vec3 u_lightDirection;
  out vec4 outColor;
  void main () {
    vec3 normal = normalize(v_normal);
    float light = dot(u_lightDirection, normal) * 0.5 + 0.5;

    // Rotaciona as coordenadas de textura em 90 graus no sentido horário
    vec2 TexCoord = vec2(v_texcoord.y, v_texcoord.x);

    vec4 texColor = texture(u_texture, TexCoord);

    vec3 finalColor = texColor.rgb * u_diffuse.rgb * light;

    outColor = vec4(finalColor, u_diffuse.a);
}`;

async function main() {

  // Inicialização e configuração do webGL
  const canvas = document.querySelector("#canvas");
  const gl = canvas.getContext("webgl2");
  if (!gl) return;
  twgl.setAttributePrefix("a_");

  // Cria um programa de shader para renderizar a malha
  const meshProgramInfo = twgl.createProgramInfo(gl, [vertexShader, fragmentShader]);

  // Pega o objeto cubo por meio de uma requisição da API
  const response = await fetch('https://webgl2fundamentals.org/webgl/resources/models/cube/cube.obj');
  const text = await response.text();
  const data = parseOBJ(text);

  // Cria informações de buffer para a malha, a partir dos dados processados
  const bufferInfo1 = twgl.createBufferInfoFromArrays(gl, data);
  const bufferInfo2 = twgl.createBufferInfoFromArrays(gl, data);

  // Cria Vertex Array Objects (VAOs) para armazenar o estado de renderização dos dois cubos
  const vao1 = twgl.createVAOFromBufferInfo(gl, meshProgramInfo, bufferInfo1);
  const vao2 = twgl.createVAOFromBufferInfo(gl, meshProgramInfo, bufferInfo2);

  // Definição da posição da câmera e demais configurações acerca da visualização
  const cameraPosition = [0, 0, 6];
  const cameraTarget = [0, 0, 0];
  const zNear = 0.1;
  const zFar = 50;

  // Realiza o carregamento da textura
  const blockTexture = loadTexture(gl, `./texture/wood.png`);

  // Função que renderiza a cena
  function render(time) {

    time *= 0.001;

    twgl.resizeCanvasToDisplaySize(gl.canvas);  // Ajusta o tamanho do canvas conforme a tela
    gl.viewport(0, 0, gl.canvas.width, gl.canvas.height);  // Define a área de renderização
    gl.enable(gl.DEPTH_TEST);  // Habilita o teste de profundidade (para sobreposição correta dos objetos)
    gl.enable(gl.CULL_FACE);  // Habilita a remoção de faces ocultas

    // Define a matriz de projeção para a câmera
    const aspect = gl.canvas.clientWidth / gl.canvas.clientHeight;
    const fieldOfViewRadians = degToRad(60);
    const projection = twgl.m4.perspective(fieldOfViewRadians, aspect, zNear, zFar);

    const up = [0, 1, 0];
    const camera = twgl.m4.lookAt(cameraPosition, cameraTarget, up);
    const view = twgl.m4.inverse(camera);  // Calcula a matriz de visualização (inversa da câmera)

    // Propriedades compartilhadas entre os dois cubos
    const sharedUniforms = {
      u_lightDirection: twgl.v3.normalize([-1, 3, 5]),
      u_view: view,
      u_projection: projection,
    };

    gl.useProgram(meshProgramInfo.program);
    twgl.setUniforms(meshProgramInfo, sharedUniforms);

    // Renderiza o primeiro cubo, que rotaciona no eixo Y.
    gl.bindVertexArray(vao1);

    const worldMatrix1 = twgl.m4.translation([-3, 0, 0]);  // Translada o cubo para a esquerda
    twgl.m4.rotateY(worldMatrix1, time, worldMatrix1);  // Rotaciona o cubo ao longo do eixo Y

    twgl.setUniforms(meshProgramInfo, {
      u_world: worldMatrix1,
      u_diffuse: [1, 0.7, 0.5, 1],
      u_diffuseMap: blockTexture,
    });

    twgl.drawBufferInfo(gl, bufferInfo1);

    // Renderiza o segundo cubo, que rotaciona no eixo X
    gl.bindVertexArray(vao2);

    const worldMatrix2 = twgl.m4.translation([3, 0, 0]);  // Translada o cubo para a direita
    twgl.m4.rotateX(worldMatrix2, time, worldMatrix2);  // Rotaciona o cubo ao longo do eixo X

    twgl.setUniforms(meshProgramInfo, {
      u_world: worldMatrix2,
    });

    twgl.drawBufferInfo(gl, bufferInfo2);

    requestAnimationFrame(render);  // Solicita a próxima renderização para animar a cena.

  }

  requestAnimationFrame(render);  // Inicia a animação
  
}

main();