#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "shapes.h"

#define SCREEN_W 1280
#define SCREEN_H 720

void ClearRenderer();
void ExitSDL();
void InitSDL();
SizeInfo_t GetTextSizeSDL(TTF_Font *font, const char *text);
void SetTextureColorSDL(SDL_Texture *texture, SDL_Color color);
void UpdateRenderer();
SizeInfo_t GetTextureSize(SDL_Texture *texture);
SDL_Texture *ScreenshotToTexture();
SDL_Texture *LoadImageSDL(const char *path);
SDL_Texture *LoadImageMemSDL(void *data, int size);
void DrawTextSDL(Text_t *text);
void DrawCenteredTextSDL(TextCentered_t *text);
void DrawImageSDL(Image_t *image);
void DrawRectSDL(Rectangle_t *rect);
void DrawRectOutlineSDL(RectangleOutline_t *rect);
void SetClipRectSDL(const SDL_Rect *rect);
void DrawGlyph(Glyph_t *glyph);
void DrawTextBoxSDL(TextCentered_t *text);
//void DrawRectRoundedSDL(int x1, int y1, int x2, int y2, SDL_Color color, int r);