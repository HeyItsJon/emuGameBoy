// EmuGameBoy.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdint.h>
#include "CPU.h"
using namespace std;

int main(int argc, char *argv[])
{
	SDL_Init(0);
	CPU cpu;
	int status = SUCCESS;

	// TODO: Read INI file
	/*
	// Get values from ini file
	GetPrivateProfileString("SETTINGS", "ROM_PATH", "C:/EmuGameBoy/GBgames/", filePathINI, sizeof(filePathINI) / sizeof(filePathINI[0]), "C:/EmuCHIP-8.ini");
	int cycles = GetPrivateProfileInt("SETTINGS", "CYCLES", 10, "C:/EmuCHIP-8/EmuCHIP-8.ini");

	filePath = string(filePathINI);
	if (argc > 1)
	filePath += argv[1];
	else
	filePath += "PONG2";

	if (chip8.Init() < 0)
	{
	cerr << "Error during initialization!" << endl;
	cin.get();
	exit(1);
	}
	if (!chip8.LoadMemory(filePath))
	{
	cerr << "ROM not found!" << endl;
	cin.get();
	exit(1);
	}
	*/

	// Welcome message
	cout << "\nEmuGameBoy - A Game Boy Emulator" << endl;
	cout << "v0.1 xx-xx-2019" << endl;
	cout << "Written by: Jon Altenburger\n" << endl;

	status = cpu.LoadCartridge("C:/Users/jattenbu/Documents/Git/Emu-Docs/Game Boy/test_roms/cpu_instrs/cpu_instrs.gb");
	if (status != SUCCESS)
	{
		cout << "An error occured loading the ROM!\nError Code: " << status << endl;
	}


	//// DEBUG - timing test
	//chrono::duration<float> diff;
	//auto t1 = chrono::high_resolution_clock::now();
	//auto lastframe = chrono::high_resolution_clock::now();
	//const float frame = 70224.0 / 4194304.0;
	//do
	//{
	//	diff = chrono::high_resolution_clock::now() - lastframe;
	//} while (diff.count() < frame);
	//lastframe = chrono::high_resolution_clock::now();
	//diff = lastframe - t1;
	//cout << "diff = " << diff.count() << endl;
	////

	while (status == SUCCESS)
	//for (int i = 0; i < 1000; i++)
		status = cpu.StepFrame();

	if (status != SUCCESS)
		cout << "An error occured!" << endl;

	cin.get(); // DEBUG
	SDL_Quit();

    return 0;
}

// TODO:
// H flag logic probably not correct (especially DEC n?)
// Input
// Audio sweep not 100% working?
// DMA, plus logic for only accessing HRAM (671 cycles)
// Sprites