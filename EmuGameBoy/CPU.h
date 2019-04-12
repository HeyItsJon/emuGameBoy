#pragma once

#include <cstdint>
#include <iostream>
#include <chrono>
//#include "MMU.h"
#include "GPU.h"
#include "Cartridge.h"
#include "APU.h"
#include "ErrorCodes.h"
using namespace std;

// Handy macros and definitions
#define REG_BC BC.reg
#define REG_B BC.hi
#define REG_C BC.lo
#define REG_DE DE.reg
#define REG_D DE.hi
#define REG_E DE.lo
#define REG_HL HL.reg
#define REG_H HL.hi
#define REG_L HL.lo
#define REG_AF AF.reg
#define REG_A AF.hi
#define FLAG_REG AF.lo
#define FLAG_Z (AF.lo & 0x80)
#define FLAG_N (AF.lo & 0x40)
#define FLAG_H (AF.lo & 0x20)
#define FLAG_C (AF.lo & 0x10)
#define SET_Z() (AF.lo |= 0x80)
#define CLEAR_Z() (AF.lo &= 0x7F)
#define SET_N() (AF.lo |= 0x40)
#define CLEAR_N() (AF.lo &= 0xBF)
#define SET_H() (AF.lo |= 0x20)
#define CLEAR_H() (AF.lo &= 0xDF)
#define SET_C() (AF.lo |= 0x10)
#define CLEAR_C() (AF.lo &= 0xEF)

typedef struct Register
{
	union
	{
		uint16_t reg;
		struct
		{
			uint8_t lo;
			uint8_t hi;
		};
	};
} _Register;

class CPU
{
public:
	CPU();
	~CPU();
	int8_t StepFrame();
	int8_t LoadCartridge(char*);

private:
	// CPU components
	GPU gpu;
	Cartridge cart;
	APU apu;

	// Registers
	Register PC;
	Register SP;
	Register AF;
	Register BC;
	Register DE;
	Register HL;

	// Memory
	uint8_t bootrom[256];
	uint8_t RAM[8192];
	uint8_t HRAM[127];
	uint8_t IO[52]; // For IO registers not used by GPU and APU (between 0xFF4C and 0xFF7F)

	// Special Registers
	// Input
	uint8_t P1;
	// Serial I/O
	uint8_t SB;
	uint8_t SC;
	// Timer
	uint8_t DIV;
	uint8_t TIMA;
	uint8_t TMA;
	uint8_t TAC;
	// Interrupt
	uint8_t IF;
	uint8_t IE;

	// Special values
	bool boot;
	bool IME;
	
	// Methods
	int8_t ReadData(uint16_t, uint8_t*);
	int8_t WriteData(uint16_t, uint8_t);
	uint8_t ExecuteOpCode(int8_t*);
	int8_t PushStack(uint8_t, uint8_t);
	int8_t PopStack(uint8_t*, uint8_t*);
	void HandleTimer(int8_t*, uint8_t); // void?
	void HandleInterrupts(int8_t*); // void?
	// TODO: Implement Timer and Interrupt functionality
};

