#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main(){
	glfwInit(); //Inicializa o GLFW
	//Configura a janela
	//Argumento 1: Que opção queremos configurar
	//Argumento 2: Seta o valor da nossa opção
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Criar a janela
	//Argumentos: width(Largura), height(altura), Nome da janela.Retorna um objeto GLFWwindow
	GLFWwindow* window = glfwCreateWindow(800, 600, "Daniel", NULL, NULL);
	if(window == NULL){
		std::cout << "Falhou em criar a Janela GLFW" << std::endl;
		glfwTerminate();
		return -1;
	}
	//Fazemos o GLFW tornar o "context" da nossa janela o "context" principal da nossa thread atual
	glfwMakeContextCurrent(window);
	//Queremos chamar essa função sempre a mudarem o tamanho da janela
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//Existem diversos tipos de callbacks functions

	//Usamos GLFW para linkar corretamente as funções do openGL
	glewExperimental = GL_TRUE;
	glewInit();

	//Render loop.Criamos um loop que continua rodando até que o GLFW pare
	//glfwWindowShouldClose checa no inicio de cada iteração se o GLFW foi mandado parar
	while(!glfwWindowShouldClose(window)){
		//Checa se qualquer evento é ativado(keyboard input etc), atualiza o estado da ja
		//nela e chama as funções correspondentes(que podemos setar por metodos callback)
		glfwSwapBuffers(window);
		//Vai trocar o buffer de cores
		glfwPollEvents();
	}

	//Depois de sair do loop deletamos todos os recursos alocados pelo GLFW.
	glfwTerminate();
	return 0;
}

//Essa função é chamada sempre que o usuario muda o tamanho da janela para ajustar o mapeamento
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	//Essa função transforma as cordenadas datas em cordenadas na tela.Ex: o ponto 
	//P(-0.5, 0.5) seria mapeado para (200, 450) em cordenadas da tela
	//Os primeiros 2 parametros setam a localização do canto inferior esquerdo da janela
	//Os paremetros 3 e 4 setam a largura e altura da janela de renderização em pixels
	glViewport(0, 0, width, height);
}