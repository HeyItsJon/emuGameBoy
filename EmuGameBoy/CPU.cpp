#include "stdafx.h"
#include "CPU.h"


CPU::CPU()
{
	// Initialize registers
	PC.reg = 0x0000; // 0x0000;
	AF.reg = 0x0000; // 0x01B0;
	BC.reg = 0x0000; // 0x0013;
	DE.reg = 0x0000; // 0x00D8;
	HL.reg = 0x0000; // 0x014D;
	SP.reg = 0x0000; // 0xFFFE;
	TIMA = 0x00;
	TMA = 0x00;
	TAC = 0x00;
	IF = 0x00;
	IE = 0x00;

	// Start in boot mode
	boot = true;

	// Clear interrupt master enable
	IME = false;

	// Clear RAM and HRAM
	memset(RAM, 0x00, sizeof(RAM));
	memset(HRAM, 0x00, sizeof(HRAM));

	// Fill the bootrom
	bootrom[0x00] = 0x31;
	bootrom[0x01] = 0xFE;
	bootrom[0x02] = 0xFF;
	bootrom[0x03] = 0xAF;
	bootrom[0x04] = 0x21;
	bootrom[0x05] = 0xFF;
	bootrom[0x06] = 0x9F;
	bootrom[0x07] = 0x32;
	bootrom[0x08] = 0xCB;
	bootrom[0x09] = 0x7C;
	bootrom[0x0A] = 0x20;
	bootrom[0x0B] = 0xFB;
	bootrom[0x0C] = 0x21;
	bootrom[0x0D] = 0x26;
	bootrom[0x0E] = 0xFF;
	bootrom[0x0F] = 0x0E;
	bootrom[0x10] = 0x11;
	bootrom[0x11] = 0x3E;
	bootrom[0x12] = 0x80;
	bootrom[0x13] = 0x32;
	bootrom[0x14] = 0xE2;
	bootrom[0x15] = 0x0C;
	bootrom[0x16] = 0x3E;
	bootrom[0x17] = 0xF3;
	bootrom[0x18] = 0xE2;
	bootrom[0x19] = 0x32;
	bootrom[0x1A] = 0x3E;
	bootrom[0x1B] = 0x77;
	bootrom[0x1C] = 0x77;
	bootrom[0x1D] = 0x3E;
	bootrom[0x1E] = 0xFC;
	bootrom[0x1F] = 0xE0;
	bootrom[0x20] = 0x47;
	bootrom[0x21] = 0x11;
	bootrom[0x22] = 0x04;
	bootrom[0x23] = 0x01;
	bootrom[0x24] = 0x21;
	bootrom[0x25] = 0x10;
	bootrom[0x26] = 0x80;
	bootrom[0x27] = 0x1A;
	bootrom[0x28] = 0xCD;
	bootrom[0x29] = 0x95;
	bootrom[0x2A] = 0x00;
	bootrom[0x2B] = 0xCD;
	bootrom[0x2C] = 0x96;
	bootrom[0x2D] = 0x00;
	bootrom[0x2E] = 0x13;
	bootrom[0x2F] = 0x7B;
	bootrom[0x30] = 0xFE;
	bootrom[0x31] = 0x34;
	bootrom[0x32] = 0x20;
	bootrom[0x33] = 0xF3;
	bootrom[0x34] = 0x11;
	bootrom[0x35] = 0xD8;
	bootrom[0x36] = 0x00;
	bootrom[0x37] = 0x06;
	bootrom[0x38] = 0x08;
	bootrom[0x39] = 0x1A;
	bootrom[0x3A] = 0x13;
	bootrom[0x3B] = 0x22;
	bootrom[0x3C] = 0x23;
	bootrom[0x3D] = 0x05;
	bootrom[0x3E] = 0x20;
	bootrom[0x3F] = 0xF9;
	bootrom[0x40] = 0x3E;
	bootrom[0x41] = 0x19;
	bootrom[0x42] = 0xEA;
	bootrom[0x43] = 0x10;
	bootrom[0x44] = 0x99;
	bootrom[0x45] = 0x21;
	bootrom[0x46] = 0x2F;
	bootrom[0x47] = 0x99;
	bootrom[0x48] = 0x0E;
	bootrom[0x49] = 0x0C;
	bootrom[0x4A] = 0x3D;
	bootrom[0x4B] = 0x28;
	bootrom[0x4C] = 0x08;
	bootrom[0x4D] = 0x32;
	bootrom[0x4E] = 0x0D;
	bootrom[0x4F] = 0x20;
	bootrom[0x50] = 0xF9;
	bootrom[0x51] = 0x2E;
	bootrom[0x52] = 0x0F;
	bootrom[0x53] = 0x18;
	bootrom[0x54] = 0xF3;
	bootrom[0x55] = 0x67;
	bootrom[0x56] = 0x3E;
	bootrom[0x57] = 0x64;
	bootrom[0x58] = 0x57;
	bootrom[0x59] = 0xE0;
	bootrom[0x5A] = 0x42;
	bootrom[0x5B] = 0x3E;
	bootrom[0x5C] = 0x91;
	bootrom[0x5D] = 0xE0;
	bootrom[0x5E] = 0x40;
	bootrom[0x5F] = 0x04;
	bootrom[0x60] = 0x1E;
	bootrom[0x61] = 0x02;
	bootrom[0x62] = 0x0E;
	bootrom[0x63] = 0x0C;
	bootrom[0x64] = 0xF0;
	bootrom[0x65] = 0x44;
	bootrom[0x66] = 0xFE;
	bootrom[0x67] = 0x90;
	bootrom[0x68] = 0x20;
	bootrom[0x69] = 0xFA;
	bootrom[0x6A] = 0x0D;
	bootrom[0x6B] = 0x20;
	bootrom[0x6C] = 0xF7;
	bootrom[0x6D] = 0x1D;
	bootrom[0x6E] = 0x20;
	bootrom[0x6F] = 0xF2;
	bootrom[0x70] = 0x0E;
	bootrom[0x71] = 0x13;
	bootrom[0x72] = 0x24;
	bootrom[0x73] = 0x7C;
	bootrom[0x74] = 0x1E;
	bootrom[0x75] = 0x83;
	bootrom[0x76] = 0xFE;
	bootrom[0x77] = 0x62;
	bootrom[0x78] = 0x28;
	bootrom[0x79] = 0x06;
	bootrom[0x7A] = 0x1E;
	bootrom[0x7B] = 0xC1;
	bootrom[0x7C] = 0xFE;
	bootrom[0x7D] = 0x64;
	bootrom[0x7E] = 0x20;
	bootrom[0x7F] = 0x06;
	bootrom[0x80] = 0x7B;
	bootrom[0x81] = 0xE2;
	bootrom[0x82] = 0x0C;
	bootrom[0x83] = 0x3E;
	bootrom[0x84] = 0x87;
	bootrom[0x85] = 0xE2;
	bootrom[0x86] = 0xF0;
	bootrom[0x87] = 0x42;
	bootrom[0x88] = 0x90;
	bootrom[0x89] = 0xE0;
	bootrom[0x8A] = 0x42;
	bootrom[0x8B] = 0x15;
	bootrom[0x8C] = 0x20;
	bootrom[0x8D] = 0xD2;
	bootrom[0x8E] = 0x05;
	bootrom[0x8F] = 0x20;
	bootrom[0x90] = 0x4F;
	bootrom[0x91] = 0x16;
	bootrom[0x92] = 0x20;
	bootrom[0x93] = 0x18;
	bootrom[0x94] = 0xCB;
	bootrom[0x95] = 0x4F;
	bootrom[0x96] = 0x06;
	bootrom[0x97] = 0x04;
	bootrom[0x98] = 0xC5;
	bootrom[0x99] = 0xCB;
	bootrom[0x9A] = 0x11;
	bootrom[0x9B] = 0x17;
	bootrom[0x9C] = 0xC1;
	bootrom[0x9D] = 0xCB;
	bootrom[0x9E] = 0x11;
	bootrom[0x9F] = 0x17;
	bootrom[0xA0] = 0x05;
	bootrom[0xA1] = 0x20;
	bootrom[0xA2] = 0xF5;
	bootrom[0xA3] = 0x22;
	bootrom[0xA4] = 0x23;
	bootrom[0xA5] = 0x22;
	bootrom[0xA6] = 0x23;
	bootrom[0xA7] = 0xC9;
	bootrom[0xA8] = 0xCE;
	bootrom[0xA9] = 0xED;
	bootrom[0xAA] = 0x66;
	bootrom[0xAB] = 0x66;
	bootrom[0xAC] = 0xCC;
	bootrom[0xAD] = 0x0D;
	bootrom[0xAE] = 0x00;
	bootrom[0xAF] = 0x0B;
	bootrom[0xB0] = 0x03;
	bootrom[0xB1] = 0x73;
	bootrom[0xB2] = 0x00;
	bootrom[0xB3] = 0x83;
	bootrom[0xB4] = 0x00;
	bootrom[0xB5] = 0x0C;
	bootrom[0xB6] = 0x00;
	bootrom[0xB7] = 0x0D;
	bootrom[0xB8] = 0x00;
	bootrom[0xB9] = 0x08;
	bootrom[0xBA] = 0x11;
	bootrom[0xBB] = 0x1F;
	bootrom[0xBC] = 0x88;
	bootrom[0xBD] = 0x89;
	bootrom[0xBE] = 0x00;
	bootrom[0xBF] = 0x0E;
	bootrom[0xC0] = 0xDC;
	bootrom[0xC1] = 0xCC;
	bootrom[0xC2] = 0x6E;
	bootrom[0xC3] = 0xE6;
	bootrom[0xC4] = 0xDD;
	bootrom[0xC5] = 0xDD;
	bootrom[0xC6] = 0xD9;
	bootrom[0xC7] = 0x99;
	bootrom[0xC8] = 0xBB;
	bootrom[0xC9] = 0xBB;
	bootrom[0xCA] = 0x67;
	bootrom[0xCB] = 0x63;
	bootrom[0xCC] = 0x6E;
	bootrom[0xCD] = 0x0E;
	bootrom[0xCE] = 0xEC;
	bootrom[0xCF] = 0xCC;
	bootrom[0xD0] = 0xDD;
	bootrom[0xD1] = 0xDC;
	bootrom[0xD2] = 0x99;
	bootrom[0xD3] = 0x9F;
	bootrom[0xD4] = 0xBB;
	bootrom[0xD5] = 0xB9;
	bootrom[0xD6] = 0x33;
	bootrom[0xD7] = 0x3E;
	bootrom[0xD8] = 0x3C;
	bootrom[0xD9] = 0x42;
	bootrom[0xDA] = 0xB9;
	bootrom[0xDB] = 0xA5;
	bootrom[0xDC] = 0xB9;
	bootrom[0xDD] = 0xA5;
	bootrom[0xDE] = 0x42;
	bootrom[0xDF] = 0x3C;
	bootrom[0xE0] = 0x21;
	bootrom[0xE1] = 0x04;
	bootrom[0xE2] = 0x01;
	bootrom[0xE3] = 0x11;
	bootrom[0xE4] = 0xA8;
	bootrom[0xE5] = 0x00;
	bootrom[0xE6] = 0x1A;
	bootrom[0xE7] = 0x13;
	bootrom[0xE8] = 0xBE;
	bootrom[0xE9] = 0x20;
	bootrom[0xEA] = 0xFE;
	bootrom[0xEB] = 0x23;
	bootrom[0xEC] = 0x7D;
	bootrom[0xED] = 0xFE;
	bootrom[0xEE] = 0x34;
	bootrom[0xEF] = 0x20;
	bootrom[0xF0] = 0xF5;
	bootrom[0xF1] = 0x06;
	bootrom[0xF2] = 0x19;
	bootrom[0xF3] = 0x78;
	bootrom[0xF4] = 0x86;
	bootrom[0xF5] = 0x23;
	bootrom[0xF6] = 0x05;
	bootrom[0xF7] = 0x20;
	bootrom[0xF8] = 0xFB;
	bootrom[0xF9] = 0x86;
	bootrom[0xFA] = 0x20;
	bootrom[0xFB] = 0xFE;
	bootrom[0xFC] = 0x3E;
	bootrom[0xFD] = 0x01;
	bootrom[0xFE] = 0xE0;
	bootrom[0xFF] = 0x50;
}


