#include "shapes.h"
#include "gfxutils.h"
#include "baseSDL.h"
#include "list.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#define STRIPSIZE 5
#define BORDERSIZE 4

Rectangle_t *RectangleCreate(SDL_Rect pos, SDL_Color color, int flood){
    Rectangle_t *out = malloc(sizeof(Rectangle_t));
    out->color = color;
    out->pos = pos;
    out->flood = flood;

    return out;
}

void RectangleFree(Rectangle_t *Rectangle){
    free(Rectangle);
}

RectangleOutline_t *RectangleOutlineCreate(SDL_Rect pos, SDL_Color color, int flood, int borderSize){
    RectangleOutline_t *out = malloc(sizeof(RectangleOutline_t));
    out->rect.color = color;
    out->rect.pos = pos;
    out->rect.flood = flood;
    out->borderSize = borderSize;

    return out;
}

void RectangleOutlineFree(RectangleOutline_t *Rectangle){
    free(Rectangle);
}

Image_t *ImageCreate(SDL_Texture *tex, SDL_Rect pos){
    Image_t *out = malloc(sizeof(Image_t));
    out->pos = pos;
    out->texture = tex;

    return out;
}

void ImageFree(Image_t *img){
    SDL_DestroyTexture(img->texture);
    free(img);
}

Text_t *TextCreate(int x, int y, char *text, SDL_Color color, TTF_Font *font){
    Text_t *out = malloc(sizeof(Text_t));
    out->x = x;
    out->y = y;
    out->color = color;
    out->text = CopyTextUtil(text);
    out->font = font;

    return out;
}

void TextFree(Text_t *text){
    free(text->text);
    free(text);
}

TextCentered_t *TextCenteredCreate(SDL_Rect pos, char *text, SDL_Color color, TTF_Font *font){
    TextCentered_t *out = malloc(sizeof(TextCentered_t));
    out->text.x = pos.x;
    out->text.y = pos.y;
    out->text.color = color;
    out->text.text = CopyTextUtil(text);
    out->text.font = font;
    out->w = pos.w;
    out->h = pos.h;

    return out;
}

void TextCenteredFree(TextCentered_t *text){
    free(text->text.text);
    free(text);
}

Button_t *ButtonCreate(SDL_Rect pos, SDL_Color primary, SDL_Color secondary, SDL_Color textcolor, SDL_Color highlight, u8 options, u8 style, char *text, TTF_Font *font, func_ptr function){
    Button_t *out = malloc(sizeof(Button_t));
    out->pos = pos;
    out->primary = primary;
    out->secondary = secondary;
    out->textcolor = textcolor;
    out->options = options;
    out->style = style;
    out->text = CopyTextUtil(text);
    out->function = function;
    out->highlight = highlight;
    out->font = font;

    return out;
}

void ButtonFree(Button_t *btn){
    free(btn->text);
    free(btn);
}

void DrawButton(Button_t *button){
    SDL_Color backcolor = (button->options & BUTTON_PRESSED) ? button->secondary : ((button->options & BUTTON_HIGHLIGHT) ? button->highlight : button->primary);
    Rectangle_t *rect = RectangleCreate(button->pos, backcolor, 1);
    TextCentered_t *text = TextCenteredCreate(button->pos, button->text, button->textcolor, button->font);

    DrawRectSDL(rect);
    DrawCenteredTextSDL(text);

    switch (button->style){
        case ButtonStyleBorder:;
            RectangleOutline_t outline = {*rect, BORDERSIZE};
            outline.rect.color = button->secondary;
            DrawRectOutlineSDL(&outline);
            break;
        case ButtonStyleBottomStrip:;
            Rectangle_t *strip = RectangleCreate(POS(button->pos.x, button->pos.y + button->pos.h - STRIPSIZE, button->pos.w, STRIPSIZE), button->secondary, 1);
            DrawRectSDL(strip);
            RectangleFree(strip);
            break;
        case ButtonStyleTopStrip:;
            Rectangle_t *stripTop = RectangleCreate(POS(button->pos.x, button->pos.y, button->pos.w, STRIPSIZE), button->secondary, 1);
            DrawRectSDL(stripTop);
            RectangleFree(stripTop);
            break;
    }   

    if (button->options & BUTTON_DISABLED){
        rect->color = COLOR(0, 0, 0, 75);
        DrawRectSDL(rect);
    }

    RectangleFree(rect);
    TextCenteredFree(text);
}

ListView_t *ListViewCreate(SDL_Rect pos, int entrySize, SDL_Color primary, SDL_Color selected, SDL_Color pressed, SDL_Color textcolor, u8 options, ShapeLinker_t *textList, func_ptr function, func_ptr selectionChanged, TTF_Font *font){
    ListView_t *out = malloc(sizeof(ListView_t));
    out->primary = primary;
    out->selected = selected;
    out->pressed = pressed;
    out->textcolor = textcolor;
    out->options = options;
    out->highlight = 0;
    out->offset = 0;
    out->text = textList;
    out->function = function;
    out->font = font;
    out->entrySize = entrySize;
    out->pos = pos;
    out->changeSelection = selectionChanged;

    return out;
}

void ListViewFree(ListView_t *lv){
    for (ShapeLinker_t *iter = lv->text; iter != NULL; iter = iter->next){
        free(iter->item);
    }
    free(lv);
}

