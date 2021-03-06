#ifndef CHIP8_HEADER
#define CHIP8_HEADER
#define CHIP8_MAX_ITER 20

struct chip_s {
  unsigned char memory[4096];
  unsigned short int pc;
  unsigned char v[16];
  unsigned short int i;

  unsigned short int stack[16];
  unsigned char stackp;

  unsigned int delay_timer;
  unsigned int sound_timer;

  unsigned char display[32 * 64];
};

typedef struct chip_s chip_t;

void chipInit(chip_t* chip);
void chipClear(chip_t* chip);
void chipStep(chip_t* chip);
void chipLoad(chip_t* chip, const char* file);

#endif /* end of include guard: CHIP8_HEADER */