CPU::~CPU()
{
}


int8_t CPU::StepFrame()
{
	int8_t status = SUCCESS;
	uint8_t cycles = 0;
	uint8_t gpu_irq = 0;
	static uint32_t totalcycles = 0;
	chrono::duration<float> diff;
	static auto lastframe = chrono::high_resolution_clock::now();
	const float frame = 70224.0 / 4194304.0;

	while ((totalcycles < 70224) && (status == SUCCESS))
	{
		cycles = ExecuteOpCode(&status);
		totalcycles += cycles;

		HandleTimer(&status, cycles);

		gpu_irq = gpu.UpdateGPU(cycles);
		if (gpu_irq & 0x01)
			IF |= 0x01;
		if (gpu_irq & 0x02)
			IF |= 0x02;

		apu.UpdateAPU(cycles);

		HandleInterrupts(&status);
	}

	// Wait for end of frame
	do
	{
		diff = chrono::high_resolution_clock::now() - lastframe;
	} while (diff.count() < frame);
	lastframe = chrono::high_resolution_clock::now();

	// Update LCD
	gpu.UpdateDisplay();

	// DEBUG
	//apu.PlaySound();
	
	totalcycles -= 70224;

	return status;
}

int8_t CPU::LoadCartridge(char *path)
{
	return cart.LoadROM(path);
}

