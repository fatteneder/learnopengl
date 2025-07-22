#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "std_image.h"

#include <iostream>
#include <cmath>

#include "common.hpp"
#include "shader.hpp"
#include "camera.hpp"

enum MaterialKind {
    Emerald,
    Jade,
    Pearl,
    Ruby,
    Gold,
    N_MateriaKind,
};

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow *window);
static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
static void scroll_callback(GLFWwindow *window, double xpos, double ypos);
static void setMaterial(MaterialKind kind, glm::vec3& ambientColor,
                        glm::vec3& diffuseColor, glm::vec3& specularColor,
                        float& shininess);

Camera camera;
float lastX = 400.0f, lastY = 300.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // load all OpenGL fptrs via glad
    if (!gladLoadGL())
    {
        std::cout << "failed to intialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);


    Shader lightingShader (STRING(SOURCE_DIR)"/lightingShader.vs", STRING(SOURCE_DIR)"/lightingShader.fs");
    Shader lightCubeShader (STRING(SOURCE_DIR)"/lightCubeShader.vs", STRING(SOURCE_DIR)"/lightCubeShader.fs");


    // set up vertices for a cube
    float vertices[] = {
        // positions          // normals          // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
    };

    unsigned int cubeVAO, VBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int lightingVAO;
    glGenVertexArrays(1, &lightingVAO);

    glBindVertexArray(lightingVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    auto startPos = glm::vec3(0.0f, 0.0f, 5.0f);
    camera = Camera(startPos);

    glm::vec3 lightPos(1.0f, 1.0f, 2.0f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        const float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render lighting
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.position);
        lightingShader.setVec3("light.position", lightPos);
        lightingShader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        lightingShader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f)); // darkened
        lightingShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), 800.0f/600.0f, 0.1f, 100.0f);
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection);
        glm::vec3 pos;
        for (int i = 0; i < MaterialKind::N_MateriaKind; i++) {
            pos = glm::vec3(2.0f*i, 0.0, 0.0);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pos);
            lightingShader.setMat4("model", model);
            glm::vec3 ambientColor, diffuseColor, specularColor;
            float shininess;
            setMaterial(MaterialKind(i), ambientColor, diffuseColor, specularColor, shininess);
            lightingShader.setVec3("material.ambient", ambientColor);
            lightingShader.setVec3("material.diffuse", diffuseColor);
            lightingShader.setVec3("material.specular", specularColor);
            lightingShader.setFloat("material.shininess", shininess);
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/8);
        }

        // render cube
        lightCubeShader.use();
        lightCubeShader.setMat4("view", view);
        lightCubeShader.setMat4("projection", projection);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightCubeShader.setMat4("model", model);

        glBindVertexArray(lightingVAO);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/8);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup
    glDeleteVertexArrays(1, &lightingVAO);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}


// callback to update gl's viewport when glfw's window changed
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void setMaterial(MaterialKind kind, glm::vec3& ambientColor,
                        glm::vec3& diffuseColor, glm::vec3& specularColor,
                        float& shininess)
{
    switch(kind) {
        case MaterialKind::Emerald:
            ambientColor = glm::vec3(0.0215f, 0.1745f, 0.0215f);
            diffuseColor = glm::vec3(0.07568f, 0.61424f, 0.07568);
            specularColor = glm::vec3(0.633f, 0.727811f, 0.633f);
            shininess = 0.6f;
            break;
        case MaterialKind::Jade:
            ambientColor = glm::vec3(0.135f, 0.2225f, 0.1575f);
            diffuseColor = glm::vec3(0.54f, 0.89f, 0.63f);
            specularColor = glm::vec3(0.316228f);
            shininess = 0.1f;
            break;
        case MaterialKind::Pearl:
            ambientColor = glm::vec3(0.25f, 0.20725f, 0.2072f);
            diffuseColor = glm::vec3(1.0f, 0.829f, 0.829f);
            specularColor = glm::vec3(0.296648f);
            shininess = 0.088f;
            break;
        case MaterialKind::Ruby:
            ambientColor = glm::vec3(0.1745, 0.01175, 0.01175f);
            diffuseColor = glm::vec3(0.61424f, 0.04136f, 0.04136f);
            specularColor = glm::vec3(0.727811f, 0.626959f, 0.626959f);
            shininess = 0.6f;
            break;
        case MaterialKind::Gold:
            ambientColor = glm::vec3(0.24725f, 0.1995f, 0.0745f);
            diffuseColor = glm::vec3(0.75164f, 0.60648f, 0.22648f);
            specularColor = glm::vec3(0.628281f, 0.555802f, 0.366065f);
            shininess = 0.4f;
            break;
        default: assert(true);
    };
}

// handle glfw keypress and -release events
static void processInput(GLFWwindow *window)
{
    const float cameraDist = 5.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::RIGHT, deltaTime);
}

// handle glfw mouse movement
static void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    camera.processMouseMovement(xoffset, yoffset);
}

// handle glfw mouse scroll
static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.processMouseScroll(yoffset);
}
