#pragma once

#include <cstdint>
#include <iostream>
#include "SDL.h"
#include "ErrorCodes.h"
using namespace std;


// ARGB4444 - ARGB (16-bit)
// Set AA = 0xF for all colors to make them fully opaque
// Colors:
// 0 (white) = 0xFFFF
// 1 (light gray) = 0xFAAA
// 2 (dark gray) = 0xF555
// 3 (black) = 0xF000
#define WHITE 0xFFFF
#define LIGHT_GRAY 0xFAAA
#define DARK_GRAY 0xF555
#define BLACK 0xF000

class GPU
{
public:
	GPU();
	~GPU();
	int8_t WriteVRAM(uint16_t, uint8_t);
	int8_t WriteOAM(uint16_t, uint8_t);
	int8_t WriteRegister(uint16_t, uint8_t);
	int8_t ReadVRAM(uint16_t, uint8_t*);
	int8_t ReadOAM(uint16_t, uint8_t*);
	int8_t ReadRegister(uint16_t, uint8_t*);
	int8_t UpdateDisplay(bool init = false);
	uint8_t UpdateGPU(uint8_t);
	uint8_t ReadSTAT();
	int8_t DMATransfer(uint8_t*);

private:
	// 8kB VRAM
	uint8_t VRAM[0x2000];
	// Sprite Attribute Table
	uint8_t OAM[0xA0];

	// Registers
	uint8_t LCDC;
	uint8_t STAT;
	uint8_t SCY;
	uint8_t SCX;
	uint8_t LY;
	uint8_t LYC;
	uint8_t DMA;
	uint8_t BGP;
	uint8_t OBP0;
	uint8_t OBP1;
	uint8_t WY;
	uint8_t WX;

	// Methods
	int8_t RenderTo(uint16_t[]);
};

