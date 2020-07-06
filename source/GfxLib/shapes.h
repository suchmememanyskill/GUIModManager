#pragma once

typedef unsigned char u8;
typedef unsigned short u16;

#include <switch.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#define BUTTON_A            0xE0E0
#define BUTTON_B            0xE0E1
#define BUTTON_X            0xE0E2
#define BUTTON_Y            0xE0E3
#define BUTTON_L            0xE0E4
#define BUTTON_R            0xE0E5
#define BUTTON_ZL           0xE0E6
#define BUTTON_ZR           0xE0E7
#define BUTTON_SL           0xE0E8
#define BUTTON_SR           0xE0E9
#define BUTTON_UP           0xE0EB
#define BUTTON_DOWN         0xE0EC
#define BUTTON_LEFT         0xE0ED
#define BUTTON_RIGHT        0xE0EE
#define BUTTON_PLUS         0xE0EF
#define BUTTON_MINUS        0xE0F0

#define CAST(type, tocast) ((type *) tocast)

typedef struct {
    int w, h;
} SizeInfo_t;

enum ShapeTypes{
    RectangleType = 0,
    TextType,
    GlyphType,
    TextCenteredType,
    TextBoxType,
    ImageType,
    RectangleOutlineType,
    ProgressBarType,
    ListItemType,
    DataType, // This is an invalid type, this also won't get cleaned up. this is just that you can safely add an entry to the list and read (and find) it back later.
    ListViewType,
    ButtonType,
};

typedef struct {
    SDL_Rect pos;
    SDL_Color color;
    u8 flood;
} Rectangle_t;

typedef struct {
    int x, y;
    char *text;
    SDL_Color color;
    TTF_Font *font;
} Text_t;

typedef struct {
    int x, y;
    u16 glyph;
    SDL_Color color;
    TTF_Font *font;
} Glyph_t;

typedef struct {
    int w, h;
    Text_t text;
} TextCentered_t;

typedef struct {
    SDL_Texture *texture;
    SDL_Rect pos;
} Image_t;

typedef struct {
    Rectangle_t rect;
    int borderSize;
} RectangleOutline_t;

typedef struct ShapeLinker_s{
    u8 type;
    void *item;
    struct ShapeLinker_s *next;
} ShapeLinker_t;

enum ctxOrigin {
    OriginButtonPress = 0,
    OriginFunction,
    OriginPlus
};

typedef struct {
    int curOffset, origin;
    ShapeLinker_t *selected, *all;
    u64 kDown, kUp, kHeld;
} Context_t;

typedef struct {
    SDL_Color leftcolor, rightcolor;
    SDL_Texture *leftimg;
    char *lefttext, *righttext;
} ListItem_t;

typedef int (*func_ptr)(Context_t *ctx);

#define SETBIT(object, shift, value) ((value) ? (object |= shift) : (object &= ~shift))
#define BUTTON_DISABLED BIT(0)
#define BUTTON_PRESSED BIT(1)
#define BUTTON_HIGHLIGHT BIT(2)

enum {
    ButtonStyleFlat = 0,
    ButtonStyleBottomStrip,
    ButtonStyleTopStrip,
    ButtonStyleBorder,
};

typedef struct {
    SDL_Rect pos;
    SDL_Color primary, secondary, textcolor, highlight;
    u8 options;
    u8 style;
    char *text;
    TTF_Font *font;
    func_ptr function;
} Button_t;

#define LIST_DISABLED BIT(0)
#define LIST_AUTO BIT(1)
#define LIST_PRESSED BIT(2)
#define LIST_SELECTED BIT(3)
#define LIST_ALWAYSRENDERSELECTED BIT(4)

typedef struct {
    SDL_Rect pos;
    int entrySize;
    SDL_Color primary, selected, pressed, textcolor;
    u8 options;
    int highlight;
    int offset;
    ShapeLinker_t *text;
    func_ptr function;
    func_ptr changeSelection;
    TTF_Font *font;
} ListView_t;

enum {
    ProgressBarStyleFlat = 0,
    ProgressBarStyleBorder,
    ProgressBarStyleSize,
    ProgressBarStyleCompleteBorder
};

typedef struct {
    SDL_Rect pos;
    SDL_Color primary, secondary;
    u8 style;
    u8 percentage;
} ProgressBar_t;

#define COLOR(r, g, b, a) (SDL_Color) {r, g, b, a}
#define POS(x, y, w, h) (SDL_Rect) {x, y, w, h}

Rectangle_t *RectangleCreate(SDL_Rect pos, SDL_Color color, int flood);
RectangleOutline_t *RectangleOutlineCreate(SDL_Rect pos, SDL_Color color, int flood, int borderSize);
Image_t *ImageCreate(SDL_Texture *tex, SDL_Rect pos);
Text_t *TextCreate(int x, int y, char *text, SDL_Color color, TTF_Font *font);
TextCentered_t *TextCenteredCreate(SDL_Rect pos, char *text, SDL_Color color, TTF_Font *font);
Button_t *ButtonCreate(SDL_Rect pos, SDL_Color primary, SDL_Color secondary, SDL_Color textcolor, SDL_Color highlight, u8 options, u8 style, char *text, TTF_Font *font, func_ptr function);
void DrawButton(Button_t *button);
ListView_t *ListViewCreate(SDL_Rect pos, int entrySize, SDL_Color primary, SDL_Color selected, SDL_Color pressed, SDL_Color textcolor, u8 options, ShapeLinker_t *textList, func_ptr function, func_ptr selectionChanged, TTF_Font *font);
void DrawListView(ListView_t *listview);
void DrawProgressBar(ProgressBar_t *bar);
ProgressBar_t *ProgressBarCreate(SDL_Rect pos, SDL_Color primary, SDL_Color secondary, u8 style, u8 percentage);
Glyph_t *GlyphCreate(int x, int y, u16 glyph, SDL_Color color, TTF_Font *font);
void ProgressBarFree(ProgressBar_t *pb);
void ListViewFree(ListView_t *lv);
void ButtonFree(Button_t *btn);
void TextCenteredFree(TextCentered_t *text);
void TextFree(Text_t *text);
void ImageFree(Image_t *img);
void RectangleOutlineFree(RectangleOutline_t *Rectangle);
void RectangleFree(Rectangle_t *Rectangle);
void GlyphFree(Glyph_t *glyph);