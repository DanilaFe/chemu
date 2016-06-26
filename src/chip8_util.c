#include "chip8_util.h"
#include <glfw/glfw3.h>
#include <stdio.h>

void chipDraw(chip_t* chip){
  for(int w = 0; w < 64; w++){
    for(int h = 0; h < 32; h++){
      int index = h * 64 + w;
      int enabled = chip->display[index];
      if(enabled){
        glBegin(GL_QUADS);
          glVertex2d(2.f * (w + 1) / 64 - 1, -(2.f * (h + 1) / 32 - 1));
          glVertex2d(2.f * (w + 1) / 64 - 1, -(2.f * h / 32 - 1));
          glVertex2d(2.f * w / 64 - 1, -(2.f * h / 32 - 1));
          glVertex2d(2.f * w / 64 - 1, -(2.f * (h + 1) / 32 - 1));
        glEnd();
      }
    }
  }
}
void chipLog(const char* logtext){
  #ifdef CHIP8_USE_LOG
  if(CHIP8_USE_LOG)
    printf("[Chip-8] %s\n", logtext);
  #endif
}

void chipPrintInfo() {
  printf("-----------\n");
  printf("Chip-8 Emulator By DanilaFe\n");
  printf("Compiled Against GLFW: %i.%i\n", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR);
  printf("Logging Enabled: %i\n", CHIP8_USE_LOG);
  printf("Target Iterations Per Second: %i\n", CHIP8_MAX_ITER);
  printf("-----------\n");
}
