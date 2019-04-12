#include "stdafx.h"
#include "GPU.h"

GPU::GPU()
{
	// Initialize registers
	LCDC = 0x00; // 0x91;
	STAT = 0x00;
	SCY = 0x00;
	SCX = 0x00;
	LY = 0x00;
	LYC = 0x00;
	DMA = 0x00;
	BGP = 0x00; // 0xFC;
	OBP0 = 0xFF; // ???
	OBP1 = 0xFF; // ???
	WY = 0x00;
	WX = 0x00;

	// Clear VRAM and OAM
	memset(VRAM, 0x00, sizeof(VRAM));
	memset(OAM, 0x00, sizeof(OAM));

	// Initalize display
	SDL_InitSubSystem(SDL_INIT_VIDEO);
	UpdateDisplay(true);
}


GPU::~GPU()
{
	// Close display
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

int8_t GPU::WriteVRAM(uint16_t addr, uint8_t data)
{
	if (addr > 0x1FFF)
		return ADDRESS_OUT_OF_RANGE;

	if (((STAT & 0x03) == 0x03) && (LCDC & 0x80)) // Mode 3
		return SUCCESS; // Error?

	VRAM[addr] = data;

	return SUCCESS;
}

int8_t GPU::WriteOAM(uint16_t addr, uint8_t data)
{
	if (addr > 0x9F)
		return ADDRESS_OUT_OF_RANGE;

	if ((STAT & 0x02) && (LCDC & 0x80)) // Mode 2 or 3
		return SUCCESS; // Error?

	OAM[addr] = data;

	return SUCCESS;
}

int8_t GPU::WriteRegister(uint16_t addr, uint8_t data)
{
	if ((addr > 0xFF4B) || (addr < 0xFF40))
		return ADDRESS_OUT_OF_RANGE;

	switch (addr)
	{
	case 0xFF40:
		LCDC = data;
		break;
	case 0xFF41:
		STAT = (data & 0xF8) | (STAT & 0x07);
		break;
	case 0xFF42:
		SCY = data;
		break;
	case 0xFF43:
		SCX = data;
		break;
	case 0xFF44:
		LY = 0;
		break;
	case 0xFF45:
		LYC = data;
		break;
	case 0xFF46:
		DMA = data;
		break;
	case 0xFF47:
		BGP = data;
		break;
	case 0xFF48:
		OBP0 = data;
		break;
	case 0xFF49:
		OBP1 = data;
		break;
	case 0xFF4A:
		WY = data;
		break;
	case 0xFF4B:
		WX = data;
		break;
	default:
		break; // Error?
	}

	return SUCCESS;
}

int8_t GPU::ReadVRAM(uint16_t addr, uint8_t *data)
{
	if (addr > 0x1FFF)
		return ADDRESS_OUT_OF_RANGE;

	if (((STAT & 0x03) == 0x03) && (LCDC & 0x80)) // Mode 3
		return SUCCESS; // Error?

	*data = VRAM[addr];

	return SUCCESS;
}

int8_t GPU::ReadOAM(uint16_t addr, uint8_t *data)
{
	if (addr > 0x9F)
		return ADDRESS_OUT_OF_RANGE;

	if ((STAT & 0x02) && (LCDC & 0x80)) // Mode 2 or 3
		return SUCCESS; // Error?

	*data = OAM[addr];

	return SUCCESS;
}

int8_t GPU::ReadRegister(uint16_t addr, uint8_t *data)
{
	if ((addr > 0xFF4B) || (addr < 0xFF40))
		return ADDRESS_OUT_OF_RANGE;

	switch (addr)
	{
	case 0xFF40:
		*data = LCDC;
		break;
	case 0xFF41:
		*data = STAT;
		break;
	case 0xFF42:
		*data = SCY;
		break;
	case 0xFF43:
		*data = SCX;
		break;
	case 0xFF44:
		*data = LY;
		break;
	case 0xFF45:
		*data = LYC;
		break;
	case 0xFF47:
		*data = BGP;
		break;
	case 0xFF48:
		*data = OBP0;
		break;
	case 0xFF49:
		*data = OBP1;
		break;
	case 0xFF4A:
		*data = WY;
		break;
	case 0xFF4B:
		*data = WX;
		break;
	default:
		*data = 0x00;
		break; // Error?
	}

	return SUCCESS;
}

int8_t GPU::UpdateDisplay(bool init)
{
	static SDL_Window *window = SDL_CreateWindow("EmuGameBoy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 160*3, 144*3, SDL_WINDOW_SHOWN);
	static SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	static SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB4444, SDL_TEXTUREACCESS_STREAMING, 160, 144); 
	uint16_t pixels[160 * 144] = { 0x00 }; // each pixel in ARGB form, 16 bits each
	
	if (init)
	{
		SDL_Surface *icon = SDL_LoadBMP("C:/EmuCHIP-8/C8.bmp"); // TODO
		SDL_SetWindowIcon(window, icon);
		SDL_FreeSurface(icon);
		return SUCCESS;
	}

	RenderTo(pixels);
	SDL_UpdateTexture(texture, nullptr, pixels, 320); // 320 - 2*160, 2 bytes per pixel?
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);
	SDL_RenderPresent(renderer);

	return SUCCESS;
}