void DrawListView(ListView_t *listview){
    if (listview->options & LIST_AUTO){
        int len = ShapeLinkCount(listview->text);
        listview->highlight = len - 1;
    }

    if (listview->highlight >= 0){
        int minView, maxView, minFrame, maxFrame;
        minFrame = listview->offset;
        maxFrame = minFrame + listview->pos.h;
        minView = listview->entrySize * listview->highlight;
        maxView = minView + listview->entrySize;

        if (minFrame > minView){
            listview->offset -= minFrame - minView;
        }
        if (maxFrame < maxView){
            listview->offset += maxView - maxFrame;
        }

        //minFrame = listview->offset, maxFrame = minFrame + listview->pos.h;
    }

    if (listview->options & LIST_AUTO)
        listview->highlight = -1;

    int currentListOffset = listview->offset / listview->entrySize, currentListOffsetRemainder = listview->offset % listview->entrySize;
    int curPixOffset = currentListOffsetRemainder * -1;

    Rectangle_t bg = {listview->pos, listview->primary, 1};
    DrawRectSDL(&bg);
    //fprintf(file, "[DLV] [%d %d %d %d] [%d %d %d]\n", listview->pos.x, listview->pos.y, listview->pos.w, listview->pos.h, listview->primary.r, listview->primary.g, listview->primary.b);
    //return;

    SetClipRectSDL(&listview->pos);
    ShapeLinker_t *link = ShapeLinkOffset(listview->text, currentListOffset);

    //fprintf(file, "[DLV] curoff: %d, curpixoffset: %d, off: %d, sz: %d\n", currentListOffset, curPixOffset, listview->offset, listview->entrySize);

    for (; link != NULL && curPixOffset < listview->pos.h; link = link->next){
        SizeInfo_t info = GetTextSizeSDL(listview->font, (char*)link->item);
        SDL_Color textColor;

        switch (link->type){
            case 0:
                textColor = listview->textcolor;
                break;
            case 1:
                textColor = COLOR(255,0,0,255);
                break;
            case 2:
                textColor = COLOR(0,255,0,255);
                break;
            case 3:
                textColor = COLOR(0,0,255,255);
                break;
        }

        Text_t text = {listview->pos.x + 5, listview->pos.y + curPixOffset + ((listview->entrySize - info.h) / 2), (char*)link->item, textColor, listview->font};

        if (currentListOffset == listview->highlight && listview->options & (LIST_SELECTED | LIST_ALWAYSRENDERSELECTED)){
            SDL_Color color = (listview->options & LIST_PRESSED) ? listview->pressed : listview->selected;

            if (listview->options & LIST_ALWAYSRENDERSELECTED && !(listview->options & LIST_SELECTED)){
                color.a = 100;
            }

            Rectangle_t high = {POS(listview->pos.x, listview->pos.y + curPixOffset, listview->pos.w, listview->entrySize), color, 1};
            DrawRectSDL(&high);
        }

        curPixOffset += listview->entrySize;
        currentListOffset++;
        DrawTextSDL(&text);
        //fprintf(file, "[DLV] [%d] Rendered %s @ [%d %d] %d\n", currentListOffset, (char*)link->item, text.x, text.y, ((listview->entrySize - info.h) / 2));
    }

    SetClipRectSDL(NULL);
}

ProgressBar_t *ProgressBarCreate(SDL_Rect pos, SDL_Color primary, SDL_Color secondary, u8 style, u8 percentage){
    ProgressBar_t *out = malloc(sizeof(ProgressBar_t));
    out->pos = pos;
    out->primary = primary;
    out->secondary = secondary;
    out->style = style;
    out->percentage = percentage;

    return out;
}

void ProgressBarFree(ProgressBar_t *pb){
    free(pb);
}

void DrawProgressBar(ProgressBar_t *bar){
    int wfilled = bar->pos.w * bar->percentage / 100, wempty = bar->pos.w - wfilled;
    SDL_Rect filled = POS(bar->pos.x, bar->pos.y, wfilled, bar->pos.h);
    SDL_Rect empty = POS(bar->pos.x + wfilled, bar->pos.y, wempty, bar->pos.h);
    Rectangle_t filledRect = {filled, bar->primary, 1};
    Rectangle_t emptyRect = {empty, bar->secondary, 1};
    RectangleOutline_t emptyRectOutline = {{bar->pos, bar->secondary, 1}, BORDERSIZE};

    switch (bar->style){
        case ProgressBarStyleFlat:;
            DrawRectSDL(&filledRect);
            DrawRectSDL(&emptyRect);
            break;
        case ProgressBarStyleBorder:;
            DrawRectOutlineSDL(&emptyRectOutline);
            DrawRectSDL(&filledRect);
            break;
        case ProgressBarStyleCompleteBorder:;
            DrawRectSDL(&filledRect);
            DrawRectOutlineSDL(&emptyRectOutline);
            break;
        case ProgressBarStyleSize:;
            emptyRect.pos.y += emptyRect.pos.h - STRIPSIZE;
            emptyRect.pos.h = STRIPSIZE;
            DrawRectSDL(&filledRect);
            DrawRectSDL(&emptyRect);
            break;
    }
}

Glyph_t *GlyphCreate(int x, int y, u16 glyph, SDL_Color color, TTF_Font *font){
    Glyph_t *out = malloc(sizeof(Glyph_t));
    out->x = x;
    out->y = y;
    out->glyph = glyph;
    out->color = color;
    out->font = font;

    return out;
}

void GlyphFree(Glyph_t *glyph){
    free(glyph);
}