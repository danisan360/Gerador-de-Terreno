#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#include "../glm-master/glm/glm.hpp"
#include "../glm-master/glm/gtc/matrix_transform.hpp"
#include "../glm-master/glm/gtc/type_ptr.hpp"

#include "../include/shader_s.h"
#include "../include/camera.h"

#include <iostream>
#include <cmath>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

float MapInRange(float x, float in_min, float in_max, float out_min, float out_max){
    if(x < in_min) x = in_min;
    if(x > in_max) x = in_max;
    return (float)((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}
//Camera
Camera camera(glm::vec3(0.0f, 2.0f, -12.0f));
bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0f / 2.0;
//timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
    GLFWwindow* window = glfwCreateWindow(800, 600, "Terreno", NULL, NULL);
    if(window == NULL){
        std::cout << "Falhou em criar a Janela GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    //Fazemos o GLFW tornar o "context" da nossa janela o "context" principal da nossa thread atual
    glfwMakeContextCurrent(window);
    //Existem diversos tipos de callbacks functions
    //Queremos chamar essa função sempre q mudarem o tamanho da janela
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //Usamos GLFW para linkar corretamente as funções do openGL
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST); //z buffer

    Shader shaderProgram("shader.vs", "shader.fs");

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
    unsigned char *data = stbi_load("../height_map/mapa_de_altura_1.jpeg", &width, &height, &nrChannels, 0);    
    if(data){
        //DEGUGAR
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
        glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
    }

    //Vertices de um triangulo
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
    int numVertX = width;
    int numVertZ = height;
    float curZ = +14.8f;
    //int count = 0;
    for (float i = 0; i < numVertZ; i+=1){
        float curX = -14.8f;
        for (float  j = 0; j < numVertX ; j+=1){
           
            vertices.push_back(glm::vec3(curX, 0.0f, curZ));
            //vertices1.push_back(curX);
            //vertices1.push_back(0.0f);
            //vertices1.push_back(curZ);
            
            
            curX += (float)29.8f/numVertX;
        }
        curZ -= (float)29.8f/numVertZ;

    }
    for(int i=0;i<(numVertX-1)*numVertZ;i++){
        if(vertices[i].z != vertices[(i) + 2].z) continue;
        indices.push_back(i);
        indices.push_back(i+1);
        indices.push_back(i+numVertX);
        indices.push_back(i+1);
        indices.push_back(i+1+numVertX);
        indices.push_back(i+numVertX);
    }
    

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * vertices.size() *sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    //stbi_image_free(data);

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
    //int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../textures/mapa_de_altura_1_textura(4097).png", &width, &height, &nrChannels, 0);
    if(data){
        //DEGUGAR
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
    glUniform1i(glGetUniformLocation(shaderProgram.ID, "texture1"), 0);
    shaderProgram.setInt("texture2", 1);

    //Render loop.Criamos um loop que continua rodando até que o GLFW pare
    //glfwWindowShouldClose checa no inicio de cada iteração se o GLFW foi mandado parar
    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //Olha se tem algum input do teclado.
        processInput(window);

        //Rendering commands aqui
        //Com glClearColor estámos setando uma cor para limpar a tela com
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //State-setting
        //Quando glClear for chamada e limpar o color buffer, ele será
        //"enchido" com a cor configurada por glClearcolor
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //State-using

        //bind na textura
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        //ativa o shader
        shaderProgram.use();

        //cria as transformações
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        view = camera.GetViewMatrix();
        view = glm::translate(view, glm::vec3(0.0f,-5.0f,0.0f));
        projection = glm::perspective(glm::radians(camera.Zoom), 800.0f/600.0f, 0.1f, 100.0f);

        shaderProgram.setMat4("projection", projection);
        shaderProgram.setMat4("view", view);


        glm::mat4 model = glm::mat4(1.0f);
        shaderProgram.setMat4("model", model);
        
        //renderiza o triangulo
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
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

//Criamos essa função para tratar todos os inputs do teclado
void processInput(GLFWwindow* window){
    //glfwGetKey recebe como entrada a window junto com uma key, e retorna
    //se a key está sendo pressionada
    //Nesse if checamos se a tecla esc está sendo presionada
    //Se n estiver a função retornara GLFW_RELEASE
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        //Se a tecla esc for precionada, fechamos a janela.
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}