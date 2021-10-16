#include <iostream>

#include <glad/glad.h> // include glad before glfw
#include <GLFW/glfw3.h>
#include "stb_image/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
// function for when the window is resized, so the viewport is resized as well
{
    std::cout << "Window resized: " << width << " x " << height << std::endl;
    // the size of the rendering window
    glViewport(0, 0, width, height);
}

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);  // initial camera position
Camera camera(cameraPos);
double lastX = SCR_WIDTH / 2.0f;
double lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void processInput(GLFWwindow* window)
// function for processing input
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // key escape is pressed
    {
        std::cout << "ESC" << std::endl;
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime); 
        std::cout << "W - Camera speed: " << camera.getMovementSpeed() << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime); 
        std::cout << "S - Camera speed: " << camera.getMovementSpeed() << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime); 
        std::cout << "A - Camera speed: " << camera.getMovementSpeed() << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime); 
        std::cout << "D - Camera speed: " << camera.getMovementSpeed() << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);
        std::cout << "SPACE - Camera speed: " << camera.getMovementSpeed() << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, deltaTime);
        std::cout << "CTRL - Camera speed: " << camera.getMovementSpeed() << std::endl;
    }
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
    /* for debug only
    std::cout << "Camera front: (" 
        << camera.getFront().x << ", " 
        << camera.getFront().y << ", " 
        << camera.getFront().z << ")" << std::endl;
    */
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
    std::cout << "Camera fov: " << camera.getZoom() << std::endl;
}

// main -----------------------------------------------------------------------------------------------------------------------------------------------------------

