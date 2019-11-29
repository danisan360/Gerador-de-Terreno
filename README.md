# Gerador de Terrenos
O programa recebe de entrada um mapa de altura e gera um espaço com um plano agora deformado com a tecnica de [displacement mapping](https://en.wikipedia.org/wiki/Displacement_mapping).Também é possivel receber uma textura adicional para melhorar a vizualização no terreno.
# Instalação Linux
Baixar o Source package do GLFW [aqui](https://www.glfw.org/download.html), então extrair e ir para a pasta e executar os seguintes comandos.
#Markdown
1. ```sudo apt-get install cmake xorg-dev libglu1-mesa-dev```
2. ```cmake -G "Unix Makefiles"```
3. ```make```
4. ```sudo make install```

Também é necessario ter na pasta biblioteca GLM, que pode ser baixada [aqui](https://glm.g-truc.net/0.9.8/index.html)
# Funcionamento do programa
Ao inciar o programa, use WASD para se movimentar e o mouse para mover a câmera.
![](https://user-images.githubusercontent.com/31495938/69889636-d4bee800-12d0-11ea-994e-85d7f1ed26c3.png)
![](https://user-images.githubusercontent.com/31495938/69889637-d5577e80-12d0-11ea-8500-6cb76b2ea8c5.png)
