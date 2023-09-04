#include <stdio.h>
#include <stdlib.h>

// 8 bits
typedef unsigned char Byte;

// 16 bits
typedef unsigned short Word;

typedef unsigned int u32;

struct RAM {
  static constexpr u32 MAX_MEM = 1024 * 64;

  Byte Data[MAX_MEM];

  void Initialize () {
    for (u32 i = 0; i < MAX_MEM; i++) {
      Data[i] = 0;
    }
  }
};

struct CPU {

  Word PC, SP; // Program Counter, Stack Pointer

  Byte A, X, Y; // Registers
  
  Byte C: 1; // Carry flag
  Byte Z: 1; // Zero flag
  Byte I: 1; // Interrupt disable
  Byte D: 1; // Decimal mode
  Byte B: 1; // Break command
  Byte V: 1; // Overflow flag
  Byte N: 1; // Negative flag

  void Reset (RAM& ram) {
    PC = 0xFFFC;
    SP = 0x0100;
    D = 0;

    // Clear flags
    C = Z = I = D = B = V = N = 0;

    // Clear Registers
    A = X = Y = 0;

    ram.Initialize();
  }
};

int main () {
  CPU cpu;
  RAM ram;
  cpu.Reset(ram);
  return 0;
}

