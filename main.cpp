#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "glm-master/glm/glm.hpp"
#include "glm-master/glm/gtc/matrix_transform.hpp"
#include "glm-master/glm/gtc/type_ptr.hpp"

#include "shader_s.h"

#include <iostream>
#include <cmath>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

float MapInRange(float x, float in_min, float in_max, float out_min, float out_max){
    if(x < in_min) x = in_min;
    if(x > in_max) x = in_max;
    return (float)((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}

float timeRot = 5.0f;

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
    std::vector<float> vertices1;
    std::vector<int> indices1;
    int numVert = 20;
    float curZ = -1.0f;
    int count = 0;
    for (float i = 0; i < numVert; i+=1){
        float curX = -1.0f;
        for (float  j = 0; j < numVert ; j+=1){
            std::cout<< count<< "\n";
            count++;
           // std::cout << "curX: "<<curX<<" curZ: "<<curZ<<std::endl;
            vertices1.push_back(curX);
            vertices1.push_back(0.0f);
            vertices1.push_back(curZ);
            vertices1.push_back(MapInRange(curX, -1.0f, 1.0f, 0, 1));
            vertices1.push_back(MapInRange(curZ, -1.0f, 1.0f, 0, 1));
            
            curX += (float)2/numVert;
            if(curX == -0.999999f) curX = -0.1f;
            //std::cout<< "curX agr é: "<<curX<<"\n";
        }
        curZ += (float)2/numVert;;

    }
    for(int i=0;i<(numVert)*numVert;i++){
        indices1.push_back(i);
        indices1.push_back(i+1);
        indices1.push_back(i+numVert);
        indices1.push_back(i+1);
        indices1.push_back(i+1+numVert);
        indices1.push_back(i+numVert);
    }
    /*for(int i=0;i<vertices1.size();i++){
        std::cout <<vertices1[i]<<" ";
        i++;
        std::cout <<vertices1[i]<<" ";
        i++;
        std::cout <<vertices1[i]<<" ";
        i++;
        std::cout <<vertices1[i]<<" ";
        i++;
        std::cout <<vertices1[i]<<std::endl;
    }*/
    //std::cout<<"a conta: "<<-0.2f + 0.1f<<"\n";
    float vertices[vertices1.size() ];
    unsigned int indices[indices1.size() ];
    for(int i=0;i<vertices1.size();i++){
        vertices[i] = vertices1[i];
        //std::cout << vertices[i]<<std::endl;
    }
    /*for(int i=0;i<vertices1.size();i++){
        std::cout << vertices[i]<<" ";
        i++;
        std::cout << vertices[i]<<" ";
        i++;
        std::cout << vertices[i]<<" ";
        i++;
        std::cout << vertices[i]<<" ";
        i++;
        std::cout << vertices[i]<<std::endl;
    }*/
    for(int i = 0;i<indices1.size();i++){
        indices[i] = indices1[i];
    }
    /*float vertices[] = {
        //posições        //cores            //Cordenadas de textura
        0.0f ,  0.0f,  1.0f ,   1.0f, 1.0f, 1.0f,  MapInRange(0.0, 0.0, 1.0, 0, 1) ,  MapInRange(1.0, 0.0, 1.0, 0, 1),
        0.25f,  0.0f,  1.0f ,   1.0f, 1.0f, 1.0f,  MapInRange(0.25, 0.0, 1.0, 0, 1),  MapInRange(1.0, 0.0, 1.0, 0, 1),
        0.5f ,  0.0f,  1.0f ,   1.0f, 1.0f, 1.0f,  MapInRange(0.5, 0.0, 1.0, 0, 1) ,  MapInRange(1.0, 0.0, 1.0, 0, 1),
        0.75f,  0.0f,  1.0f ,   1.0f, 1.0f, 1.0f,  MapInRange(0.75, 0.0, 1.0, 0, 1),  MapInRange(1.0, 0.0, 1.0, 0, 1),
        1.0f ,  0.0f,  1.0f ,   1.0f, 1.0f, 1.0f,  MapInRange(1.0, 0.0, 1.0, 0, 1) ,  MapInRange(1.0, 0.0, 1.0, 0, 1),
        0.0f ,  0.0f,  0.75f,   1.0f, 1.0f, 1.0f,  MapInRange(0.0, 0.0, 1.0, 0, 1) ,  MapInRange(0.75, 0.0, 1.0, 0, 1),
        0.25f,  0.0f,  0.75f,   1.0f, 1.0f, 1.0f,  MapInRange(0.25, 0.0, 1.0, 0, 1),  MapInRange(0.75, 0.0, 1.0, 0, 1),
        0.5f ,  0.0f,  0.75f,   1.0f, 1.0f, 1.0f,  MapInRange(0.5, 0.0, 1.0, 0, 1) ,  MapInRange(0.75, 0.0, 1.0, 0, 1),
        0.75f,  0.0f,  0.75f,   1.0f, 1.0f, 1.0f,  MapInRange(0.75, 0.0, 1.0, 0, 1),  MapInRange(0.75, 0.0, 1.0, 0, 1),
        1.0f ,  0.0f,  0.75f,   1.0f, 1.0f, 1.0f,  MapInRange(1.0, 0.0, 1.0, 0, 1) ,  MapInRange(0.75, 0.0, 1.0, 0, 1),
        0.0f ,  0.0f,  0.5f ,   1.0f, 1.0f, 1.0f,  MapInRange(0.0, 0.0, 1.0, 0, 1) ,  MapInRange(0.5, 0.0, 1.0, 0, 1),
        0.25f,  0.0f,  0.5f ,   1.0f, 1.0f, 1.0f,  MapInRange(0.25, 0.0, 1.0, 0, 1),  MapInRange(0.5, 0.0, 1.0, 0, 1),
        0.5f ,  0.0f,  0.5f ,   1.0f, 1.0f, 1.0f,  MapInRange(0.5, 0.0, 1.0, 0, 1) ,  MapInRange(0.5, 0.0, 1.0, 0, 1),
        0.75f,  0.0f,  0.5f ,   1.0f, 1.0f, 1.0f,  MapInRange(0.75, 0.0, 1.0, 0, 1),  MapInRange(0.5, 0.0, 1.0, 0, 1),
        1.0f ,  0.0f,  0.5f ,   1.0f, 1.0f, 1.0f,  MapInRange(1.0, 0.0, 1.0, 0, 1) ,  MapInRange(0.5, 0.0, 1.0, 0, 1),
        0.0f ,  0.0f,  0.25f,   1.0f, 1.0f, 1.0f,  MapInRange(0.0, 0.0, 1.0, 0, 1) ,  MapInRange(0.25f, 0.0, 1.0, 0, 1),
        0.25f,  0.0f,  0.25f,   1.0f, 1.0f, 1.0f,  MapInRange(0.25, 0.0, 1.0, 0, 1),  MapInRange(0.25f, 0.0, 1.0, 0, 1),
        0.5f ,  0.0f,  0.25f,   1.0f, 1.0f, 1.0f,  MapInRange(0.5, 0.0, 1.0, 0, 1) ,  MapInRange(0.25f, 0.0, 1.0, 0, 1),
        0.75f,  0.0f,  0.25f,   1.0f, 1.0f, 1.0f,  MapInRange(0.75, 0.0, 1.0, 0, 1),  MapInRange(0.25f, 0.0, 1.0, 0, 1),
        1.0f ,  0.0f,  0.25f,   1.0f, 1.0f, 1.0f,  MapInRange(1.0, 0.0, 1.0, 0, 1) ,  MapInRange(0.25f, 0.0, 1.0, 0, 1),
        0.0f ,  0.0f,  0.0f ,   1.0f, 1.0f, 1.0f,  MapInRange(0.0, 0.0, 1.0, 0, 1) ,  MapInRange(0.0, 0.0, 1.0, 0, 1),
        0.25f,  0.0f,  0.0f ,   1.0f, 1.0f, 1.0f,  MapInRange(0.25, 0.0, 1.0, 0, 1),  MapInRange(0.0, 0.0, 1.0, 0, 1),
        0.5f ,  0.0f,  0.0f ,   1.0f, 1.0f, 1.0f,  MapInRange(0.5, 0.0, 1.0, 0, 1) ,  MapInRange(0.0, 0.0, 1.0, 0, 1),
        0.75f,  0.0f,  0.0f ,   1.0f, 1.0f, 1.0f,  MapInRange(0.75, 0.0, 1.0, 0, 1),  MapInRange(0.0, 0.0, 1.0, 0, 1),
        1.0f ,  0.0f,  0.0f ,   1.0f, 1.0f, 1.0f,  MapInRange(1.0, 0.0, 1.0, 0, 1) ,  MapInRange(0.0, 0.0, 1.0, 0, 1),  
    };*/

    /*unsigned int indices[] = {
        0 , 5 , 6,
        0 , 6 , 1,
        1 , 6 , 7,
        1 , 7 , 2,
        2 , 7 , 8,
        2 , 8 , 3,
        3 , 8 , 9,
        3 , 9 , 4,
        5 , 10, 11,
        5 , 11, 6,
        6 , 11, 12,
        6 , 12, 7,
        7 , 12, 13,
        7 , 13, 8,
        8 , 13, 14,
        8 , 14, 9,
        10, 15, 16,
        10, 16, 11,
        11, 16, 17,
        11, 17, 12,
        12, 17, 18,
        12, 18, 13,
        13, 18, 19,
        13, 19, 14,
        15, 20, 21,
        15, 21, 16,
        16, 21, 22,
        16, 22, 17,
        17, 22, 23,
        17, 23, 18,
        18, 23, 24,
        18, 24, 19 


    };*/

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //atributo de cor
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);
    //atibuto das cordenadas de textura
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
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
    unsigned char *data = stbi_load("mapadealtura.jpeg", &width, &height, &nrChannels, 0);    
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

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//Wireframe 
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    shaderProgram.use();
    

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

        glm::ortho(-200.0f, 800.0f, 600.0f, 600.0f, 0.1f, 100.0f);

        //cria as transformações
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.2f));
        float save = (float)glfwGetTime();
        save = save/timeRot;
        transform = glm::rotate(transform, save, glm::vec3(-1.0f, 0.0f, 0.0f));

        //ativa o shader
        shaderProgram.use();
        unsigned int transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        //renderiza o triangulo
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 4000, GL_UNSIGNED_INT, 0);

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
        timeRot += 0.1f;
        //std::cout << timeRot << std::endl;
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        timeRot -= 0.1f;
        //std::cout << timeRot << std::endl;
    }
}