int8_t CPU::ReadData(uint16_t addr, uint8_t *data)
{
	int8_t status = SUCCESS;

	if (addr < 0x8000)
	{
		if (boot)
		{
			if (addr < 0x100)
			{
				*data = bootrom[addr];

				if (addr == 0xFF)
				{
					boot = false;
					//PC.reg = 0x0100; // ???
				}
			}
			else
				status = cart.ReadROM(addr, data);		
		}
		else
		{
			status = cart.ReadROM(addr, data);
		}
	}
	else if (addr < 0xA000)
	{
		// Read GPU VRAM
		status = gpu.ReadVRAM(addr - 0x8000, data);
	}
	else if (addr < 0xC000)
	{
		// Read cartridge RAM
		status = cart.ReadRAM(addr - 0xA000, data);
	}
	else if (addr < 0xE000)
	{
		// Read internal RAM
		*data = RAM[addr - 0xC000];
	}
	else if (addr < 0xFE00)
	{
		// Read internal RAM (echo of 0xC000-0xDDFF)
		*data = RAM[addr - 0xE000];
	}
	else if (addr < 0xFEA0)
	{
		// Read GPU sprite attribute table
		status = gpu.ReadOAM(addr - 0xFE00, data);
	}
	else if (addr < 0xFF00)
	{
		*data = 0x00;
		status = ILLEGAL_MEMORY_ACCESS; // illegal address
	}
	else if (addr < 0xFF10)
	{
		// TODO
		// Input plus some illegal stuff?
		*data = 0x00;
	}
	else if (addr < 0xFF40)
	{
		// Read APU registers
		status = apu.ReadRegister(addr, data);
	}
	else if (addr < 0xFF4C)
	{
		// Read GPU registers
		status = gpu.ReadRegister(addr, data);
	}
	else if (addr < 0xFF80)
	{
		// Unspecified?
		*data = IO[addr - 0xFF4C];
	}
	else if (addr < 0xFFFF)
	{
		// Read internal Hi RAM
		*data = HRAM[addr - 0xFF80];
	}
	else if (addr == 0xFFFF)
	{
		// Read Interrupt Enable
		*data = IE;
	}
	else
	{
		*data = 0x00;
		status = ADDRESS_OUT_OF_RANGE; // address out of range
	}

	return status;
}

