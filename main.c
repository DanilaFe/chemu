#include <glfw/glfw3.h>
#include <stdio.h>
#include "chip8.h"

int main(int argc, char** argv){
  chipPrintInfo();
  if(argc == 1) {
    chipLog("No input file. Exiting.");
    return 0;
  }

  chipLog("Initializing GLFW");
  glfwInit();
  GLFWwindow* window = glfwCreateWindow(64 * 8, 32 * 8, "Chip-8", NULL, NULL);
  glfwMakeContextCurrent(window);
  chipLog("GLFW Initialized");

  chipLog("Initializing Chip");
  chip_t chip;
  chipInit(&chip);
  chipLoad(&chip, argv[1]);

  double next;
  short iterations;

  while(!glfwWindowShouldClose(window)){
    next = glfwGetTime() + 1.f / 60;
    iterations = 0;

    while(glfwGetTime() < next) {
      if(iterations < CHIP8_MAX_ITER) {
        chipStep(&chip);
        iterations++;
      }
    }

    glClear(GL_COLOR_BUFFER_BIT);
    chipDraw(&chip);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  chipLog("Terminating GLFW");
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
