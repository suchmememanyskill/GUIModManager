#pragma once
#include "GfxLib/shapes.h"

#define COLOR_WHITE COLOR(255,255,255,255)
#define COLOR_GREY COLOR(66,66,68,255)
#define COLOR_LIGHTGREY COLOR(100,100,100,255)
#define COLOR_AQUA COLOR(0, 203, 255, 255)
#define COLOR_BLUEGREY COLOR(50, 80, 100, 255)
#define COLOR_DARKGREY COLOR(57, 57, 59, 255)
#define COLOR_DARKERGREY COLOR(42, 42, 42, 255)
#define COLOR_LESSBLUEGREY COLOR(55, 58, 62, 255)
#define COLOR_MAGENTA COLOR(170, 0, 105, 255)
#define COLOR_GREYMAGENTA COLOR(99, 55, 90, 255)
#define COLOR_DARKERRED COLOR(63, 0, 0, 255)
#define COLOR_DARKRED COLOR(100, 0, 0, 255)
#define COLOR_DARKERORANGE COLOR(114, 86, 0, 255)
#define COLOR_DARKORANGE COLOR(137, 103, 0, 255)
#define COLOR_DARKERBLUE COLOR(0, 52, 61, 255)
#define COLOR_DARKBLUE COLOR(0, 86, 99, 255)
#define COLOR_RED COLOR(255,0,0,255)
#define COLOR_BLUE COLOR(0,0,255,255)
#define COLOR_GREEN COLOR(0,255,0,255)

int PowerMenu(Context_t ctx);
ShapeLinker_t *CreateMainMenu();
int ModFolderMenu(Context_t ctx);
int InstallMod(Context_t ctx);
int ModMenuSelectionChange(Context_t ctx);