int8_t CPU::WriteData(uint16_t addr, uint8_t data)
{
	int8_t status = SUCCESS;

	if (addr < 0x8000)
	{
		if (!boot)
		{
			// Write to cartridge ROM (bank select, etc.)
			status = cart.WriteROM(addr, data);
		}
	}
	else if (addr < 0xA000)
	{
		// Write to GPU
		status = gpu.WriteVRAM(addr - 0x8000, data);
	}
	else if (addr < 0xC000)
	{
		// Write to cartridge RAM
		status = cart.WriteRAM(addr - 0xA000, data);
	}
	else if (addr < 0xE000)
	{
		// Write to CPU working RAM
		RAM[addr - 0xC000] = data;
	}
	else if (addr < 0xFE00)
	{
		// Write to CPU working RAM (echo of 0xC000-0xDDFF)
		RAM[addr - 0xE000] = data;
	}
	else if (addr < 0xFEA0)
	{
		// Write to GPU sprite attribute table
		status = gpu.WriteOAM(addr - 0xFE00, data);
	}
	else if (addr < 0xFF00)
	{
		// Illegal address
		status = ILLEGAL_MEMORY_ACCESS;
	}
	else if (addr < 0xFF10)
	{
		// TODO
		// Input plus some illegal stuff?
		if (addr == 0x0FF04)
			DIV = 0x00;
	}
	else if (addr < 0xFF40)
	{
		// Write APU registers
		status = apu.WriteRegister(addr, data);
	}
	else if (addr < 0xFF4C)
	{
		// Write to GPU registers
		status = gpu.WriteRegister(addr, data);

		// Check for DMA
		//if (addr == 0xFF46)
		//	status = gpu.DMATransfer((uint16_t)data << 8);
	}
	else if (addr < 0xFF80)
	{
		// Unspecified?
		IO[addr - 0xFF4C] = data;
	}
	else if (addr < 0xFFFF)
	{
		// Write to CPU HRAM
		HRAM[addr - 0xFF80] = data;
	}
	else if (addr == 0xFFFF)
	{
		// Write Interrupt Enable
		IE = data;
	}
	else
	{
		// Address out of range
		status = ADDRESS_OUT_OF_RANGE;
	}

	return status;
}

