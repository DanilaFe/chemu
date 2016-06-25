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

  unsigned char fontset[] = {
	  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	  0x20, 0x60, 0x20, 0x20, 0x70, // 1
	  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};
  for(int i = 0; i < 90; i++){
    chip->memory[0x50 + i] = fontset[i];
  }
}
void chipClear(chip_t* chip){
  for(int i = 0; i < (64 *32); i++) chip->display[i] = 0;
}
void chipStep(chip_t* chip) {
  unsigned short int opcode = (chip->memory[chip->pc] << 8) | (chip->memory[chip->pc + 1]);
  unsigned char head = ((opcode & 0xf000) >> 12) & 0xff;

  (chip->pc) += 2;
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
