#ifndef DISPLAY_H
#define DISPLAY_H

#include <inttypes.h>
#include "audio/audio.h"

#if defined(_KS0066_16X2_8BIT) || defined(_KS0066_16X2_PCF8574)
#define _KS0066
#endif

#if defined(_KS0066)
#include "display/ks0066.h"
#define MIN_BRIGHTNESS          KS0066_MIN_BRIGHTNESS
#define MAX_BRIGHTNESS          KS0066_MAX_BRIGHTNESS
#define writeString(x)          ks0066WriteString(x)
#define displayClear()          ks0066Clear()
#elif defined(_LS020)
#include "display/ls020.h"
#define MIN_BRIGHTNESS          LS020_MIN_BRIGHTNESS
#define MAX_BRIGHTNESS          LS020_MAX_BRIGHTNESS
#define writeString(x)          ls020WriteString(x)
#define displayClear()          ls020Clear()
#else
#include "display/gdfb.h"
#define MIN_BRIGHTNESS          GD_MIN_BRIGHTNESS
#define MAX_BRIGHTNESS          GD_MAX_BRIGHTNESS
#define writeString(x)          gdWriteString(x)
#define displayClear()          gdClear()
#endif

// Backlight state
#define BACKLIGHT_ON            1
#define BACKLIGHT_OFF           0

// Radio tuning mode
#define MODE_RADIO_TUNE         1
#define MODE_RADIO_CHAN         0

// String buffer
#define STR_BUFSIZE             20

// Spectrum output mode
typedef enum {
    SP_MODE_METER = 0,
    SP_MODE_STEREO,
    SP_MODE_MIXED,

    SP_MODE_END
} SpMode;

typedef enum {
    FALL_SPEED_LOW = 0,
    FALL_SPEED_MIDDLE,
    FALL_SPEED_FAST,

    FALL_SPEED_END
} FallSpeed;

#if defined(_KS0066)
typedef enum {
    LCD_LEVELS = 0,
    LCD_BAR,
    LCD_ALARM,

    LCD_END
} LcdSymGroup;

typedef enum {
    SYM_STEREO_DEGREE = 0,
    SYM_MUTE_CROSS,
    SYM_LOUDNESS_CROSS,
    SYM_SURROUND_CROSS,
    SYM_EFFECT_3D_CROSS,
    SYM_TONE_BYPASS_CROSS,
    SYM_STEREO_MONO,

    SYM_END
} LcdUserAddSym;
#endif

void displayInit();
void displayPowerOff();
void displayUpdate();

void setDefDisplay(uint8_t value);
uint8_t getDefDisplay();

void nextRcCmd();
void switchRcCmd(uint8_t index);

void switchSpMode();
void switchFallSpeed();

void getSpectrum();

void changeBrWork(int8_t diff);
void setWorkBrightness();
void setStbyBrightness();

#ifdef _ALARM
void showAlarm();
#endif
void showBrWork();
void showRadio(uint8_t tune);
void showRcInfo();
#ifdef _TEMPCONTROL
void showTemp();
#endif
void showTime();
void showTimer(int16_t timer);
void showSpectrum();

void showEffect3d();
void showLoudness();
void showMute();
void showSurround();
void showToneBypass();

void showSndParam(sndMode mode);

#endif // DISPLAY_H
