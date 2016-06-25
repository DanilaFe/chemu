#include <glfw/glfw3.h>
#include <stdio.h>
#include "chip8.h"

#define CHIP8_USE_LOG true
#define CHIP8_MAX_ITER 20

void chiplog(const char* logtext){
  #ifdef CHIP8_USE_LOG
  printf("%s\n", logtext);
  #endif
}

int main(int argc, char** argv){
  if(argc == 1) {
    chiplog("Please enter a program to emulate.");
    return 0;
  }

  chiplog("Initializing GLFW");
  glfwInit();
  GLFWwindow* window = glfwCreateWindow(64 * 8, 32 * 8, "Chip-8", NULL, NULL);
  glfwMakeContextCurrent(window);
  chiplog("GLFW Initialized");

  chiplog("Initializing Chip");
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

  chiplog("Terminating GLFW");
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