uint8_t GPU::UpdateGPU(uint8_t cycles)
{
	static uint32_t totalcycles = 0;
	uint8_t irq = 0;	// DECODER:
						// Bit 0 - V-Blank Interrupt
						// Bit 1 - LCDC interrupt (non-V-Blank interrupt)

	totalcycles += cycles;

	if (totalcycles > 456)
	{
		totalcycles -= 456;
		LY++;
		if (LY == 144)
		{
			// Mode 1
			STAT &= 0x03;
			STAT |= 0x01;
			irq |= 0x01; // V-Blank interrupt
			if (STAT & 0x10)
				irq |= 0x02;
		}
		else if (LY == 154)
		{
			LY = 0;
		}

		if (LY == LYC)
		{
			// Coincidence
			STAT |= 0x04;
			if (STAT & 0x40)
				irq |= 0x02; // LYC coincidence - LCDC interrupt
		}
	}

	if (LY < 144)
	{
		if (totalcycles < 205)
		{
			// Mode 0
			STAT &= 0xFC;
			if (STAT & 0x08)
				irq |= 0x02; // Mode 00 (H-Blank) - LCDC interrupt
		}
		else if (totalcycles < 285)
		{
			// Mode 2
			STAT &= 0xFC;
			STAT |= 0x02;
			if (STAT & 0x20)
				irq |= 0x02; // Mode 10 (OAM read) - LCDC interrupt
		}
		else if (totalcycles < 457)
		{
			// Mode 3
			STAT |= 0x03;
		}
	}

	return irq;
}

uint8_t GPU::ReadSTAT()
{
	return STAT;
}

int8_t GPU::DMATransfer(uint8_t *data)
{
	memcpy(OAM, data, 0xA0);

	return SUCCESS;
}