uint8_t CPU::ExecuteOpCode(int8_t *status)
{
	uint8_t opcode = 0;
	uint8_t operand1 = 0;
	uint8_t operand2 = 0;
	uint8_t temp = 0;
	uint8_t temp2 = 0;
	uint8_t cycles = 0;
	static bool DI = false;

	if (DI)
	{
		DI = false;
		IME = false;
	}

	// Grab opcode
	*status = ReadData(PC.reg, &opcode);
	if (*status != SUCCESS)
	{
		cout << "Read Failure!\nError Code: " << status << endl;
		return 0;
	}

	// Grab operands that may or may not be used - for convenience
	*status = ReadData(PC.reg + 1, &operand1);
	if (*status != SUCCESS)
	{
		cout << "Read Failure!\nError Code: " << status << endl;
		return 0;
	}

	*status = ReadData(PC.reg + 2, &operand2);
	if (*status != SUCCESS)
	{
		cout << "Read Failure!\nError Code: " << status << endl;
		return 0;
	}

	//cout << "PC=" << hex << PC.reg << ", opcode=" << hex << (int)opcode << endl;

	switch (opcode)
	{
	case 0x00: // NOP
		cycles = 4;
		PC.reg++;
		break;
	case 0x01: // LD BC,nn
		BC.hi = operand2;
		BC.lo = operand1;
		PC.reg += 3;
		cycles = 12;
		break;
	case 0x02: // LD (BC),A
		*status = WriteData(REG_BC, REG_A);
		PC.reg++;
		cycles = 8;
		break;
	case 0x03: // INC BC
		REG_BC++;
		PC.reg++;
		cycles = 8;
		break;
	case 0x04: // INC B
		REG_B++;
		PC.reg++;
		cycles = 4;
		(REG_B == 0) ? SET_Z() : CLEAR_Z();
		((REG_B & 0x0F) == 0x00) ? SET_H() : CLEAR_H();
		CLEAR_N();
		break;
	case 0x05: // DEC B
		REG_B--;
		PC.reg++;
		cycles = 4;
		(REG_B == 0) ? SET_Z() : CLEAR_Z();
		((REG_B & 0x0F) == 0x0F) ? SET_H() : CLEAR_H();
		SET_N();
		break;
	case 0x06: // LD B,n
		REG_B = operand1;
		PC.reg += 2;
		cycles = 8;
		break;
	case 0x07: // RLCA
		temp = REG_A;
		REG_A = (REG_A << 1) & 0xFE;
		(REG_A == 0) ? SET_Z() : CLEAR_Z();
		CLEAR_N();
		CLEAR_H();
		(temp & 0x80) ? SET_C() : CLEAR_C();
		PC.reg++;
		cycles = 4;
		break;
	case 0x0B: // DEC BC
		REG_BC--;
		PC.reg++;
		cycles = 8;
		break;
	case 0x0C: // INC C
		REG_C++;
		PC.reg++;
		cycles = 4;
		(REG_C == 0) ? SET_Z() : CLEAR_Z();
		((REG_C & 0x0F) == 0x00) ? SET_H() : CLEAR_H();
		CLEAR_N();
		break;
	case 0x0D: // DEC C
		REG_C--;
		PC.reg++;
		cycles = 4;
		(REG_C == 0) ? SET_Z() : CLEAR_Z();
		((REG_C & 0x0F) == 0x0F) ? SET_H() : CLEAR_H();
		SET_N();
		break;
	case 0x0E: // LD C,e
		REG_C = operand1;
		PC.reg += 2;
		cycles = 8;
		break;
	case 0x11: // LD DE,nn
		DE.hi = operand2;
		DE.lo = operand1;
		PC.reg += 3;
		cycles = 12;
		break;
	case 0x13: // INC DE
		REG_DE++;
		PC.reg++;
		cycles = 8;
		break;
	case 0x15: // DEC D
		REG_D--;
		PC.reg++;
		cycles = 4;
		(REG_D == 0) ? SET_Z() : CLEAR_Z();
		((REG_D & 0x0F) == 0x0F) ? SET_H() : CLEAR_H();
		SET_N();
		break;
	case 0x16: // LD D,n
		REG_D = operand1;
		PC.reg += 2;
		cycles = 8;
		break;
	case 0x17: // RLA
		// Store initial value of A
		temp = REG_A;

		// Shift A with carry
		REG_A = REG_A << 1;
		if (FLAG_C != 0)
			REG_A++;

		// Set flags
		(temp & 0x80) ? SET_C() : CLEAR_C();
		(REG_A == 0) ? SET_Z() : CLEAR_Z();
		CLEAR_N();
		CLEAR_H();

		PC.reg++;
		cycles = 4;
		break;
	case 0x18: // JR n
		PC.reg += 2;
		PC.reg += (int8_t)operand1;
		cycles = 8;
		break;
	case 0x1A: // LD A,(DE)
		*status = ReadData(REG_DE, &REG_A);
		PC.reg++;
		cycles = 8;
		break;
	case 0x1D: // DEC E
		REG_E--;
		PC.reg++;
		cycles = 4;
		(REG_E == 0) ? SET_Z() : CLEAR_Z();
		((REG_E & 0x0F) == 0x0F) ? SET_H() : CLEAR_H();
		SET_N();
	case 0x1E: // LD E,n
		REG_E = operand1;
		PC.reg += 2;
		cycles = 8;
		break;
	case 0x20: // JR NZ,n
		if (!(FLAG_Z))
		{
			// Z flag clear
			PC.reg += (int8_t)operand1;
			cycles = 4;
		}
		PC.reg += 2;
		cycles += 12;
		break;
	case 0x21: // LD HL,nn
		HL.hi = operand2;
		HL.lo = operand1;
		PC.reg += 3;
		cycles = 8;
		break;
	case 0x22: // LDI (HL),A
		*status = WriteData(REG_HL, REG_A);
		REG_HL++;
		PC.reg++;
		cycles = 8;
		break;
	case 0x23: // INC HL
		REG_HL++;
		PC.reg++;
		cycles = 8;
		break;
	case 0x24: // INC H
		REG_H++;
		PC.reg++;
		cycles = 4;
		(REG_H == 0) ? SET_Z() : CLEAR_Z();
		((REG_H & 0x0F) == 0x00) ? SET_H() : CLEAR_H();
		CLEAR_N();
		break;
	case 0x26: // LD H,n
		REG_H = operand1;
		PC.reg += 2;
		cycles = 8;
		break;
	case 0x28: // JR Z,n
		if (FLAG_Z)
		{
			// Z flag set
			PC.reg += (int8_t)operand1;
			cycles = 4;
		}
		PC.reg += 2;
		cycles += 12;
		break;
	case 0x2A: // LDI A,(HL)
		*status = ReadData(REG_HL, &REG_A);
		REG_HL++;
		PC.reg++;
		cycles = 8;
		break;
	case 0x2C: // INC L
		REG_L++;
		PC.reg++;
		cycles = 4;
		(REG_L == 0) ? SET_Z() : CLEAR_Z();
		((REG_L & 0x0F) == 0x00) ? SET_H() : CLEAR_H();
		CLEAR_N();
		break;
	case 0x2D: // DEC L
		REG_L--;
		PC.reg++;
		cycles = 4;
		(REG_L == 0) ? SET_Z() : CLEAR_Z();
		((REG_L & 0x0F) == 0x0F) ? SET_H() : CLEAR_H();
		SET_N();
		break;
	case 0x2E: // LD L,n
		REG_L = operand1;
		PC.reg += 2;
		cycles = 8;
		break;
	case 0x31: // LD SP,nn
		SP.hi = operand2;
		SP.lo = operand1;
		PC.reg += 3;
		cycles = 8;
		break;
	case 0x32: // LDD (HL),A
		*status = WriteData(REG_HL, REG_A);
		REG_HL--;
		PC.reg++;
		cycles = 8;
		break;
	case 0x38: // JR C,n
		if (FLAG_C)
			PC.reg += (int8_t)operand1;
		else
			PC.reg += 2;
		cycles = 8;
		break;
	case 0x3C: // INC A
		REG_A++;
		(REG_A == 0) ? SET_Z() : CLEAR_Z();
		CLEAR_N();
		((REG_A & 0x0F) == 0x00) ? SET_H() : CLEAR_H();
		PC.reg++;
		cycles = 4;
		break;
	case 0x3D: // DEC A
		REG_A--;
		// Flags
		(REG_A == 0) ? SET_Z() : CLEAR_Z();
		SET_N();
		((REG_A & 0x0F) == 0x0F) ? SET_H() : CLEAR_H();
		PC.reg++;
		cycles = 4;
		break;
	case 0x3E: // LD A,e
		REG_A = operand1;
		PC.reg += 2;
		cycles = 8;
		break;
	case 0x46: // LD B,(HL)
		*status = ReadData(REG_HL, &REG_B);
		PC.reg++;
		cycles = 8;
		break;
	case 0x4E: // LD C,(HL)
		*status = ReadData(REG_HL, &REG_C);
		PC.reg++;
		cycles = 8;
		break;
	case 0x4F: // LD C,A
		REG_C = REG_A;
		PC.reg++;
		cycles = 4;
		break;
	case 0x56: // LD D,(HL)
		*status = ReadData(REG_HL, &REG_D);
		PC.reg++;
		cycles = 8;
		break;
	case 0x57: // LD D,A
		REG_D = REG_A;
		PC.reg++;
		cycles = 4;
		break;
	case 0x67: // LD H,A
		REG_H = REG_A;
		PC.reg++;
		cycles = 4;
		break;
	case 0x6F: // LD L,A
		REG_L = REG_A;
		PC.reg++;
		cycles = 4;
		break;
	case 0x77: // LD (HL),A
		*status = WriteData(REG_HL, REG_A);
		PC.reg++;
		cycles = 8;
		break;
	case 0x78: // LD A,B
		REG_A = REG_B;
		PC.reg++;
		cycles = 4;
		break;
	case 0x7A: // LD A,D
		REG_A = REG_D;
		PC.reg++;
		cycles = 4;
		break;
	case 0x7B: // LD A,E
		REG_A = REG_E;
		PC.reg++;
		cycles = 4;
		break;
	case 0x7C: // LD A,H
		REG_A = REG_H;
		PC.reg++;
		cycles = 4;
		break;
	case 0x7D: // LD A,L
		REG_A = REG_L;
		PC.reg++;
		cycles = 4;
		break;
	case 0x86: // ADD A,(HL)
		temp = REG_A;
		*status = ReadData(REG_HL, &temp2);
		REG_A += temp2;
		PC.reg++;
		cycles = 8;
		(REG_A == 0) ? SET_Z() : CLEAR_Z();
		CLEAR_N();
		(REG_A < temp) ? SET_C() : CLEAR_C();
		((REG_A & 0x0F) < (temp & 0x0F)) ? SET_H() : CLEAR_H();
		break;
	case 0x90: // SUB B
		temp = REG_A;
		REG_A -= REG_B;
		PC.reg++;
		cycles = 4;
		(REG_A == 0) ? SET_Z() : CLEAR_Z();
		SET_N();
		(REG_A > temp) ? CLEAR_C() : SET_C();
		((REG_A & 0x0F) > (temp & 0x0F)) ? CLEAR_C() : SET_C();
		break;
	case 0xA9: // XOR C
		REG_A ^= REG_C;
		(REG_A == 0) ? SET_Z() : CLEAR_Z();
		CLEAR_N();
		CLEAR_H();
		CLEAR_C();
		PC.reg++;
		cycles = 4;
		break;
	case 0xAE: // XOR (HL)
		*status = ReadData(REG_HL, &temp);
		REG_A ^= temp;
		(REG_A == 0) ? SET_Z() : CLEAR_Z();
		CLEAR_N();
		CLEAR_H();
		CLEAR_C();
		PC.reg++;
		cycles = 8;
		break;
	case 0xAF: // XOR A
		REG_A = 0x00; // clear A (xor something with itself clears it)
		SET_Z();
		CLEAR_N();
		CLEAR_H();
		CLEAR_C();
		PC.reg++;
		cycles = 4;
		break;
	case 0xB1: // OR C
		REG_A |= REG_C;
		(REG_A == 0) ? SET_Z() : CLEAR_Z();
		CLEAR_N();
		CLEAR_H();
		CLEAR_C();
		PC.reg++;
		cycles = 4;
		break;
	case 0xB7: // OR A
		// A | A = A
		(REG_A == 0) ? SET_Z() : CLEAR_Z();
		CLEAR_N();
		CLEAR_H();
		CLEAR_C();
		PC.reg++;
		cycles = 4;
		break;
	case 0xBE: // CP (HL)
		*status = ReadData(REG_HL, &temp);
		(REG_A == temp) ? SET_Z() : CLEAR_Z();
		SET_N();
		((REG_A & 0x0F) < (temp & 0x0F)) ? SET_H() : CLEAR_H();
		(REG_A < temp) ? SET_C() : CLEAR_C();
		PC.reg++;
		cycles = 8;
		break;
	case 0xC1: // POP BC
		*status = PopStack(&REG_B, &REG_C);
		PC.reg++;
		cycles = 12;
		break;
	case 0xC3: // JP nn
		PC.lo = operand1;
		PC.hi = operand2;
		cycles = 12;
		break;
	case 0xC4: // CALL NZ,nn
		PC.reg += 3;
		if (FLAG_Z == 0)
		{
			*status = PushStack(PC.hi, PC.lo);
			PC.hi = operand2;
			PC.lo = operand1;
		}
		cycles = 12;
		break;
	case 0xC5: // PUSH BC
		*status = PushStack(REG_B, REG_C);
		PC.reg++;
		cycles = 16;
		break;
	case 0xC6: // ADD A,#
		temp = REG_A;
		REG_A += operand1;
		PC.reg += 2;
		cycles = 8;
		(REG_A == 0) ? SET_Z() : CLEAR_Z();
		CLEAR_N();
		(temp > REG_A) ? SET_C() : CLEAR_C();
		((temp & 0x0F) > (REG_A & 0x0F)) ? SET_H() : CLEAR_H();
		break;
	case 0xC8: // RET Z
		if (FLAG_Z)
			*status = PopStack(&PC.hi, &PC.lo);
		else
			PC.reg++;
		cycles = 8;
		break;
	case 0xC9: // RET
		*status = PopStack(&PC.hi, &PC.lo);
		cycles = 8;
		break;
	case 0xCB:
		if ((operand1 & 0xC0) == 0x40) // BIT opcodes are special!!
		{
			switch (operand1 & 0x07)
			{
			//case 0x00: // BIT b,B
			//	*status = ILLEGAL_OPCODE;
			//	break;
			//case 0x01: // BIT b,C
			//	*status = ILLEGAL_OPCODE;
			//	break;
			//case 0x02: // BIT b,D
			//	*status = ILLEGAL_OPCODE;
			//	break;
			//case 0x03: // BIT b,E
			//	*status = ILLEGAL_OPCODE;
			//	break;
			case 0x04: // BIT b,H
				if (REG_H & (1 << ((operand1 & 0x38) >> 3)))
					CLEAR_Z();
				else
					SET_Z();
				CLEAR_N();
				SET_H();
				break;
			//case 0x05: // BIT b,L
			//	*status = ILLEGAL_OPCODE;
			//	break;
			//case 0x06: // BIT b,(HL)
			//	*status = ILLEGAL_OPCODE;
			//	break;
			//case 0x07: // BIT b,A
			//	*status = ILLEGAL_OPCODE;
			//	break;
			default:
				cout << "CB " << hex << (int)operand1 << " - Illegal opcode!" << endl;
				cout << "PC = " << hex << PC.reg << endl; // DEBUG
				break;
			}
			PC.reg += 2;
			cycles = 8;
		}
		else
		{
			switch (operand1)
			{
			case 0x11: // RL C
				// Store initial value of C
				temp = REG_C;

				// Shift C with carry
				REG_C = REG_C << 1;
				if (FLAG_C)
					REG_C++;

				// Set flags
				(temp & 0x80) ? SET_C() : CLEAR_C();
				(REG_C == 0) ? SET_Z() : CLEAR_Z();
				CLEAR_N();
				CLEAR_H();

				PC.reg += 2;
				cycles = 8;
				break;
			case 0x38: // SRL B
				temp = REG_B;
				REG_B = (REG_B >> 1) & 0x7F;
				(REG_B == 0) ? SET_Z() : CLEAR_Z();
				CLEAR_N();
				CLEAR_H();
				(temp & 0x01) ? SET_C() : CLEAR_C();
				PC.reg++;
				cycles = 8;
				break;
			default:
				*status = ILLEGAL_OPCODE;
				cout << "CB " << hex << (int)operand1 << " - Illegal opcode!" << endl;
				cout << "PC = " << hex << PC.reg << endl; // DEBUG
				break;
			}
		}
		break;
	case 0xCD: // CALL nn
		PC.reg += 3;
		*status = PushStack(PC.hi, PC.lo);
		PC.hi = operand2;
		PC.lo = operand1;
		cycles = 12;
		break;
	case 0xD0: // RET NC
		if (FLAG_C)
			PC.reg++;
		else
			*status = PopStack(&PC.hi, &PC.lo);
		cycles = 8;
		break;
	case 0xD5: // PUSH DE
		*status = PushStack(DE.hi, DE.lo);
		PC.reg++;
		cycles = 16;
		break;
	case 0xD6: // SUB #
		temp = REG_A;
		REG_A -= operand1;
		PC.reg += 2;
		cycles = 8;
		(REG_A == 0) ? SET_Z() : CLEAR_Z();
		SET_N();
		(temp > REG_A) ? SET_C() : CLEAR_C();
		((temp & 0x0F) > (REG_A & 0x0F)) ? SET_H() : CLEAR_H();
		break;
	case 0xE0: // LDH (n),a
		*status = WriteData((0xFF00 + operand1), REG_A);
		PC.reg += 2;
		cycles = 12;
		break;
	case 0xE1: // POP HL
		*status = PopStack(&REG_H, &REG_L);
		PC.reg++;
		cycles = 12;
		break;
	case 0xE2: // LD (C),A
		*status = WriteData((0xFF00 + REG_C), REG_A);
		PC.reg++;
		cycles = 8;
		break;
	case 0xE5: // PUSH HL
		*status = PushStack(REG_H, REG_L);
		PC.reg++;
		cycles = 16;
		break;
	case 0xE6: // AND #
		REG_A &= operand1;
		(REG_A == 0) ? SET_Z() : CLEAR_Z();
		CLEAR_N();
		SET_H();
		CLEAR_C();
		PC.reg += 2;
		cycles = 8;
		break;
	case 0xEA: // LD (nn),A
		*status = WriteData((((uint16_t)operand2 << 8) | (uint16_t)operand1), REG_A);
		PC.reg += 3;
		cycles = 16;
		break;
	case 0xF0: // LDH A,(n)
		*status = ReadData((0xFF00 + operand1), &REG_A);
		PC.reg += 2;
		cycles = 12;
		break;
	case 0xF1: // POP AF
		*status = PopStack(&REG_A, &FLAG_REG);
		PC.reg++;
		cycles = 12;
		break;
	case 0xF3: // DI
		PC.reg++;
		cycles = 4;
		DI = true;
		break;
	case 0xF5: // PUSH AF
		*status = PushStack(REG_A, FLAG_REG);
		PC.reg++;
		cycles = 16;
		break;
	case 0xF9: // LD SP,HL
		SP.reg = REG_HL;
		PC.reg++;
		cycles = 8;
		break;
	case 0xFA: // LD A,(nn)
		*status = ReadData(((((uint16_t)operand2 << 8) & 0xFF00) | (uint16_t)operand1), &REG_A);
		PC.reg += 3;
		cycles = 16;
		break;
	case 0xFE: // CP #
		(REG_A == operand1) ? SET_Z() : CLEAR_Z();
		SET_N();
		((REG_A & 0x0F) < (operand1 & 0x0F)) ? SET_H() : CLEAR_H();
		(REG_A < operand1) ? SET_C() : CLEAR_C();
		PC.reg += 2;
		cycles = 8;
		break;
	case 0xFF: // RST 38H
		PC.reg++; // ???
		*status = PushStack(PC.hi, PC.lo);
		PC.reg = 0x0038;
		cycles = 32;
		break;
	default:
		cout << hex << (int)opcode << " - Illegal opcode!" << endl;
		cout << "PC = " << hex << PC.reg << endl; // DEBUG
		*status = ILLEGAL_OPCODE;
		break;
	}

	return cycles;
}