int main()
{
    std::cout << "initializing... " << std::endl;
    glfwInit();
    // configure GLFW using glfwWindowHint()
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // create a window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // make window the current context

    // GLAD manages function pointers for OpenGL so initialize GLAD before calling any OpenGL function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell GLFW to call framebuffer_size_callback() function on every window resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    //Create shader program -------------------------------------------------------------------------------------------
    Shader objectShader("res/shaders/shader.vs", "res/shaders/shader.fs");
    Shader lightShader("res/shaders/lighting.vs", "res/shaders/lighting.fs");

    // Create object to draw ------------------------------------------------------------------------------------------
    float sot = glm::sqrt(1.0f/3.0f); // square root of one third
    // set of vertices
    float vertices[] = {
        // positions             // colors           // tex coords  // Normal vector
         0.5f,  0.5f, -0.5f,     1.0f, 1.0f, 1.0f,   1.0f, 1.0f,    sot,   sot, -sot,   // top right down
         0.5f, -0.5f, -0.5f,     1.0f, 1.0f, 1.0f,   1.0f, 0.0f,    sot,  -sot, -sot,   // bottom right down
        -0.5f, -0.5f, -0.5f,     1.0f, 1.0f, 1.0f,   0.0f, 0.0f,   -sot,  -sot, -sot,   // bottom left down
        -0.5f,  0.5f, -0.5f,     1.0f, 1.0f, 1.0f,   0.0f, 1.0f,   -sot,   sot, -sot,   // top left down

         0.5f,  0.5f,  0.5f,     1.0f, 1.0f, 1.0f,   0.0f, 1.0f,    sot,   sot,  sot,   // top right up
         0.5f, -0.5f,  0.5f,     1.0f, 1.0f, 1.0f,   0.0f, 0.0f,    sot,  -sot,  sot,   // bottom right up
        -0.5f, -0.5f,  0.5f,     1.0f, 1.0f, 1.0f,   1.0f, 0.0f,   -sot,  -sot,  sot,   // bottom left up
        -0.5f,  0.5f,  0.5f,     1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   -sot,   sot,  sot    // top left up
    };

    // set of indices
    unsigned int indices[] = {  // note that we start from 0
        0, 1, 3,    1, 2, 3, 
        4, 5, 7,    5, 6, 7,
        0, 1, 4,    1, 4, 5,
        2, 3, 6,    3, 6, 7,
        0, 3, 4,    3, 4, 7,
        1, 2, 5,    2, 5, 6
    };

    // generate a Vertex Array Object, Vertex Buffer Object, and Element Buffer Object
    unsigned int VAO, VBO, EBO; // create an (unsigned int) ID for the VAO, VBO, EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);
    // 2. copy our vertices array in a vertex buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 4. then set the vertex attributes pointers:
    unsigned int vertexDataSize = 11 * sizeof(float);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexDataSize, (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexDataSize, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // tex-coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexDataSize, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // nornal vec attribute
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, vertexDataSize, (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    // Light source --------------------------------------------------------------------------------------------------------
    
    // for the light source object
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // we only need to bind to the VBO, the container's VBO's data already contains the data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // set the vertex attribute 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexDataSize, (void*)0);
    glEnableVertexAttribArray(0);

    // Texture --------------------------------------------------------------------------------------------------------
    unsigned int texture1, texture2; // ID for texture
    // texture 1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image for texture 1
    int texWidth, texHeight, texChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* texData = stbi_load("res/textures/noHair.png", &texWidth, &texHeight, &texChannels, 0);
    if (texData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData); // generate texture
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    // free texture after used
    stbi_image_free(texData);

    // texture 2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image for texture 2
    // stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    texData = stbi_load("res/textures/pop_cat.png", &texWidth, &texHeight, &texChannels, 0);
    if (texData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData); // generate texture
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    // free texture after used
    stbi_image_free(texData);

    // -------------------------------------------------------------------------------------------
    objectShader.use(); // don't forget to activate/use the shader before setting uniforms!

    objectShader.setInt("texture1", 0);
    objectShader.setInt("texture2", 1);

    // This is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object
    // so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe mode

    // ----------------------------------------------------------------------------------------------------------------------

    glm::vec3 cubePositions[] = {
        // different positions to render object at
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    glm::vec3 lightPos(-4.2f, 3.0f, -2.0f);

    glm::mat4 model = glm::mat4(1.0f);  // model to world, make sure to initialize matrix to identity matrix first
    glm::mat4 view;                     // world to camera
    glm::mat4 projection;               // camera to screen

    // render loop, until GLFW is told to stop ------------------------------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);   // processing input

        // Rendering ------------------------------------------
        // clear the colorbuffer
        glClearColor(0.05f, 0.04f, 0.025f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Draw the Triangle    
        glActiveTexture(GL_TEXTURE0);   // bind textures on corresponding texture units
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        objectShader.use();
        // note that we're translating the scene in the reverse direction of where we want to move
        view = camera.GetViewMatrix();
        objectShader.setMat4f("view", view);
        projection = glm::perspective(glm::radians(camera.getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        objectShader.setMat4f("projection", projection);

        objectShader.setVec3f("lightColor", glm::vec3(1.0f, 0.8f, 0.5f));
        glm::vec3 lightRotateAxis = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::mat4 lightRotate = glm::rotate(model, (float)glfwGetTime(), lightRotateAxis);
        glm::vec4 lightRotate2 = glm::vec4(lightRotate * glm::vec4(lightPos, 1.0f));
        objectShader.setVec3f("lightPos", glm::vec3(lightRotate2.x, lightRotate2.y, lightRotate2.z));
        objectShader.setVec3f("viewPos", camera.getPosition());

        glBindVertexArray(VAO);
        // create transformations
        for (unsigned int i = 0; i < sizeof(cubePositions)/sizeof(cubePositions[0]); i++)
        {   
            glm::mat4 model_object = glm::translate(model, cubePositions[i]);
            float rotateAngle = (float)glfwGetTime() * glm::radians(10.0f * i);
            glm::vec3 rotateAxis = glm::vec3(1.0f, 0.3f, 0.5f * i);
            glm::mat4 model_object2 = glm::rotate(model_object, rotateAngle, rotateAxis);
            objectShader.setMat4f("model", model_object2);
            
            glDrawElements(GL_TRIANGLES, sizeof(indices)/ sizeof(indices[0]), GL_UNSIGNED_INT, 0);    // Draw all elements in indices

        }
        
        lightShader.use();
        lightShader.setMat4f("view", view);
        lightShader.setMat4f("projection", projection);
        glm::mat4 model_light = glm::rotate(model, (float)glfwGetTime(), lightRotateAxis);
        model_light = glm::translate(model_light, lightPos);
        model_light = glm::scale(model_light, glm::vec3(0.5f));
        lightShader.setMat4f("model", model_light);
        
        glBindVertexArray(lightVAO);
        
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);    // Draw all elements in indices

        // glBindVertexArray(0); // no need to unbind it every time 

        // check and call events and swap the buffers ------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clean up -----------------------------------------------------------------------------------------
    std::cout << "Closing..." << std::endl;
    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}