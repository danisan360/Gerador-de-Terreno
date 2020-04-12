# Gerador de Terrenos
O programa recebe de entrada um mapa de altura e gera o terreno correspondente a ele, usando a tecnica de [displacement mapping](https://en.wikipedia.org/wiki/Displacement_mapping).Também é possivel receber uma textura adicional para melhorar a vizualização no terreno.
# Instalação Linux
Baixar o Source package do [GLFW](https://www.glfw.org/download.html), então extrair e ir para a pasta e executar os seguintes comandos.
1. ```sudo apt-get install cmake xorg-dev libglu1-mesa-dev```
2. ```cmake -G "Unix Makefiles"```
3. ```make```
4. ```sudo make install```

É necessario também ter na pasta a biblioteca [GLM](https://glm.g-truc.net/0.9.8/index.html)

Então basta executar o ```makefile.linux``` e será criado o executável "Terreno"
# Instalação Windows
É necessario usar o [MinGw](https://osdn.net/projects/mingw/releases/) para a compilação.Também são necessarios o [GLFW](https://www.glfw.org/download.html) e o [GLEW](http://glew.sourceforge.net/).O programa assume que ambos estão numa pasta ```C:\OpenGl```

É necessario também ter na pasta a biblioteca [GLM](https://glm.g-truc.net/0.9.8/index.html)

Então basta executar o ```makefile.win``` com o comando ```mingw32-make -f makefile.win``` e será criado o executável "Terreno.exe"
# Funcionamento do programa
Ao inciar o programa, use WASD para se movimentar e o mouse para mover a câmera.
![](https://user-images.githubusercontent.com/31495938/69889636-d4bee800-12d0-11ea-994e-85d7f1ed26c3.png)
![](https://user-images.githubusercontent.com/31495938/69889637-d5577e80-12d0-11ea-8500-6cb76b2ea8c5.png)