int8_t CPU::PushStack(uint8_t addrhi, uint8_t addrlo)
{
	int8_t status = SUCCESS;

	// Stack is stored in Little Endian
	// SP points to MSB (SP -> MSB, SP-1 -> LSB)

	// Write high byte of address (MSB) to location pinted to by SP
	status = WriteData(SP.reg, addrhi);
	if (status != SUCCESS)
		return status;

	// Write low byte of address (LSB) to location pinted to by SP-1
	status = WriteData(SP.reg - 1, addrlo);

	// Decrement SP by 2 to point to the "empty" top of stack
	SP.reg -= 2;

	return status;
}

int8_t CPU::PopStack(uint8_t *addrhi, uint8_t *addrlo)
{
	int8_t status = SUCCESS;

	// Increment SP by 2 to point to the first value in the stack
	// SP will now point to the MSB of the address on top of the stack
	// The LSB is found at SP-1
	// The current values at the top of the stack will be overwritten if PushStack is called
	SP.reg += 2;

	// Read the MSB and store it in the high address passed in the function
	status = ReadData(SP.reg, addrhi);
	if (status != SUCCESS)
		return status;

	// Read the LSB and store it in the low address passed in the function
	return ReadData(SP.reg - 1, addrlo);
}

void CPU::HandleTimer(int8_t *status, uint8_t cycles)
{
	uint16_t cycles_for_update;
	static uint16_t totalcycles = 0;
	static uint16_t divcycles = 0;

	if (*status != SUCCESS)
		return;

	if (TAC & 0x04)
	{
		// Timer on, determine number of cycles for TIMA update
		switch (TAC & 0x03)
		{
		case 0x00: // 4.096kHz
			cycles_for_update = 1024;
			break;
		case 0x01: // 262.144kHz
			cycles_for_update = 16;
			break;
		case 0x02: // 65.536kHz
			cycles_for_update = 64;
			break;
		case 0x03: // 16.384kHz
			cycles_for_update = 256;
			break;
		default:
			cycles_for_update = 1024;
			break;
		}
	}
	else
	{
		// Timer off, reset cycles counters
		totalcycles = 0;
		divcycles = 0;
		return;
	}

	// Update cycle totals
	totalcycles += cycles;
	divcycles += cycles;

	// Update DIV register every 256 cycles
	if (divcycles > 255)
	{
		DIV++;
		divcycles -= 255;
	}

	// Update TIMA register every X cycles determined by TAC register
	if (totalcycles >= cycles_for_update)
	{
		TIMA++;
		totalcycles -= cycles_for_update;
		// Set the timer overflow interrupt flag if TIMA overflows, load TIMA with TMA
		if (TIMA == 0)
		{
			IF |= 0x04;
			TIMA = TMA;
		}
	}

	return;
}

void CPU::HandleInterrupts(int8_t *status)
{
	if (*status != SUCCESS)
		return;

	if (IME)
	{
		// Reset IME
		IME = false;

		// Push PC onto stack
		*status = PushStack(PC.hi, PC.lo);
		if (*status != SUCCESS)
			return;

		// Interrupt handling, in order of priority

		// V-Blank
		if ((IE & 0x01) && (IF & 0x01))
		{
			IF &= 0xFE;
			PC.reg = 0x0040;
		}
		// LCDC
		else if ((IE & 0x02) && (IF & 0x02))
		{
			IF &= 0xFD;
			PC.reg = 0x0048;
		}
		// Timer overflow
		else if ((IE & 0x04) && (IF & 0x04))
		{
			IF &= 0xFB;
			PC.reg = 0x0050;
		}
		// Serial transfer
		else if ((IE & 0x08) && (IF & 0x08))
		{
			// not implemented
			IF &= 0xF7;
		}
		// P10-P13 Hi-Lo
		else if ((IE & 0x10) && (IF & 0x10))
		{
			IF &= 0xEF;
			PC.reg = 0x0060;
		}
	}

	return;
}
