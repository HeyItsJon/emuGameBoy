#pragma once

#include <cstdint>
#include <iostream>
#include "SDL.h"
#include "ErrorCodes.h"
using namespace std;

typedef struct _channel
{
	uint8_t sweepperiod;
	uint8_t sweeptimer;
	bool sweepdown;
	uint8_t sweepshift;
	bool sweepenable;
	uint16_t shadowreg;
	int8_t *dutyptr;
	uint8_t dutypos;
	uint16_t dutytimer;
	uint8_t length;
	uint8_t volume;
	bool envelopeup;
	uint8_t envelopeperiod;
	uint8_t envelopetimer;
	uint16_t frequency;
	bool init;
	bool lengthenable;
} channel;

class APU
{
public:
	APU();
	~APU();
	int8_t WriteRegister(uint16_t, uint8_t);
	int8_t ReadRegister(uint16_t, uint8_t*);
	int8_t PlaySound(int8_t, bool init = false);
	int8_t UpdateAPU(uint8_t);

private:
	// Registers
	uint8_t NR10;
	uint8_t NR11;
	uint8_t NR12;
	uint8_t NR13;
	uint8_t NR14;
	uint8_t NR21;
	uint8_t NR22;
	uint8_t NR23;
	uint8_t NR24;
	uint8_t NR30;
	uint8_t NR31;
	uint8_t NR32;
	uint8_t NR33;
	uint8_t NR34;
	uint8_t NR41;
	uint8_t NR42;
	uint8_t NR43;
	uint8_t NR44;
	uint8_t NR50;
	uint8_t NR51;
	uint8_t NR52;

	//Devices
	//uint16_t progtmr;
	//uint8_t configtmr;
	//uint8_t lencntr;
	//uint8_t ulencntr;
	//uint8_t volctrl;
	//uint8_t masterout;
	//uint8_t freqsweep;
	//uint8_t dutygen;
	channel CH1;
	channel CH2;
	
	// Wave pattern RAM
	uint8_t waveRAM[0x10];

	// Methods
	int8_t GetSquareChannelSample(uint8_t);
	int8_t GetNoiseChannelSample();
	int8_t GetWaveChannelSample();
};

