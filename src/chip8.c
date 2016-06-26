#include "chip8.h"
#include <stdio.h>
#include <glfw/glfw3.h>
#include <stdlib.h>

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

  if(head == 0x0){
    unsigned char tail = opcode & 0xf;
    if(tail == 0x0) {
      chipClear(chip);
    }
    else if(tail == 0xe){
      chip->pc = chip->stack[--(chip->stackp)];
    }
  } else if(head == 0x1) {
    unsigned short int addr = opcode & 0xfff;
    chip->pc = addr - 2;
  } else if(head == 0x2) {
    unsigned short int addr = opcode & 0xfff;
    chip->stack[(chip->stackp)++] = chip->pc;
    chip->pc = addr - 2;
  } else if(head == 0x3) {
    unsigned char reg = ((opcode & 0xf00) >> 8) & 0xff;
    unsigned char nn = opcode & 0xff;
    if(chip->v[reg] == nn) (chip->pc) += 2;
  } else if(head == 0x4) {
    unsigned char reg = ((opcode & 0xf00) >> 8) & 0xff;
    unsigned char nn = opcode & 0xff;
    if(chip->v[reg] != nn) (chip->pc) += 2;
  } else if(head == 0x5) {
    unsigned char regx = ((opcode & 0xf00) >> 8) & 0xff;
    unsigned char regy = ((opcode & 0xf0 ) >> 4) & 0xff;
    if(chip->v[regx] == chip->v[regy]) (chip->pc) += 2;
  } else if(head == 0x6) {
    unsigned char reg = ((opcode & 0xf00) >> 8) & 0xff;
    unsigned char nn = opcode & 0xff;
    chip->v[reg] = nn;
  } else if(head == 0x7) {
    unsigned char reg = ((opcode & 0xf00) >> 8) & 0xff;
    unsigned char nn = opcode & 0xff;
    chip->v[reg] = (chip->v[reg] + nn) & 0xff;
  } else if(head == 0x8) {
    unsigned char regx = ((opcode & 0xf00) >> 8) & 0xff;
    unsigned char regy = ((opcode & 0xf0 ) >> 4) & 0xff;
    unsigned char tail = opcode & 0xf;
    if(tail == 0x0) chip->v[regx] = chip->v[regy];
    else if(tail == 0x1) chip->v[regx] |= chip->v[regy];
    else if(tail == 0x2) chip->v[regx] &= chip->v[regy];
    else if(tail == 0x3) chip->v[regx] ^= chip->v[regy];
    else if(tail == 0x4) {
      unsigned short int result = chip->v[regx] + chip->v[regy];
      chip->v[0xf] = !((result & 0xff00) == 0);
      chip->v[regx] = result & 0xff;
    } else if(tail == 0x5) {
      unsigned short int result = chip->v[regx] - chip->v[regy];
      chip->v[0xf] = ((result & 0xff00) == 0);
      chip->v[regx] = result & 0xff;
    } else if(tail == 0x6) {
      chip->v[0xf] = chip->v[regx] & 0x1;
      chip->v[regx] = chip->v[regx] >> 1;
    } else if(tail == 0x7) {
      unsigned short int result = chip->v[regy] - chip->v[regx];
      chip->v[0xf] = ((result & 0xff00) == 0);
      chip->v[regx] = result & 0xff;
    } else if(tail == 0xe) {
      chip->v[0xf] = chip->v[regx] & 0x80;
      chip->v[regx] = (chip->v[regx] << 1) & 0xff;
    }
  } else if(head == 0x9) {
    unsigned char regx = ((opcode & 0xf00) >> 8) & 0xff;
    unsigned char regy = ((opcode & 0xf0 ) >> 4) & 0xff;
    if(chip->v[regx] != chip->v[regy]) (chip->pc) += 2;
  } else if(head == 0xA) {
    chip->i = opcode & 0xfff;
  } else if(head == 0xB) {
    chip->pc = chip->i + chip->v[0];
  } else if(head == 0xC) {
    unsigned char random = rand() % 0xff;
    unsigned char regx = ((opcode & 0xf00) >> 8) & 0xff;
    unsigned char nn = opcode & 0xff;
    chip->v[regx] = random & nn;
  } else if(head == 0xD) {
    unsigned char regx = ((opcode & 0xf00) >> 8) & 0xff;
    unsigned char regy = ((opcode & 0xf0 ) >> 4) & 0xff;
    unsigned char height = opcode & 0xf;
    chip->v[0xf] = 0;
    for(int y = 0; y < height; y++){
      unsigned char sprline = chip->memory[chip->i + y];
      for(int x = 0; x < 8; x++){
        unsigned char pixel = sprline & (0x80 >> x);
        if(pixel){
          unsigned char drawx = (chip->v[regx] + x) % 64;
          unsigned char drawy = (chip->v[regy] + y) % 32;
          unsigned short int disp = drawx + drawy * 64;
          if(chip->display[disp]) chip->v[0xf] = 1;
          chip->display[disp] ^= 1;
        }
      }
    }
  } else if(head == 0xE) {
    unsigned char regx = ((opcode & 0xf00) >> 8) & 0xff;
    unsigned char tail = opcode  & 0xff;
    if(tail == 0x9e) { chip->pc += 0; }
    if(tail == 0xa1) { chip->pc += 2; }
  } else if(head == 0xF) {
    unsigned char regx = ((opcode & 0xf00) >> 8) & 0xff;
    unsigned char tail = opcode & 0xff;
    if(tail == 0x07) chip->v[regx] = chip->delay_timer;
    else if(tail == 0x0a) {
      chip->v[regx] = 0;
    }
    else if(tail == 0x15) chip->delay_timer = chip->v[regx];
    else if(tail == 0x18) chip->sound_timer = chip->v[regx];
    else if(tail == 0x1e) chip->i += chip->v[regx];
    else if(tail == 0x29) {
      chip->i = 0x50 + (chip->v[regx] * 5);
    }
    else if(tail == 0x33) {
      unsigned char value = chip->v[regx];
      unsigned char units = value % 10;
      unsigned char tens =  (value % 100) / 10;
      unsigned char hundreds = value / 100;
      chip->memory[chip->i] = hundreds;
      chip->memory[chip->i + 1] = tens;
      chip->memory[chip->i + 2] = units;
    }
    else if(tail == 0x55) {
      for(int i = 0; i <= regx; i++) chip->memory[chip->i + i] = chip->v[i];
    }
    else if(tail == 0x65) {
      for(int i = 0; i <= regx; i++) chip->v[i] = chip->memory[chip->i + i];
    }
  }

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
