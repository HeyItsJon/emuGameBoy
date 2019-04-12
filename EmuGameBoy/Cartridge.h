#pragma once

#include <cstdint>
#include <iostream>
#include "ErrorCodes.h"
using namespace std;

#define RAM_MODE true
#define ROM_MODE false
#define ROM_ONLY 0x00
#define MBC1 0x01
#define MBC2 0x02
#define MBC3 0x03

class Cartridge
{
public:
	Cartridge();
	~Cartridge();
	int8_t LoadROM(char*);
	int8_t WriteROM(uint16_t, uint8_t);
	int8_t ReadROM(uint16_t, uint8_t*);
	int8_t WriteRAM(uint16_t, uint8_t);
	int8_t ReadRAM(uint16_t, uint8_t*);

private:
	//uint8_t ROM[0x8000]; // 125 possible ROM Banks
						 // TODO: Dynamically allocate ROM array based on ROM size
	uint8_t *ROM;
	uint8_t RAM[0x8000]; // 4 possible RAM Banks
	bool RAMEnable;
	bool BankMode;
	uint8_t ROMBank;
	uint8_t RAMBank;
	uint8_t MBC;
	// TODO: Set MBC in LoadROM, implement checking in Read/Write functions
};

