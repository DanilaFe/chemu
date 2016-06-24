#include <glfw/glfw3.h>
#include <stdio.h>
#include "chip8.h"

#define CHIP8_USE_LOG true

void chiplog(const char* logtext){
  #ifdef CHIP8_USE_LOG
  printf("%s\n", logtext);
  #endif
}

int main(int argc, char** argv){
  chiplog("Initializing GLFW");
  glfwInit();
  GLFWwindow* window = glfwCreateWindow(64 * 8, 32 * 8, "Chip-8", NULL, NULL);
  glfwMakeContextCurrent(window);
  chiplog("GLFW Initialized");

  chiplog("Initializing Chip");
  chip_t chip;
  chipInit(&chip);
  chipLoad(&chip, "files/particles.ch8");

  while(!glfwWindowShouldClose(window)){
    glClear(GL_COLOR_BUFFER_BIT);
    chipStep(&chip);
    chipDraw(&chip);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  chiplog("Terminating GLFW");
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
