#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "std_image.h"

#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>

#include "common.hpp"
#include "shader.hpp"
#include "camera.hpp"

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow *window);
static void scroll_callback(GLFWwindow *window, double xpos, double ypos);
static void vertices_octahedron(unsigned int div_lvl, std::vector<float> &vertices);

Camera camera;
float lastX = 400.0f, lastY = 300.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

template <typename T>
size_t vectorsizeof(const typename std::vector<T>& vec)
{
    return sizeof(T) * vec.size();
}

int main(void)
{
    std::srand(12345);
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "Globe", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // load all OpenGL fptrs via glad
    if (!gladLoadGL())
    {
        std::cout << "failed to intialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);


    // set up octahedron
    Shader shader (STRING(SOURCE_DIR)"/shader.vs", STRING(SOURCE_DIR)"/shader.fs");

    std::vector<float> octaVertices(0);
    unsigned int div_lvl = 5;
    vertices_octahedron(div_lvl, octaVertices);

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vectorsizeof(octaVertices), octaVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    auto center = glm::vec3(0.0f, 0.0f, 0.0f);
    float radius = 3.0f;
    camera = Camera(center, radius);

    // wireframe mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        const float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), 800.0f/600.0f, 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0f);

        // draw octahedron
        shader.use();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, octaVertices.size()/3);

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
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// handle glfw keypress and -release events
static void processInput(GLFWwindow *window)
{
    const float cameraDist = 5.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::RIGHT, deltaTime);
}

// handle glfw mouse scroll
static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.processMouseScroll(yoffset);
}

static float unitRand() {
    return static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
}

static void push_back_vec3(std::vector<float>& vec, glm::vec3 &v)
{
    vec.push_back(v.x);
    vec.push_back(v.y);
    vec.push_back(v.z);
}

static void vertices_octahedron_side(unsigned div_lvl,
        // vertices of bounding cube
        glm::vec3& v1, glm::vec3& v2, glm::vec3& v3,
        std::vector<float> &vertices)
{
    auto ndivs = std::pow(2, div_lvl); // number of divisions along one side
    float L = std::sqrt(2); // side length triangle
    float dl = L/ndivs; // side length sub triangle
    // vectors connecting neighboring vertices
    auto d21 = dl * glm::normalize(v2-v1);
    auto d31 = dl * glm::normalize(v3-v1);
    auto d23 = dl * glm::normalize(v2-v3);
    for (unsigned int j = 0; j < ndivs; j++) {
        for (unsigned int i = 0; i < ndivs-j; i++) {
            auto rgb = glm::vec3(unitRand(), unitRand(), unitRand());
            auto w1 = v1 + (float)(i) * d21 + (float)(j) * d31;
            auto w2 = w1 + d21;
            auto w3 = w1 + d31;
            w1 = glm::normalize(w1);
            w2 = glm::normalize(w2);
            w3 = glm::normalize(w3);
            push_back_vec3(vertices, w1);
            push_back_vec3(vertices, rgb);
            push_back_vec3(vertices, w2);
            push_back_vec3(vertices, rgb);
            push_back_vec3(vertices, w3);
            push_back_vec3(vertices, rgb);
            if (j == 0) // first row of sub triangles don't need to be mirrored across d21
                continue;
            rgb = glm::vec3(unitRand(), unitRand(), unitRand());
            w1 = v1 + (float)(i) * d21 + (float)(j) * d31;
            w2 = w1 + d21;
            w3 = w1 + d23;
            w1 = glm::normalize(w1);
            w2 = glm::normalize(w2);
            w3 = glm::normalize(w3);
            push_back_vec3(vertices, w1);
            push_back_vec3(vertices, rgb);
            push_back_vec3(vertices, w2);
            push_back_vec3(vertices, rgb);
            push_back_vec3(vertices, w3);
            push_back_vec3(vertices, rgb);
        }
    }
}

// compute vertices for an unit octahedron
// TODO unique vertices and provide std::vector<int>& indices argument instead
static void vertices_octahedron(unsigned int div_lvl, std::vector<float> &vertices)
{
    glm::vec3 v1, v2, v3;

    int nt = std::pow(2, 2*div_lvl); // total number triangles
    int nv = 3; // number vertices per triangle
    int nc = 3; // number components per vertex
    int nd = 2; // vertex + color
    vertices.reserve(nt*nv*nc*nd);
    vertices.clear();

    v1 = glm::vec3(1.0f, 0.0f, 0.0f);
    v2 = glm::vec3(0.0f, 1.0f, 0.0f);
    v3 = glm::vec3(0.0f, 0.0f, 1.0f);
    vertices_octahedron_side(div_lvl, v1, v2, v3, vertices);

    v1 = glm::vec3(-1.0f, 0.0f, 0.0f);
    v2 = glm::vec3(0.0f, 1.0f, 0.0f);
    v3 = glm::vec3(0.0f, 0.0f, 1.0f);
    vertices_octahedron_side(div_lvl, v1, v2, v3, vertices);

    v1 = glm::vec3(-1.0f, 0.0f, 0.0f);
    v2 = glm::vec3(0.0f, -1.0f, 0.0f);
    v3 = glm::vec3(0.0f, 0.0f, 1.0f);
    vertices_octahedron_side(div_lvl, v1, v2, v3, vertices);

    v1 = glm::vec3(1.0f, 0.0f, 0.0f);
    v2 = glm::vec3(0.0f, -1.0f, 0.0f);
    v3 = glm::vec3(0.0f, 0.0f, 1.0f);
    vertices_octahedron_side(div_lvl, v1, v2, v3, vertices);

    v1 = glm::vec3(1.0f, 0.0f, 0.0f);
    v2 = glm::vec3(0.0f, 1.0f, 0.0f);
    v3 = glm::vec3(0.0f, 0.0f, -1.0f);
    vertices_octahedron_side(div_lvl, v1, v2, v3, vertices);

    v1 = glm::vec3(-1.0f, 0.0f, 0.0f);
    v2 = glm::vec3(0.0f, 1.0f, 0.0f);
    v3 = glm::vec3(0.0f, 0.0f, -1.0f);
    vertices_octahedron_side(div_lvl, v1, v2, v3, vertices);

    v1 = glm::vec3(-1.0f, 0.0f, 0.0f);
    v2 = glm::vec3(0.0f, -1.0f, 0.0f);
    v3 = glm::vec3(0.0f, 0.0f, -1.0f);
    vertices_octahedron_side(div_lvl, v1, v2, v3, vertices);

    v1 = glm::vec3(1.0f, 0.0f, 0.0f);
    v2 = glm::vec3(0.0f, -1.0f, 0.0f);
    v3 = glm::vec3(0.0f, 0.0f, -1.0f);
    vertices_octahedron_side(div_lvl, v1, v2, v3, vertices);
}
