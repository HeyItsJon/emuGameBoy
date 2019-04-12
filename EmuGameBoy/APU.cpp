#include "stdafx.h"
#include "APU.h"

int8_t duty0[] = { -1, -1, -1, -1, -1, -1, -1 , 1 };
int8_t duty1[] = {  1, -1, -1, -1, -1, -1, -1,  1 };
int8_t duty2[] = {  1, -1, -1, -1, -1,  1,  1,  1 };
int8_t duty3[] = { -1,  1,  1,  1,  1,  1,  1, -1 };

APU::APU()
{
	// Initialize registers
	NR10 = 0x00;// 0x80;
	NR11 = 0x00;//0xBF;
	NR12 = 0x00;//0xF3;
	NR13 = 0x00;//0xFF;
	NR14 = 0x00;//0xBF;
	NR21 = 0x00;//0x3F;
	NR22 = 0x00;
	NR23 = 0x00;//0xFF;
	NR24 = 0x00;//0xBF;
	NR30 = 0x00;//0x7F;
	NR31 = 0x00;//0xFF;
	NR32 = 0x00;//0x9F;
	NR33 = 0x00;//0xBF;
	NR34 = 0x00;//0xFF;
	NR41 = 0x00;//0xFF;
	NR42 = 0x00;
	NR43 = 0x00;
	NR44 = 0x00;//0xBF;
	NR50 = 0x00;//0x77;
	NR51 = 0x00;//0xF3;
	NR52 = 0x00;//0xF1;

	// Initialize devices
	// Channel 1 - square wave with frequency sweep
	CH1.sweepperiod = (NR10 >> 4) & 0x07;
	CH1.sweeptimer = CH1.sweepperiod;
	CH1.sweepdown = NR10 & 0x08;
	CH1.sweepshift = NR10 & 0x07;
	CH1.sweepenable = false;
	CH1.frequency = (((uint16_t)NR14 << 8) | NR13) & 0x7FF;
	CH1.dutypos = 0;
	CH1.dutytimer = (2048 - CH1.frequency) * 4;
	CH1.length = NR11 & 0x3F;
	CH1.lengthenable = NR14 & 0x40;
	CH1.volume = (NR12 >> 4) & 0x0F;
	CH1.envelopeup = NR12 & 0x08;
	CH1.envelopeperiod = NR12 & 0x07;
	CH1.envelopetimer = CH1.envelopeperiod;
	CH1.shadowreg = CH1.frequency;
	CH1.init = NR14 & 0x80;
	switch (NR11 & 0xC0)
	{
	case 0x00:
		CH1.dutyptr = duty0;
		break;
	case 0x40:
		CH1.dutyptr = duty1;
		break;
	case 0x80:
		CH1.dutyptr = duty2;
		break;
	case 0xC0:
		CH1.dutyptr = duty3;
		break;
	default:
		CH1.dutyptr = duty0;
		break;
	}
	// Channel 2 - square wave
	CH2.sweepperiod = 0;
	CH2.sweeptimer = 0;
	CH2.sweepdown = false;
	CH2.sweepshift = 0;
	CH2.sweepenable = false;
	CH2.frequency = (((uint16_t)NR24 << 8) | NR23) & 0x7FF;
	CH2.dutypos = 0;
	CH2.dutytimer = (2048 - CH2.frequency) * 4;
	CH2.length = NR21 & 0x3F;
	CH2.lengthenable = NR24 & 0x40;
	CH2.volume = (NR22 >> 4) & 0x0F;
	CH2.envelopeup = NR22 & 0x08;
	CH2.envelopeperiod = NR22 & 0x07;
	CH2.envelopetimer = CH2.envelopeperiod;
	CH2.shadowreg = 0;
	CH2.init = NR24 & 0x80;
	switch (NR21 & 0xC0)
	{
	case 0x00:
		CH2.dutyptr = duty0;
		break;
	case 0x40:
		CH2.dutyptr = duty1;
		break;
	case 0x80:
		CH2.dutyptr = duty2;
		break;
	case 0xC0:
		CH2.dutyptr = duty3;
		break;
	default:
		CH2.dutyptr = duty0;
		break;
	}

	// Clear the RAM
	memset(waveRAM, 0x00, sizeof(waveRAM));

	SDL_InitSubSystem(SDL_INIT_AUDIO);

	PlaySound(0, true);
}


