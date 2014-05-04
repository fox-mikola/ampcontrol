#include "tea5767.h"
#include "i2c.h"

#include "ks0108.h"

#include <util/delay.h>

static tea5767Ctrl ctrl;
static uint8_t HILO;

void tea5767Init(void)
{
	ctrl.high_cut = 0;
	ctrl.st_noise = 0;
	ctrl.soft_mute = 0;
	ctrl.deemph_75 = 1;
	ctrl.japan_band = 0;
	ctrl.pllref = 0;
	ctrl.xtal_freq = 1;

	HILO = 1;
}

void tea5767SetFreq(uint8_t hilo, uint32_t freq)
{
	uint8_t buf[5];
	uint16_t div;
	uint8_t i;

	_delay_ms(25);

	if (hilo)
		div = (freq + 225000) >> 13;
	else
		div = (freq - 225000) >> 13;

	buf[0] = (div >> 8) & 0x3F;
	buf[1] = div & 0xff;

	buf[2] = 0;
	if (hilo)
		buf[2] |= TEA5767_HLSI;

	buf[3] = 0;
	if (ctrl.high_cut)
		buf[3] |= TEA5767_HCC;
	if (ctrl.st_noise)
		buf[3] |= TEA5767_SNC;
	if (ctrl.soft_mute)
		buf[3] |= TEA5767_SMUTE;
	if (ctrl.japan_band)
		buf[3] |= TEA5767_BL;
	if (ctrl.xtal_freq)
		buf[3] |= TEA5767_XTAL;

	buf[4] = 0;
	if (ctrl.deemph_75)
		buf[4] |= TEA5767_DTC;
	if (ctrl.pllref)
		buf[4] |= TEA5767_PLLREF;

	I2CStart();
	I2CWriteByte(TEA5767_ADDR);
	for (i = 0; i < 5; i++) {
		I2CWriteByte(buf[i]);
	}
	I2CStop();

	_delay_ms(25);
}

void tea5767ReadStatus(uint8_t *buf)
{
	uint8_t i;

	_delay_ms(25);

	I2CStart();
	I2CWriteByte(TEA5767_ADDR + 1);
	for (i = 0; i < 5; i++)
		I2CReadByte(&buf[i], 1);
	I2CStop();

	_delay_ms(25);
}

uint8_t tea5767ADCLevel(uint8_t *buf)
{
	return (buf[3] & TEA5767_LEV_MASK) >> 4;
}


uint8_t tea5767Stereo(uint8_t *buf)
{
	return (buf[2] & TEA5767_STEREO) ? 1 : 0;
}

uint8_t tea5767Ready(uint8_t *buf)
{
	return (buf[0] & TEA5767_RF) ? 1 : 0;
}

uint8_t tea5767BlReached (uint8_t *buf)
{
	return (buf[0] & TEA5767_BLF) ? 1 : 0;
}

uint8_t tea5767HiloOptimal(uint32_t freq)
{

	int signal_high = 0;
	int signal_low = 0;
	uint8_t buf[5];

	tea5767SetFreq(1, freq + 450000);

	tea5767ReadStatus(buf);
	signal_high = tea5767ADCLevel(buf);

	tea5767SetFreq(0, freq - 450000);

	tea5767ReadStatus(buf);
	signal_low = tea5767ADCLevel(buf);

	return (signal_high < signal_low) ? 1 : 0;
}

void tea5767SetOptimalFreq (uint32_t freq)
{
	HILO = tea5767HiloOptimal(freq);
	tea5767SetFreq (HILO, freq);
}

uint32_t tea5767FreqAvail(uint8_t *buf)
{
	if (HILO == 1)
		return ((uint32_t)(((buf[0] & 0x3F) << 8) + buf[1]) << 13) - 225000;
	else
		return ((uint32_t)(((buf[0] & 0x3F) << 8) + buf[1]) << 13) + 225000;
}

uint32_t tea5767CurFreq(uint8_t *buf)
{
	return ((uint32_t)(((buf[0] & 0x3F) << 8) + buf[1]) << 13);
}

void tea5767SearchUp(uint32_t freq, uint8_t *buf)
{
	uint16_t div;
	uint8_t i;

	_delay_ms(25);

	div = (freq + 100000 + 225000) >> 13;

	buf[0] = (div >> 8) & 0x3F;
	buf[1] = div & 0xff;

	buf[2] = 0;
	buf[2] |= TEA5767_HLSI;

	buf[2] |= TEA5767_SSL_MID;
	buf[2] |= TEA5767_SUD;

	buf[3] = 0;
	if (ctrl.high_cut)
		buf[3] |= TEA5767_HCC;
	if (ctrl.st_noise)
		buf[3] |= TEA5767_SNC;
	if (ctrl.soft_mute)
		buf[3] |= TEA5767_SMUTE;
	if (ctrl.japan_band)
		buf[3] |= TEA5767_BL;
	if (ctrl.xtal_freq)
		buf[3] |= TEA5767_XTAL;

	buf[4] = 0;
	if (ctrl.deemph_75)
		buf[4] |= TEA5767_DTC;
	if (ctrl.pllref)
		buf[4] |= TEA5767_PLLREF;

	buf[0] |= TEA5767_SM;

	I2CStart();
	I2CWriteByte(TEA5767_ADDR);
	for (i = 0; i < 5; i++) {
		I2CWriteByte(buf[i]);
	}
	I2CStop();

	_delay_ms(25);
}
