#include "chip8.h"
#include <stdio.h>
#include <glfw/glfw3.h>

void chipInit(chip_t* chip) {
  chip->pc = 0x200;
  chip->i = 0;
  chip->stackp = 0;
  chip->delay_timer = 0;
  chip->sound_timer = 0;
  for(int i = 0; i <   16; i++) chip->v[i] = 0;
  for(int i = 0; i < 4096; i++) chip->memory[i] = 0;
  for(int i = 0; i <   16; i++) chip->stack[i] = 0;
  for(int i = 0; i < (64 *32); i++) chip->display[i] = 0;
}
void chipStep(chip_t* chip) {

}
void chipLoad(chip_t* chip, const char* file){
  FILE* filep = fopen(file, "r");
  fread(chip->memory + 0x200, sizeof(char), 4096 - 0x200, filep);
  fclose(filep);
}
void chipDraw(chip_t* chip){
  for(int w = 0; w < 64; w++){
    for(int h = 0; h < 32; h++){
      int index = h * 64 + w;
      int enabled = chip->display[index];
      if(enabled){
        glBegin(GL_QUADS);
          glVertex2d(2.f * (w + 1) / 64 - 1, 2.f * (h + 1) / 32 - 1);
          glVertex2d(2.f * (w + 1) / 64 - 1, 2.f * h / 32 - 1);
          glVertex2d(2.f * w / 64 - 1, 2.f * h / 32 - 1);
          glVertex2d(2.f * w / 64 - 1, 2.f * (h + 1) / 32 - 1);
        glEnd();
      }
    }
  }
}