APU::~APU()
{
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

// TODO: Add functionality when registers are read/written

int8_t APU::WriteRegister(uint16_t addr, uint8_t data)
{
	int8_t status = SUCCESS;

	switch (addr)
	{
	case 0xFF10:
		NR10 = data;
		CH1.sweepperiod = (data >> 4) & 0x07;
		CH1.sweeptimer = CH1.sweepperiod; // ???
		CH1.sweepdown = data & 0x08;
		CH1.sweepshift = data & 0x07;
		break;
	case 0xFF11:
		NR11 = data;
		CH1.length = data & 0x3F;
		switch (data & 0xC0)
		{
		case 0x00:
			CH1.dutyptr = duty0;
			break;
		case 0x40:
			CH1.dutyptr = duty1;
			break;
		case 0x80:
			CH1.dutyptr = duty2;
			break;
		case 0xC0:
			CH1.dutyptr = duty3;
			break;
		default:
			CH1.dutyptr = duty0;
			break;
		}
		CH1.dutypos = 0;
		CH1.dutytimer = (2048 - CH1.frequency) * 4;
		break;
	case 0xFF12:
		NR12 = data;
		CH1.volume = (data >> 4) & 0x0F;
		CH1.envelopeup = data & 0x08;
		CH1.envelopeperiod = data & 0x07;
		CH1.envelopetimer = CH1.envelopeperiod; // ???
		break;
	case 0xFF13:
		NR13 = data;
		CH1.frequency = (((uint16_t)NR14 << 8) | data) & 0x7FF;
		break;
	case 0xFF14:
		NR14 = data;
		CH1.frequency = (((uint16_t)data << 8) | NR13) & 0x7FF;
		CH1.lengthenable = data & 0x40;
		// Trigger event
		if (data & 0x80)
		{
			CH1.init = true;
			if (CH1.length == 0)
				CH1.length = 64;
			CH1.envelopetimer = CH1.envelopeperiod;
			CH1.volume = (NR14 >> 4) & 0x0F;
			CH1.shadowreg = CH1.frequency;
			CH1.sweeptimer = CH1.sweepperiod;
			((CH1.sweepperiod > 0) || (CH1.sweepshift > 0)) ? (CH1.sweepenable = true) : (CH1.sweepenable = false);
			if (CH1.sweepshift > 0)
			{
				uint16_t temp = CH1.shadowreg;
				temp = temp >> CH1.sweepshift;
				if (CH1.sweepdown)
					temp = ~temp;
				temp += CH1.shadowreg;
				if (temp > 2047)
					CH1.sweepenable = false;
			}
		}
		break;
	case 0xFF16:
		NR21 = data;
		CH2.length = data & 0x3F;
		switch (data & 0xC0)
		{
		case 0x00:
			CH2.dutyptr = duty0;
			break;
		case 0x40:
			CH2.dutyptr = duty1;
			break;
		case 0x80:
			CH2.dutyptr = duty2;
			break;
		case 0xC0:
			CH2.dutyptr = duty3;
			break;
		default:
			CH2.dutyptr = duty0;
			break;
		}
		CH2.dutypos = 0;
		CH2.dutytimer = (2048 - CH2.frequency) * 4;
		break;
	case 0xFF17:
		NR22 = data;
		CH2.volume = (data >> 4) & 0x0F;
		CH2.envelopeup = data & 0x08;
		CH2.envelopeperiod = data & 0x07;
		CH2.envelopetimer = CH2.envelopeperiod;
		break;
	case 0xFF18:
		NR23 = data;
		CH2.frequency = (((uint16_t)NR24 << 8) | data) & 0x7FF;
		break;
	case 0xFF19:
		NR24 = data;
		CH2.frequency = (((uint16_t)data << 8) | NR23) & 0x7FF;
		CH2.lengthenable = data & 0x40;
		// Trigger event
		if (data & 0x80)
		{
			CH2.init = true;
			if (CH2.length == 0)
				CH2.length = 64;
			CH2.envelopetimer = CH2.envelopeperiod;
			CH2.volume = (NR24 >> 4) & 0x0F;
		}
		break;
	case 0xFF1A:
		NR30 = data;
		break;
	case 0xFF1B:
		NR31 = data;
		break;
	case 0xFF1C:
		NR32 = data;
		break;
	case 0xFF1D:
		NR33 = data;
		break;
	case 0xFF1E:
		NR34 = data;
		break;
	case 0xFF20:
		NR41 = data;
		break;
	case 0xFF21:
		NR42 = data;
		break;
	case 0xFF22:
		NR43 = data;
		break;
	case 0xFF23:
		NR44 = data;
		break;
	case 0xFF24:
		NR50 = data;
		break;
	case 0xFF25:
		NR51 = data;
		break;
	case 0xFF26:
		NR52 = data;
		if (NR52 & 0x80)
		{
			NR10 = 0x00;
			NR11 = 0x00;
			NR12 = 0x00;
			NR13 = 0x00;
			NR14 = 0x00;
			NR21 = 0x00;
			NR22 = 0x00;
			NR23 = 0x00;
			NR24 = 0x00;
			NR30 = 0x00;
			NR31 = 0x00;
			NR32 = 0x00;
			NR33 = 0x00;
			NR34 = 0x00;
			NR41 = 0x00;
			NR42 = 0x00;
			NR43 = 0x00;
			NR44 = 0x00;
		}
		break;
	default:
		if ((addr > 0xFF2F) && (addr < 0xFF40))
		{
			waveRAM[addr & 0x000F] = data;
		}
		else
			status = ADDRESS_OUT_OF_RANGE;
		break;
	}

	return status;
}

int8_t APU::ReadRegister(uint16_t addr, uint8_t *data)
{
	int8_t status = SUCCESS;

	switch (addr)
	{
	case 0xFF10:
		*data = (NR10 | 0x80);
		break;
	case 0xFF11:
		*data = (NR11 | 0x3F);
		break;
	case 0xFF12:
		*data = NR12;
		break;
	case 0xFF13: // NR-13 - Write-only
		*data = 0xFF;
		break;
	case 0xFF14:
		*data = (NR14 | 0xBF);
		break;
	case 0xFF15: // Unused
		*data = 0xFF;
		break;
	case 0xFF16:
		*data = (NR21 | 0x3F);
		break;
	case 0xFF17:
		*data = NR22;
		break;
	case 0xFF18: // NR23 - Write-only
		*data = 0xFF;
		break;
	case 0xFF19:
		*data = (NR24 | 0xBF);
		break;
	case 0xFF1A:
		*data = (NR30 | 0x7F);
		break;
	case 0xFF1B:
		*data = NR31; // might be write-only?
		break;
	case 0xFF1C:
		*data = (NR32 | 0x9F);
		break;
	case 0xFF1D: // NR33 - Write-only
		*data = 0xFF;
		break;
	case 0xFF1E:
		*data = (NR34 | 0xBF);
		break;
	case 0xFF1F: // Unused
		*data = 0xFF;
		break;
	case 0xFF20:
		*data = NR41; // might be write-only?
		break;
	case 0xFF21:
		*data = NR42;
		break;
	case 0xFF22:
		*data = NR43;
		break;
	case 0xFF23:
		*data = (NR44 | 0xBF);
		break;
	case 0xFF24:
		*data = NR50;
		break;
	case 0xFF25:
		*data = NR51;
		break;
	case 0xFF26:
		*data = NR52;
		break;
	default:
		if ((addr > 0xFF2F) && (addr < 0xFF40))
		{
			*data = waveRAM[addr & 0x000F];
		}
		else
			*data = 0xFF;
		break;
	}

	return status;
}

int8_t APU::PlaySound(int8_t sample, bool init)
{
	// Initialize audio
	static SDL_AudioSpec obtained;
	static SDL_AudioDeviceID dev;
	static uint16_t counter = 0; // DEBUG
	if (init)
	{
		SDL_AudioSpec spec;
		spec.freq = 32768*8;// 131072 * 8;  // max sound frequency is 131072Hz, 8 samples per period (could make this lower - 32768*8? 65536*8?)
		spec.format = AUDIO_S8;
		spec.channels = 1;
		// ~20ms, 440Hz wave requires ~100 samples per period, we want an intergral number of periods to keep the sound smooth
		// freq/50 gives the number of samples in ~20ms - divide by 100, round, and multiple by 100 rounds to the nearest 100
		//spec.samples = 100;// round((spec.freq / 50.0) / 100.0) * 100;
		spec.samples = 1; //spec.freq / 50;
		spec.callback = NULL;
		dev = SDL_OpenAudioDevice(NULL, 0, &spec, &obtained, 0);
		SDL_PauseAudioDevice(dev, 0); // unpause
		return SUCCESS;
	}
	
	//// DEBUG
	// Create square wave - array size is the number of samples per frame in the audiospec
	//char *squareWave = (char *)malloc(obtained.samples);
	//for (int i = 0; i < obtained.samples; i++)
	//	squareWave[i] = ((i % 100 < 50) ? 25 : -25);
	
	//SDL_QueueAudio(dev, squareWave, obtained.samples);
	////

	//// DEBUG 2
	//int8_t temp = ((counter % 596 < 298) ? 25 : -25);
	//counter++;
	//if (counter == 596)
	//	counter = 0;
	//SDL_QueueAudio(dev, &temp, 1);
	////

	SDL_QueueAudio(dev, &sample, 1);

	return SUCCESS;
}

int8_t APU::UpdateAPU(uint8_t cycles)
{
	static uint8_t totalcycles = 0;
	int8_t sample = 0;

	totalcycles += cycles;

	while (totalcycles > 15)
	{
		// Mix channels
		sample = GetSquareChannelSample(16);
		sample += GetWaveChannelSample();
		sample += GetNoiseChannelSample();

		PlaySound(sample);

		totalcycles -= 16;
	}

	return SUCCESS;
}

int8_t APU::GetSquareChannelSample(uint8_t cycles)
{
	static uint16_t lengthcycles = 0;
	static uint32_t volumecycles1 = 0;
	static uint32_t volumecycles2 = 0;
	static uint16_t sweepcycles = 0;
	uint32_t *volumecycles;
	bool lengthrollover = false;
	channel *mychan;
	int8_t tempsample = 0;
	int8_t sample = 0;

	// DEBUG
	//if (cycles == 16)
	//{
	//	if (lengthcycles < 298)
	//	{
	//		lengthcycles++;
	//		return 15;
	//	}
	//	else if (lengthcycles < 596)
	//	{
	//		lengthcycles++;
	//		if (lengthcycles == 596)
	//			lengthcycles = 0;
	//		return -15;
	//	}
	//}

	lengthcycles += cycles;
	if (lengthcycles > 16383)
	{
		lengthcycles -= 16384;
		lengthrollover = true;
	}

	for (int chan = 1; chan < 3; chan++)
	{
		tempsample = 0;

		if (chan == 1)
		{
			mychan = &CH1;
			volumecycles = &volumecycles1;
		}
		else
		{
			mychan = &CH2;
			volumecycles = &volumecycles2;
		}

		if (!mychan->init)
		{
			*volumecycles = 0;
			continue;
		}

		// Frequency sweep (CH1 only)
		if ((chan == 1) && (mychan->sweepenable) && (mychan->sweepperiod > 0))
		{
			sweepcycles += cycles;
			if (sweepcycles > 32767)
			{
				sweepcycles -= 32768;
				mychan->sweeptimer -= 1;
				if (mychan->sweeptimer == 0)
				{
					uint16_t temp = mychan->shadowreg;
					mychan->sweeptimer = mychan->sweepperiod;
					temp = temp >> mychan->sweepshift;
					if (mychan->sweepdown)
						temp = ~temp;
					temp += mychan->shadowreg;
					if ((temp > 2047) || (mychan->sweepshift == 0))
						mychan->sweepenable = false;
					else
					{
						mychan->shadowreg = temp;
						NR13 = mychan->shadowreg & 0x00FF;
						NR14 &= 0xF8;
						NR14 |= ((mychan->shadowreg >> 8) & 0x0007);
						mychan->frequency = mychan->shadowreg;
						mychan->dutytimer = ((2048 - mychan->frequency) * 4); // not sure if this is correct
						// Extra overflow check
						temp = temp >> mychan->sweepshift;
						if (mychan->sweepdown)
							temp = ~temp;
						temp += mychan->shadowreg;
						if (temp > 2047)
							mychan->sweepenable = false;
					}
				}
			}
		}

		// Get position in duty cycle
		mychan->dutytimer -= cycles;
		// Check for rollover - max dutytimer is 2048*4=8196, a higher number indicates a rollover
		// Subtract 1 to "force" a rollover at dutytimer=0
		if (mychan->dutytimer - 1 > 8196)
		{
			// Reset dutytimer, subtract off extra cycles beyond rollover
			mychan->dutytimer = ((2048 - mychan->frequency) * 4) - (0xFFFF - mychan->dutytimer - 1);
			// Increment dutypos, reset at 8
			mychan->dutypos += 1;
			mychan->dutypos &= 0x07;
		}
		tempsample = *(mychan->dutyptr + mychan->dutypos);

		// Update length
		if ((mychan->length != 0) && (mychan->lengthenable)) // NOTE: If length is 0, if channel is init'd by setting bit 7 of NRx4, length is maxed out
		{
			if (lengthrollover)
			{
				mychan->length -= 1;
				if (mychan->length == 0)
				{
					mychan->init = false;
					(chan == 1) ? (NR52 &= 0xFE) : (NR52 &= 0xFD);
					(chan == 1) ? (NR14 &= 0x7F) : (NR24 &= 0x7F);
				}
			}
		}
		if ((mychan->lengthenable) && (mychan->length == 0))
			continue; // is it ok to skip the volume envelope?

		// Get current volume
		if (mychan->envelopeperiod > 0)
		{
			*volumecycles += cycles;
			if (*volumecycles > 65535)
			{
				*volumecycles -= 65536;
				mychan->envelopetimer -= 1;
				if (mychan->envelopetimer == 0)
				{
					if ((mychan->volume < 15) && (mychan->volume > 0))
						(mychan->envelopeup) ? (mychan->volume += 1) : (mychan->volume -= 1);
					mychan->envelopetimer = mychan->envelopeperiod;
				}
			}
		}
		else
			*volumecycles = 0;
		tempsample *= mychan->volume;
		sample += tempsample;
	}

	// scale sample up for SDL
	return sample * 2;
}

int8_t APU::GetNoiseChannelSample()
{
	return 0;
}

int8_t APU::GetWaveChannelSample()
{
	return 0;
}

// Frequency ranges from 64Hz to 131072Hz - 131072 / (2048-freq)
// Frequency sweeps updated at multiples of 128Hz (32768 cycles)
// Sound lengths are multiples of 256HZ (16384 cycles) - so sound length is n*16384 cycles
// Volume envelopes updated at multiples of 64Hz (65536 cycles) - volume increased/decreased every n*65536 cycles


//Writing a value to a length counter will IMMEDIATELY start the counter counting
//down, regardless of the channel's current on/off state! This will happen ANY
//time that the length counter enable bit is on during a write.Several games,
//notably 'Mega Man 2' and 'Balloon Kid' prove this.
//
//NOTE: The length counters are NEVER reloaded to their last written value!They
//	must be reloaded by hand EVERY TIME, as the values written are destroyed during
//	the countdown itself!However, Initializing the channel(bit 7 of $FF14, $FF19,
//		$FF1E, $FF23) will cause a maximum - length - possible - for - that - channel playback,
//	BUT ONLY if the current counter is at terminal count(0)!VERIFIED.
//
//	VERIFIED : If the counter is written to while the length counter enable bit(see
//		below) is OFF, the value will remain intact until the enable bit is turned on.
//	This is regardless of whether the channel is INITed then, although I'm fairly
//	sure it was intended that one would start the counter and the channel during
//	the same PAPU write.
//
//	The clocking of the counter is enabled or disabled depending on the status of
//	the 'length counter enable register' (AKA the 'consecutive' bit) in register
//	set 5, a 1 enabling the counter, a 0 disabling it.
//
//The real - time status for the length counter of a given channel can be attained
//by reading register $FF26(NR52).a 1 indicates that the length counter is
//non - 0, or that the consecutive / length counter enable bit is 0; 0 indicates that
//the length counter is 0, and subsequently that channel is off.

// Frequency Timer (1 per channel)
//		Timer's period is 4194304/frequency(Hz)
//		Decremented each cycle, reloaded with the period when 0 and outputs clock to duty cycle generator

// Duty cycle (1 per channel 1,2)
//		Timer period is ((2048-frequency(bitwise))*4
//		When timer outputs a clock, duty cycle generator outputs a value based on selected duty cycles
//		Duty 0 - 0,0,0,0,0,0,0,1
//		Duty 1 - 1,0,0,0,0,0,0,1
//		Duty 2 - 1,0,0,0,0,1,1,1
//		Duty 3 - 0,1,1,1,1,1,1,0

// Frame sequencer fires at 512Hz (every 8192 cycles)
// 8 steps to frame sequencer:
//		0) Length counter ticks
//		1) Nothing
//		2) Length counter ticks and frequency sweep unit fires
//		3) Nothing
//		4) Length counter ticks
//		5) Nothing
//		6) Length counter ticks and frequency sweep unit fires
//		7) Volume envelope counter ticks

// Length Counter (1 per channel)
//		Contains 6-bit counter and enable flag
//		Counter is loaded whenever NRx1 is written to
//		When frame sequencer clocks it, if channel is enabled by NRx4 and counter is not 0, counter is decremented
//		When counter becomes zero, the enable flag is cleared
//		If enable flag is cleared, the channel outputs a volume of 0.  Otherwise the channel will output a volume specified by other units

// Volume Envelope (1 per channel 1,2,4)
//		Contains a 4-bit(?) volume counter
//		When clocked by frame sequencer, if envelope period if not 0 then new volume is calculated
//		New volume is previous volume plus/minus 1 (per NRx2).  If new volume is between 0-15, volume is updated

// Frequency Sweep Unit (Channel 1 only)
//		Contains a timer, enable flag, and shadow register
//		During trigger event:
//			CH1 freq is copied to shadow register
//			Timer is reloaded
//			Enable flag is set if sweep period or shift are non-0, cleared otherwise
//			If sweep shift is non-0, frequency calaculation is done
//		Frequency calculation: ~((shadow reg value) >> (sweep shift)) + (shadow reg value) [negation is optional]
//		If new frequency overflows (> 2047), channel is disabled
//		When frame sequencer clocks sweep unit, if enable flag is set and sweep period if non-0, new frequency is calculated
//		and overflow checked.  If freq is < 2048 and sweep shift is non-0, freq is written to shadow reg and NR13 and NR14.  New calculation
//		and overflow check again, but second freq not written.
//		If NR13/NR14 written to during a sweep, the shadow reg is not affected so the newly written values will be blown away

// Update APU - input number of cycles from last opcode
// Duty cycle update (CH1 and 2)
//		Timer is set to 2048-freq(bit-value) * 4
//		Timer decremented every cycle
//		When timer reaches 0, sound output moved to next value in duty array and timer reloaded
//		Sound output becomes 0 or 1.0, scaled by volume envelope
// Frame sequencer update occurs at 512Hz (every 8192 cycles)
//		Cycles through 8 states
//		Length counter update: (256Hz)
//			(counter is enabled by setting bit 7 of NRx4)
//			(counter is loaded when NRx1 is written to (bits 5-0))
//			Counter is decremented whether channel is on or off
//			If counter is zero, bit 0 of NR52 is cleared and output is zero
//			If counter is non-zero, output can be fed to mixer
//		Volume envelope update: (64Hz)
//			3-bit counter is decremented
//			If counter is 0, counter is reloaded, channel volume is incremented/decremented (capped at 0 and 15)
//		Freq sweep update: (128Hz)
//			(counter is disabled if sweep time is 0)
//			3-bit counter is decremented
//			If counter is 0, counter is reloaded, shadow register value is right-shifted by amount in NR10, (bitwise negated if sweep direction is down), then added back to the shadow register value to get new freq.
//			If new freq is greater than 2047, CH1 is disabled.  Otherwise frequency is updated
//			Overflow check is run again and if "next" freq is greater tham 2047, CH1 is disabled

// Register Writes
// NRx4, bit 7 set:
//		(trigger event)
//		
// NRx1:
//		
// NRx2:
//		
// NRx3:
//		
// NRx4, bits 2-0: 
//		
// NR50:
//		
// NR51:
//		
// NR52:
//		