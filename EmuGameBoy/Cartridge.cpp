#include "stdafx.h"
#include "Cartridge.h"


Cartridge::Cartridge()
{
	RAMEnable = false;
	BankMode = ROM_MODE;
	ROMBank = 1;
	RAMBank = 0;
	MBC = 0;

	memset(RAM, 0x00, sizeof(RAM));
}


Cartridge::~Cartridge()
{
}

int8_t Cartridge::LoadROM(char *path)
{
	int status = SUCCESS;
	long size = 0;
	FILE *romfile;

	status = fopen_s(&romfile, path, "rb");
	if (status != SUCCESS)
		return status;

	// Get ROM size
	fseek(romfile, 0, SEEK_END);
	size = ftell(romfile);

	// Go back to the file's beginning
	fseek(romfile, 0, SEEK_SET);

	// Allocate ROM array
	ROM = (uint8_t *)calloc(size, sizeof(uint8_t));

	// Read ROM into array
	fread(ROM, sizeof(uint8_t), size, romfile);

	// Close file
	fclose(romfile);

	// Set MBC mode
	switch (ROM[0x0147])
	{
	case 0x00:
		MBC = ROM_ONLY;
		break;
	case 0x01:
	case 0x02:
	case 0x03:
		MBC = MBC1;
		break;
	case 0x05:
	case 0x06:
		MBC = MBC2;
		break;
	default:
		MBC = ROM_ONLY;
		status = UNDEFINED_MBC_MODE;
		break;
	}

	ROMBank = 1;
	RAMBank = 0;

	return status;
}

int8_t Cartridge::WriteROM(uint16_t addr, uint8_t data)
{
	int status = SUCCESS;

	if (addr < 0x2000)
	{
		if (MBC == MBC1)
		{
			if ((data & 0x0F) == 0x0A)
				RAMEnable = true;
			else
				RAMEnable = false;
		}
		else if (MBC == MBC2)
		{
			// Is this correct?
			if ((data & 0x10) == 0x00)
			{
				if ((data & 0x0F) == 0x0A)
					RAMEnable = true;
				else
					RAMEnable = false;
			}
		}
	}
	else if (addr < 0x4000)
	{
		if (MBC == MBC1)
		{
			ROMBank &= 0x60; // Clear lower five bits
			ROMBank |= (data & 0x1F);
			if ((ROMBank == 0) || (ROMBank == 0x20) || (ROMBank == 0x40) || (ROMBank == 0x60))
				ROMBank++;
		}
		else if (MBC == MBC2)
		{
			if ((addr & 0x10) == 0x10)
				ROMBank = (data & 0x0F);
		}
	}
	else if (addr < 0x6000)
	{
		if (MBC == MBC1)
		{
			if (BankMode == ROM_MODE)
			{
				ROMBank &= 0x1F; // Clear upper 2 bits
				ROMBank |= ((data & 0x03) << 5);
			}
			else
			{
				RAMBank = (data & 0x03);
			}
		}
	}
	else if (addr < 0x8000)
	{
		if (MBC == MBC1)
			BankMode = (data & 0x01);
	}
	else
		status = ADDRESS_OUT_OF_RANGE;

	return status;
}

int8_t Cartridge::ReadROM(uint16_t addr, uint8_t *data)
{
	if (addr > 0x7FFF)
		return ADDRESS_OUT_OF_RANGE;

	if (addr < 0x4000)
	{
		*data = ROM[addr];
	}
	else if (addr < 0x8000)
	{
		*data = ROM[0x4000 * (ROMBank - 1)  + addr]; // unclear if this is correct
	}

	return SUCCESS;
}

int8_t Cartridge::WriteRAM(uint16_t addr, uint8_t data)
{
	if (addr > 0x1FFF)
		return ADDRESS_OUT_OF_RANGE;

	if (MBC == ROM_ONLY)
	{
		RAM[addr] = data;
	}
	else if (MBC == MBC1)
	{
		
		RAM[0x2000 * RAMBank + addr] = data; // unclear if this is correct
	}
	else if (MBC == MBC2)
	{
		RAM[0x2000 * RAMBank + addr] = (data & 0x0F); // unclear if this is correct
	}

	return SUCCESS;
}

int8_t Cartridge::ReadRAM(uint16_t addr, uint8_t *data)
{
	if (addr > 0x1FF)
		return ADDRESS_OUT_OF_RANGE;

	if (MBC == ROM_ONLY)
	{
		*data = RAM[addr];
	}
	else if (MBC == MBC1)
	{
		*data = RAM[0x2000 * RAMBank + addr]; // unclear if this is correct
	}
	else if (MBC == MBC2)
	{
		*data = (RAM[0x2000 * RAMBank + addr] & 0x0F); // unclear if this is correct
	}

	return SUCCESS;
}
