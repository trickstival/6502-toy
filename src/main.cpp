#include <cstdio>
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

  Byte SP; // Stack Pointer
  Word PC; // Program Counter

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
    SP = 0xFF;
    D = 0;

    // Clear flags
    C = Z = I = D = B = V = N = 0;

    // Clear Registers
    A = X = Y = 0;

    ram.Initialize();
  }
  


  // opcodes
  static constexpr Byte INS_LDA_IM = 0xA9;
  static constexpr Byte INS_LDA_ZP = 0xA5;
  static constexpr Byte INS_LDA_ZPX = 0xB5;
  static constexpr Byte INS_JSR = 0x20;

  Byte Fetch (u32& cycles, RAM& ram) {
    Byte data = ram.Data[PC];
    PC++;
    cycles--;
    return data;
  }

  Byte ReadByte (u32& cycles, RAM& ram, Byte addr) {
    Byte data = ram.Data[addr];
    cycles--;
    return data;
  }

  void LDA_Reload_Status () {
      Z = A == 0;
      N = (A & 0b10000000) > 0;
  }

  Word FetchWord (u32& cycles, RAM& ram) {
    Word Value = ram.Data[PC];
    PC++;
    Value |= (ram.Data[PC] << 8);
    PC++;

    cycles -= 2;
    return Value;
  }

  void WriteWord (u32& cycles, RAM& ram, Word value, Word addr) {
    ram.Data[addr] = value & 0xFF;
    ram.Data[addr + 1] = value >> 8;

    cycles -= 2;
  }

  void Execute (u32 cycles, RAM& ram) { 
    while (cycles > 0) {

      Byte NextInstruction = Fetch(
        cycles,
        ram
      );

      switch (NextInstruction) {
        case INS_LDA_IM: {
          Byte valueToLoad = Fetch(cycles, ram);
          A = valueToLoad;
          LDA_Reload_Status();

          printf("Loaded value into A: %d", valueToLoad);
        } break;
        case INS_LDA_ZP: {
          Byte ZeroPageAddr = Fetch(cycles, ram);
          A = ReadByte(cycles, ram, ZeroPageAddr);
          LDA_Reload_Status();

          printf("Loaded ZP into A: %d", A);
        } break;
        case INS_LDA_ZPX: {
          Byte ZeroPageAddr = Fetch(cycles, ram);

          ZeroPageAddr += X;
          ZeroPageAddr = ZeroPageAddr % 0xFF;
          cycles--;

          A = ReadByte(cycles, ram, ZeroPageAddr);
          LDA_Reload_Status();

          printf("Loaded ZPX into A: %d", A);
        } break;
        case INS_JSR: {
          Word subroutine_addr = FetchWord(cycles, ram);
          WriteWord(cycles, ram, PC - 1, SP);
          PC = subroutine_addr;
          SP--;
          cycles--;
        } break;
        default: {
          printf("Instruction not handled: %d", NextInstruction);
        }
      }
    }
  }
};


int main () {
  CPU cpu;
  RAM ram;
  cpu.Reset(ram);

  // Inline program
  ram.Data[cpu.PC] = CPU::INS_LDA_ZP;
  ram.Data[cpu.PC + 1] = 0x42;
  ram.Data[0x42] = 88;

  // End inline program
  cpu.Execute(3, ram);


  return 0;
}

