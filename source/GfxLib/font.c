#include "font.h"
#include <switch.h>
#include <SDL2/SDL_ttf.h>

TTF_Font *FONT_TEXT[MAX_FONT], *FONT_BTN[MAX_FONT];

int FontInit(){
    if (plInitialize(0))
        return -1;

    PlFontData font, button;
    if (plGetSharedFontByType(&font, PlSharedFontType_Standard) || plGetSharedFontByType(&button, PlSharedFontType_NintendoExt))
        return -1;

    int font_sizes[] = { 15, 18, 20, 23, 25, 28, 30, 33, 35, 45, 48, 60, 63, 72, 170 };

    for (int i = 0; i < MAX_FONT; i++){
        FONT_TEXT[i] = TTF_OpenFontRW(SDL_RWFromMem(font.address, font.size), 1, font_sizes[i]);
        FONT_BTN[i] = TTF_OpenFontRW(SDL_RWFromMem(button.address, button.size), 1, font_sizes[i]);
    }

    return 0;
}

void FontExit(){
    for (int i = 0; i < MAX_FONT; i++){
        TTF_CloseFont(FONT_TEXT[i]);
        TTF_CloseFont(FONT_BTN[i]);
    }

    plExit();
}