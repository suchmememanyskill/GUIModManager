/*
Based on
https://github.com/ITotalJustice/Gamecard-Installer-NX/blob/master/source/gfx/SDL_easy.c
*/ 

#include "baseSDL.h"
#include "shapes.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#define SDLIMGFLAGS IMG_INIT_PNG | IMG_INIT_JPG
#define WFLAGS SDL_WINDOW_SHOWN
#define RFLAGS SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC

static SDL_Window   *SDLMainWindow;
static SDL_Renderer *SDLMainRenderer;

void InitSDL(){
    TTF_Init();
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(SDLIMGFLAGS);

    SDLMainWindow = SDL_CreateWindow("mainwindow", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, WFLAGS);
    SDLMainRenderer = SDL_CreateRenderer(SDLMainWindow, -1, RFLAGS);
    SDL_SetRenderDrawBlendMode(SDLMainRenderer, SDL_BLENDMODE_BLEND);
}

void ExitSDL(){
    SDL_DestroyRenderer(SDLMainRenderer);
    SDL_DestroyWindow(SDLMainWindow);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void ClearRenderer(){
    SDL_SetRenderDrawColor(SDLMainRenderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(SDLMainRenderer);
}

void UpdateRenderer(){
    SDL_RenderPresent(SDLMainRenderer);
}

void SetTextureColorSDL(SDL_Texture *texture, SDL_Color color){
    if (!texture)
        return;
    
    SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
}

SizeInfo_t GetTextSizeSDL(TTF_Font *font, const char *text){
    SizeInfo_t info;
    TTF_SizeUTF8(font, text, &info.w, &info.h);
    return info;
}

SizeInfo_t GetTextureSize(SDL_Texture *texture){
    SizeInfo_t info;
    SDL_QueryTexture(texture, NULL, NULL, &info.w, &info.h);
    return info;
}

SDL_Texture *_ConvSurfToTexSDL(SDL_Surface *surface){
    if (!surface) return NULL;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(SDLMainRenderer, surface);
	SDL_FreeSurface(surface);
    return texture;
}

SDL_Texture *ScreenshotToTexture(){
    SDL_Surface *surface = SDL_CreateRGBSurface(0, SCREEN_W, SCREEN_H, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_RenderReadPixels(SDLMainRenderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);
    
    return _ConvSurfToTexSDL(surface);
}

SDL_Texture *LoadImageSDL(const char *path){
    return IMG_LoadTexture(SDLMainRenderer, path);
}

SDL_Texture *LoadImageMemSDL(void *data, int size){
    return IMG_LoadTexture_RW(SDLMainRenderer, SDL_RWFromMem(data, size), 1);
}

void DrawTextSDL(Text_t *text){
    SDL_Surface *surface = TTF_RenderUTF8_Blended(text->font, text->text, text->color);
    SDL_Rect pos = {text->x, text->y, surface->w, surface->h};
    SDL_Texture *texture = _ConvSurfToTexSDL(surface);
    SDL_RenderCopy(SDLMainRenderer, texture, NULL, &pos);
    SDL_DestroyTexture(texture);
}

void DrawGlyph(Glyph_t *glyph){
    SDL_Surface *surface = TTF_RenderGlyph_Blended(glyph->font, glyph->glyph, glyph->color);
    SDL_Rect pos = {glyph->x, glyph->y, surface->w, surface->h};
    SDL_Texture *t = _ConvSurfToTexSDL(surface);

    SDL_RenderCopy(SDLMainRenderer, t, NULL, &pos);
    SDL_DestroyTexture(t);
}

void DrawCenteredTextSDL(TextCentered_t *text){
    SizeInfo_t size = GetTextSizeSDL(text->text.font, text->text.text);
    Text_t newText = text->text;
    newText.x += (text->w - size.w) / 2;
    newText.y += (text->h - size.h) / 2;
    DrawTextSDL(&newText);
}

void DrawTextBoxSDL(TextCentered_t *text){
    SDL_Surface *surface = TTF_RenderUTF8_Blended_Wrapped(text->text.font, text->text.text, text->text.color, text->w);
    SDL_Rect pos = {text->text.x, text->text.y, surface->w, surface->h};
    SDL_Rect textPos = {text->text.x, text->text.y, text->w, text->h};
    SDL_Texture *t = _ConvSurfToTexSDL(surface);

    SetClipRectSDL(&textPos);
    SDL_RenderCopy(SDLMainRenderer, t, NULL, &pos);
    SDL_DestroyTexture(t);
    SetClipRectSDL(NULL);
}

void DrawImageSDL(Image_t *image){
    SDL_RenderCopy(SDLMainRenderer, image->texture, NULL, &image->pos);
}

void DrawRectSDL(Rectangle_t *rect){
    SDL_SetRenderDrawColor(SDLMainRenderer, rect->color.r, rect->color.g, rect->color.b, rect->color.a);
    rect->flood ? SDL_RenderFillRect(SDLMainRenderer, &rect->pos) : SDL_RenderDrawRect(SDLMainRenderer, &rect->pos);
}

void DrawRectOutlineSDL(RectangleOutline_t *rect){
    int x = rect->rect.pos.x, y = rect->rect.pos.y, w = rect->rect.pos.w, h = rect->rect.pos.h;

    Rectangle_t top = {{x, y, w, rect->borderSize}, rect->rect.color, rect->rect.flood};
    Rectangle_t bottom = {{x, y + h - rect->borderSize, w, rect->borderSize}, rect->rect.color, rect->rect.flood};
    Rectangle_t left = {{x, y + rect->borderSize, rect->borderSize, h - rect->borderSize * 2}, rect->rect.color, rect->rect.flood};
    Rectangle_t right = {{x + w - rect->borderSize, y + rect->borderSize, rect->borderSize, h - rect->borderSize * 2}, rect->rect.color, rect->rect.flood};

    DrawRectSDL(&top);
    DrawRectSDL(&bottom);
    DrawRectSDL(&left);
    DrawRectSDL(&right);
}

void SetClipRectSDL(const SDL_Rect *rect){
    SDL_RenderSetClipRect(SDLMainRenderer, rect);
}

/*
void DrawRectRoundedSDL(int x1, int y1, int x2, int y2, SDL_Color color, int r){
    roundedBoxRGBA(SDLMainRenderer, x1, y1, x2, y2, r, color.r, color.g, color.b, color.a);
} */