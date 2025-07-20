#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

#include "common.hpp"
#include "shader.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

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

    // load all OpenGL fptrs via glad
    if (!gladLoadGL())
    {
        std::cout << "failed to intialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    Shader shader (STRING(SOURCE_DIR)"/shader.vs", STRING(SOURCE_DIR)"/shader.fs");

    // set vertex data
    float vertices[] = {
        // positions        // colors
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // top
    };

    // generate buffers
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // bind buffers and copy data over
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // set attributes
    glVertexAttribPointer(0 /* (layout = 0) in vertex shader */,
                          3, // size of vertex attribute
                          GL_FLOAT, // vertex data type, all vec* are GL_FLOAT
                          GL_FALSE, // normalize data?
                          6*sizeof(float), // stride of consecutive vertex attributes
                          (void*)0 // offset of where position data begins
                          );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1 /* (layout = 1) in vertex shader */,
                          3, // size of vertex attribute
                          GL_FLOAT, // vertex data type, all vec* are GL_FLOAT
                          GL_FALSE, // normalize data?
                          6*sizeof(float), // stride of consecutive vertex attributes
                          (void*)(3*sizeof(float)) // offset where position data begins
                          );
    glEnableVertexAttribArray(1);

    shader.use();

    // wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw something
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}


// callback to update gl's viewport when glfw's window changed
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// handle glfw keypress and -release events
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
