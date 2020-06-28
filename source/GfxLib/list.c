#include "list.h"
#include "shapes.h"
#include <stdlib.h>

int ShapeLinkAdd(ShapeLinker_t **start, void *item, u8 type){
    ShapeLinker_t *add = malloc(sizeof(ShapeLinker_t));
    add->item = item;
    add->type = type;
    add->next = NULL;

    if (*start == NULL){
        *start = add;
        return 0;
    }
    
    int offset = 0;

    ShapeLinker_t *iter = *start;
    while (iter != NULL){
        if (iter->next == NULL){
            iter->next = add;
            return offset;
        }

        iter = iter->next;
        offset++;
    }

    return -1;
}

ShapeLinker_t *ShapeLinkOffset(ShapeLinker_t *start, int offset){
    ShapeLinker_t *iter = start;

    for (int i = 0; i < offset && iter != NULL; i++){
        iter = iter->next;
    }

    return iter;
}

void ShapeLinkDispose(ShapeLinker_t **start){
    for (ShapeLinker_t *iter = *start; iter != NULL;){
        ShapeLinker_t *iter_next = iter->next;

        switch (iter->type){
            case RectangleType:
                RectangleFree((Rectangle_t*)iter->item);
                break;
            case TextType:
                TextFree((Text_t*)iter->item);
                break;
            case TextBoxType:
            case TextCenteredType:
                TextCenteredFree((TextCentered_t*)iter->item);
                break;
            case ImageType:
                ImageFree((Image_t*)iter->item);
                break;
            case RectangleOutlineType:
                RectangleOutlineFree((RectangleOutline_t*)iter->item);
                break;
            case ButtonType:
                ButtonFree((Button_t*)iter->item);
                break;
            case ListViewType:
                ListViewFree((ListView_t*)iter->item);
                break;
            case ProgressBarType:
                ProgressBarFree((ProgressBar_t*)iter->item);
                break;
            case GlyphType:
                GlyphFree((Glyph_t*)iter->item);
                break;
        }

        free(iter);
        iter = iter_next;
    }

    *start = NULL;
}

int ShapeLinkCount(ShapeLinker_t *start){
    int count = 0;

    for (ShapeLinker_t *iter = start; iter != NULL; iter = iter->next)
        count++;

    return count;
}