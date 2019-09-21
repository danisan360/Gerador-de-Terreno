#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const char *vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";

int main(){
	glfwInit(); //Inicializa o GLFW
	//Configura a janela
	//Argumento 1: Que opção queremos configurar
	//Argumento 2: Seta o valor da nossa opção
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
    	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
	#endif

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

	//Build e compila os programas shaders
	//------------------------------------
	//Vertex shader
	unsigned int vertexShader;
	//Criando um shader do typo GL_VERTEX_SHADER
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//Argumentos.Objeto shader a ser compilado.Quantas strings estamos passando.O codigo fonte
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader); //Compila :P
	//Checa erros de compilação
	int success; //indica sucesso
	char infoLog[512]; //Guarda mensagens de erro(se houverem).
	//Função checa se a compilação foi bem sucedida
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	//se a compilação n for bem sucedida
	if(!success){
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//------------------------------------
	//Fragment shader(calcula o output da cor de nossos pixels)
	unsigned int fragmentShader;
	//Criando um shader do typo GL_FRAGMENT_SHADER
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//Argumentos.Objeto shader a ser compilado.Quantas strings estamos passando.O codigo fonte
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);// Compila :P
	//Checa erros de compilação
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	//Se a compilação n for bem sucedida
	if(!success){
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//------------------------------------
	//Linkando os shaders
	unsigned int shaderProgram;
	//Cria um programa e retorna o ID de referencia do objeto criado
	shaderProgram = glCreateProgram();
	//Agr iremos anexar os shaders compilados ao objeto programa e linkalos
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//Checa por erros na linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;	
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//Vertices de um triangulo
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	unsigned int VBO, VAO;//Vertex buffer objects can store a large number of vertices in the gpu
	//A vantagem disso é que podemos enviar grandes quantidades de dados de uma só vez para a placa gráfica
	//sem precisar enviar um vértice por vez
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO); //Gera objetos buffer
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Junta o buffer recem criado com GL_ARRAY_BUFFER
	//Copia os vertices definidos na memoria do buffer
	//Argumentos:O tipo de buffer que queremos copiar data para.Tamanho do data em bytes que queremos enviar
	//para o buffer.Actual data we want to send.Como queremos que a placa grafica gerencie
	//a data dada.
    //GL_STATIC_DRAW: the data will most likely not change at all or very rarely.
    //GL_DYNAMIC_DRAW: the data is likely to change a lot.
    //GL_STREAM_DRAW: the data will change every time it is drawn.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Render loop.Criamos um loop que continua rodando até que o GLFW pare
	//glfwWindowShouldClose checa no inicio de cada iteração se o GLFW foi mandado parar
	while(!glfwWindowShouldClose(window)){
		//Olha se tem algum input do teclado.
		processInput(window);

		//Rendering commands aqui
		//Com glClearColor estámos setando uma cor para limpar a tela com
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //State-setting
		//Quando glClear for chamada e limpar o color buffer, ele será
		//"enchido" com a cor configurada por glClearcolor
		glClear(GL_COLOR_BUFFER_BIT); //State-using

		//Desenha o triangulo
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//Checa se qualquer evento é ativado(keyboard input etc), atualiza o estado da ja
		//nela e chama as funções correspondentes(que podemos setar por metodos callback)
		glfwSwapBuffers(window);
		//Vai trocar o buffer de cores
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
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

//Criamos essa função para tratar todos os inputs
void processInput(GLFWwindow* window){
	//glfwGetKey recebe como entrada a window junto com uma key, e retorna
	//se a key está sendo pressionada
	//Nesse if checamos se a tecla esc está sendo presionada
	//Se n estiver a função retornara GLFW_RELEASE
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		//Se a tecla esc for precionada, fechamos a janela.
		glfwSetWindowShouldClose(window, true);
	}
}