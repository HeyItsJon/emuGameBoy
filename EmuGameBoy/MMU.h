#pragma once

#include <cstdint>
using namespace std;

class MMU
{
public:
	MMU();
	~MMU();
	uint8_t ReadData(uint16_t addr);
	bool WriteData(uint16_t addr, uint8_t data);
	bool LoadROM(); // parameters TBD

private:
	uint8_t bios[0x100];
	uint8_t memory[0x10000];
	uint8_t cartridge[0x8000]; // might need to be dynamically allocated?  LoadROM has size parameter?
};