int8_t GPU::RenderTo(uint16_t pixels[])
{
	uint16_t colors[4] = { WHITE, LIGHT_GRAY, DARK_GRAY, BLACK };
	uint16_t pallet[4] = { 0 };
	uint8_t xval = 0;
	uint8_t yval = 0;
	uint8_t xlimit = 0;
	uint8_t ylimit = 0;
	uint16_t pixelcnt = 0;
	uint8_t lsb = 0;
	uint8_t msb = 0;
	uint8_t color = 0;
	uint8_t tile[16] = { 0 };
	uint8_t tilenumber = 0;
	uint8_t shiftval = 0;
	uint16_t tilemapstart = (LCDC & 0x08) ? 0x1C00 : 0x1800;

	if (!(LCDC & 0x80))
	{
		for (int i = 0; i < 160 * 144; i++)
			pixels[i] = WHITE;
		return SUCCESS;
	}

	// Construct background and window pallet table
	pallet[0] = colors[BGP & 0x03];
	pallet[1] = colors[(BGP & 0x0C) >> 2];
	pallet[2] = colors[(BGP & 0x30) >> 4];
	pallet[3] = colors[(BGP & 0xC0) >> 6];
	
	// Render background

	if (LCDC & 0x01)
	{
		// Iterate over the display rows
		yval = SCY;
		ylimit = SCY + 144;
		do
		{
			// Iterate over the display columns
			xval = SCX;
			xlimit = SCX + 160;
			do
			{
				// Get tile number
				tilenumber = VRAM[tilemapstart + ((yval / 8) * 32) + (xval / 8)];

				// Get bytes from tile
				if (LCDC & 0x10)
				{
					lsb = VRAM[tilenumber * 16 + (yval % 8) * 2];
					msb = VRAM[tilenumber * 16 + (yval % 8) * 2 + 1];
				}
				else
				{
					lsb = VRAM[((int8_t)tilenumber * 16) + 0x1000 + (yval % 8) * 2];
					msb = VRAM[((int8_t)tilenumber * 16) + 0x1000 + (yval % 8) * 2 + 1];
				}

				// Iterate over the pixels in the specified row of the tile
				do
				{
					shiftval = 7 - (xval % 8);
					color = (((msb >> shiftval) << 1) & 0x02) | ((lsb >> shiftval) & 0x01);
					pixels[pixelcnt++] = pallet[color];
					xval++;
				} while (((xval % 8) != 0) && (xval != xlimit));

			} while (xval != xlimit);

			yval++;
		} while (yval != ylimit);
	}
	else
	{
		for (int i = 0; i < 160 * 144; i++)
			pixels[i] = WHITE;
	}

	// Render window

	//// TODO: (make window separate texture, place on top of background texture?)
	if (LCDC & 0x20)
	{
		tilemapstart = (LCDC & 0x40) ? 0x1C00 : 0x1800;

		// Iterate over the display rows
		yval = 0;
		ylimit = 144 - WY;
		do
		{
			// Iterate over the display columns
			xval = 0;
			xlimit = 166 - WX;
			do
			{
				// Get tile number
				tilenumber = VRAM[tilemapstart + ((yval / 32) * 32) + (xval / 32)];

				// Get bytes from tile
				if (LCDC & 0x10)
				{
					lsb = VRAM[tilenumber * 16 + (yval % 8) * 2];
					msb = VRAM[tilenumber * 16 + (yval % 8) * 2 + 1];
				}
				else
				{
					lsb = VRAM[((int8_t)tilenumber * 16) + 0x1000 + (yval % 8) * 2];
					msb = VRAM[((int8_t)tilenumber * 16) + 0x1000 + (yval % 8) * 2 + 1];
				}

				// Iterate over the pixels in the specified row of the tile, overwrite background
				do
				{
					shiftval = 7 - (xval % 8);
					color = (((msb >> shiftval) << 1) & 0x02) | ((lsb >> shiftval) & 0x01);
					if (((int8_t)WX - 7 + xval) >= 0)
						pixels[((WY + yval) * 160) + (WX - 7 + xval)] = pallet[color];
					xval++;
				} while (((xval % 8) != 0) && (xval != xlimit));

			} while (xval != xlimit);

			yval++;
		} while (yval != ylimit);
	}

	//for (int row = 0; row < 144; )
	//{
	//	yval += row;
	//	xval = SCX;
	//	for (int col = 0; col < 160; )
	//	{
	//		xval += col;

	//		// Get tile number
	//		tilenumber = VRAM[tilemapstart + ((yval / 32) * 32) + (xval / 32)];

	//		// Get 16-byte tile data
	//		if (LCDC & 0x10)
	//			memcpy(tile, &(VRAM[tilenumber * 16]), 16);
	//		else
	//			memcpy(tile, &(VRAM[((int8_t)tilenumber * 16) + 0x1000]), 16);
	//		
	//		for (int i = yval % 8; i < 8; i++)
	//		{
	//			for (int j = xval % 8; j < 8; j++)
	//			{
	//				byte0 = tile[i * 2];
	//				byte1 = tile[(i * 2) + 1];
	//				color = (((byte1 >> (7 - j)) << 1) & 0x02) | ((byte0 >> (7 - j)) & 0x01);
	//				pixels[(rowcnt * 160) + colcnt] = pallet[color];
	//			}
	//		}

	//		
	//	}
	//}

	return SUCCESS;
}
