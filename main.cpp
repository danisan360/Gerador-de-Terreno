#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader_s.h"

#include <iostream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

float mixValue = 0.2f;

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
    //Queremos chamar essa função sempre q mudarem o tamanho da janela
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //Existem diversos tipos de callbacks functions

    //Usamos GLFW para linkar corretamente as funções do openGL
    glewExperimental = GL_TRUE;
    glewInit();

    Shader shaderProgram("3.3.shader.vs", "3.3.shader.fs");

    //Vertices de um triangulo
    float vertices[] = {
        //posições        //cores            //Cordenadas de textura
        0.5f,  0.5f,  0.0f,  1.0f, 0.0f, 0.0f,   2.0f, 2.0f,
        0.5f, -0.5f,  0.0f,  0.0f, 1.0f, 0.0f,   2.0f, 0.0f,
       -0.5f, -0.5f,  0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
       -0.5f,  0.5f,  0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 2.0f   
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    unsigned int VBO, VAO, EBO;//Vertex buffer objects can store a large number of vertices in the gpu
    //A vantagem disso é que podemos enviar grandes quantidades de dados de uma só vez para a placa gráfica
    //sem precisar enviar um vértice por vez
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); //Gera objetos buffer]
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);// bind vertex array object

    //Copia nosso array de vertices em um buffer para o opengl usar
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //Junta o buffer recem criado com GL_ARRAY_BUFFER
    //Copia os vertices definidos na memoria do buffer
    //Argumentos:O tipo de buffer que queremos copiar data para.Tamanho do data em bytes que queremos enviar
    //para o buffer.Actual data we want to send.Como queremos que a placa grafica gerencie
    //a data dada.
    //GL_STATIC_DRAW: the data will most likely not change at all or very rarely.
    //GL_DYNAMIC_DRAW: the data is likely to change a lot.
    //GL_STREAM_DRAW: the data will change every time it is drawn.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //atributo de posição
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //atributo de cor
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //atibuto das cordenadas de textura
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //Carrega e cria uma textura
    //--------------------------
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);//
    //seta os parametos de wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//seta o repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //seta os parametros de filtragem
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //Carrega a imagem
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("tassio.jpeg", &width, &height, &nrChannels, 0);    
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //Carrega e cria uma textura2
    //--------------------------
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);//
    //seta os parametos de wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//seta o repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //seta os parametros de filtragem
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //Carrega a imagem
    data = stbi_load("bolo.png", &width, &height, &nrChannels, 0); 
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//Wireframe 
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    shaderProgram.use();
    glUniform1i(glGetUniformLocation(shaderProgram.ID, "texture"), 0);
    shaderProgram.setInt("texture2", 1);

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

        //bind na textura
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        shaderProgram.setFloat("mixValue", mixValue);

        //ativa o shader
        shaderProgram.use();
        //renderiza o triangulo
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //Checa se qualquer evento é ativado(keyboard input etc), atualiza o estado da ja
        //nela e chama as funções correspondentes(que podemos setar por metodos callback)
        glfwSwapBuffers(window);
        //Vai trocar o buffer de cores
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
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
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        mixValue += 0.01f;
        if(mixValue >= 1.0f)
            mixValue = 1.0f;
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        mixValue -= 0.01f;
        if(mixValue <= 0.0f){
            mixValue = 0.0f;
        }
    }
}