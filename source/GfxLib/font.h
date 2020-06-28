#pragma once
#include <SDL2/SDL_ttf.h>

#define MAX_FONT 15

extern TTF_Font *FONT_TEXT[MAX_FONT];
extern TTF_Font *FONT_BTN[MAX_FONT];

typedef enum
{
    FSize15,
    FSize18,
    FSize20,
    FSize23,
    FSize25,
    FSize28,
    FSize30,
    FSize33,
    FSize35,
    FSize45,
    FSize48,
    FSize60,
    FSize63,
    FSize72,
    FSize170,
} FSize;

int FontInit();
void FontExit();