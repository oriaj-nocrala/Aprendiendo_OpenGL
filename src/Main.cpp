#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Renderer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

#define WIDTH 960
#define HEIGHT 540

int main(void)
{
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  glfwSwapInterval(1);

  /* Initialize GLEW */
  if(!glewInit() == GLEW_OK)
    return -1;

  
  std::cout << glGetString(GL_VERSION) << std::endl;

  float positions[] = {
    0.0f, 0.0f, 0.0f, 0.0f,
    580.0f, 0.0f, 1.0f, 0.0f,
    580.0f, 330.0f, 1.0f, 1.0f,
    0.0f, 330.0f, 0.0f, 1.0f
  };

  GLCall(glEnable(GL_BLEND));
  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };
  
  VertexArray va;
  VertexBuffer vb(positions, 4 * 4 * sizeof(float));
  VertexBufferLayout layout;

  layout.Push<float>(2);
  layout.Push<float>(2);
  va.AddBuffer(vb, layout);

  IndexBuffer ib(indices, 6);

  glm::mat4 proj = glm::ortho(0.0f, float(WIDTH), 0.0f, float(HEIGHT), -1.0f, 1.0f);
  glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

  Shader shader("res/shaders/Basic.shader");
  shader.Bind();
  
  shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

  Texture texture("res/textures/texture.png");
  texture.Bind();
  shader.SetUniform1i("u_Texture", 0);

  shader.Unbind();
  vb.Unbind();
  ib.Unbind();

  Renderer renderer;

  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 460");

  // bool show_demo_window = true;
  // bool show_another_window = false;
  // ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  glm::vec3 translation(200, 100, 0);

  float r = 0.0f;
  float increment = 0.03f;
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {

    /* Render here */
    renderer.Clear();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
    glm::mat4 mvp = proj * view * model;


    shader.Bind();
    shader.SetUniform4f("u_Color", 1.0f, 1.0f, r, 1.0f);
    shader.SetUniformMat4f("u_MVP", mvp);

    renderer.Draw(va, ib, shader);

    if(r > 1.0f)
        increment = -0.03f;
    else if(r < 0.0f)
        increment = 0.03f;

    r += increment;

    {
      ImGui::Begin("Configuración");
      ImGui::SliderFloat("Translación X", &translation.x, 0.0f, float(WIDTH) - 580.0f);
      ImGui::SliderFloat("Translación Y", &translation.y, 0.0f, float(HEIGHT) - 330.0f);
      ImGui::Text("Tiempo por imágen %.3f ms/imágen (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();
  return 0;
}