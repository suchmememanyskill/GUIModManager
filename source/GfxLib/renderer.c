#include "renderer.h"
#include "baseSDL.h"
#include "gfxutils.h"
#include "shapes.h"

void RenderShapeLinkList(ShapeLinker_t *list){
    ClearRenderer();
    for (ShapeLinker_t *iter = list; iter != NULL; iter = iter->next){
        switch (iter->type){
            case RectangleType:
                DrawRectSDL((Rectangle_t*)iter->item);
                break;
            case TextType:
                DrawTextSDL((Text_t*)iter->item);
                break;
            case TextCenteredType:
                DrawCenteredTextSDL((TextCentered_t*)iter->item);
                break;
            case ImageType:
                DrawImageSDL((Image_t*)iter->item);
                break;
            case RectangleOutlineType:
                DrawRectOutlineSDL((RectangleOutline_t*)iter->item);
                break;
            case ButtonType:
                DrawButton((Button_t*)iter->item);
                break;
            case ListViewType:
                DrawListView((ListView_t*)iter->item);
                break;
            case ProgressBarType:
                DrawProgressBar((ProgressBar_t*)iter->item);
                break;
            case GlyphType:
                DrawGlyph((Glyph_t*)iter->item);
                break;
            case TextBoxType:
                DrawTextBoxSDL((TextCentered_t*)iter->item);
                break;
        }
    }

    UpdateRenderer();
}