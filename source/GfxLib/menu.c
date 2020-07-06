#include "menu.h"
#include "baseSDL.h"
#include "list.h"
#include "shapes.h"
#include "gfxutils.h"
#include "renderer.h"
#include <switch.h>
#include <math.h>

enum {
    DirectionUp = 1,
    DirectionRight,
    DirectionDown,
    DirectionLeft
};

int _JumpToClosestBox(ShapeLinker_t *list, int direction, ShapeLinker_t *src, int currentSelectionIndex){
    int closestSelection = -1, currentScore = 5000, offset = 0;
    int srcx = 0, srcy = 0, dstx = 0, dsty = 0, weight;

    // Change this later to support more than just buttons
    if (src->type == ButtonType){
        Button_t *srcbtn = CAST(Button_t, src->item);
        srcx = srcbtn->pos.x + srcbtn->pos.w / 2;
        srcy = srcbtn->pos.y + srcbtn->pos.h / 2;
    }
    else if (src->type == ListViewType){
        ListView_t *srclv = CAST(ListView_t, src->item);
        srcx = srclv->pos.x + srclv->pos.w / 2;
        srcy = srclv->pos.y + srclv->pos.h / 2;
    }
    else {
        return -1;
    }

    for (ShapeLinker_t *iter = list; iter != NULL; iter = iter->next, offset++){
        if (iter->type < ListViewType)
            continue;

        if (currentSelectionIndex == offset)
            continue;

        // Change this later to support more than just buttons
        if (iter->type == ButtonType){
            Button_t *dstbtn = CAST(Button_t, iter->item);
            dstx = dstbtn->pos.x + dstbtn->pos.w / 2;
            dsty = dstbtn->pos.y + dstbtn->pos.h / 2;

            if (dstbtn->options & BUTTON_DISABLED)
                continue;
        }
        else if (iter->type == ListViewType){
            ListView_t *dstlv = CAST(ListView_t, iter->item);
            dstx = dstlv->pos.x + dstlv->pos.w / 2;
            dsty = dstlv->pos.y + dstlv->pos.h / 2;

            if (dstlv-> options & (LIST_DISABLED | LIST_AUTO))
                continue;
        }
        else {
            return -1;
        }
        weight = 0;
                
        if ((direction == DirectionDown && srcy < dsty) || (direction == DirectionUp && srcy > dsty)){
            weight += abs(srcy - dsty);
            weight += abs(srcx - dstx) * 5;
        }
        else if ((direction == DirectionLeft && srcx > dstx) || (direction == DirectionRight && srcx < dstx)){
            weight += abs(srcy - dsty) * 5;
            weight += abs(srcx - dstx);
        }

        if (weight < currentScore && weight){
            currentScore = weight;
            closestSelection = offset;
        }
                
    }

    return closestSelection;
}

int CheckTouchCollision(ShapeLinker_t *list){
    int offset = 0, touchX, touchY;
    touchPosition touch;

    if (hidTouchCount() <= 0)
        return -1;

    hidTouchRead(&touch, 0);

    touchX = touch.px; //+ (touch.dx / 2);
    touchY = touch.py; //+ (touch.dy / 2);

    for (ShapeLinker_t *iter = list; iter != NULL; iter = iter->next, offset++){
        if (iter->type < ListViewType)
            continue;

        if (iter->type == ButtonType){
            Button_t *btn = (Button_t*)iter->item;
            if (btn->options & BUTTON_DISABLED)
                continue;

            if (btn->pos.x < touchX && btn->pos.x + btn->pos.w > touchX && btn->pos.y < touchY && btn->pos.y + btn->pos.h > touchY){
                btn->options |= BUTTON_PRESSED;
                return offset;
            }
            else {
                SETBIT(btn->options, BUTTON_PRESSED, 0);
            }
        }
        else if (iter->type == ListViewType){
            ListView_t *lv = (ListView_t*)iter->item;
            if (lv->options & LIST_DISABLED)
                continue;

            if (lv->pos.x < touchX && lv->pos.x + lv->pos.w > touchX && lv->pos.y < touchY && lv->pos.y + lv->pos.h > touchY){
                int max = ShapeLinkCount(lv->text), pressedEntry = (touchY - lv->pos.y + lv->offset) / lv->entrySize;
                //lv->offset = 0;

                if (pressedEntry >= max)
                    continue;

                lv->highlight = pressedEntry;

                lv->options |= LIST_PRESSED | LIST_SELECTED;
                return offset;
            }
            else {
                SETBIT(lv->options, (LIST_SELECTED | LIST_PRESSED), 0);
            }
        }
    }

    return -1;
}

int RunSelection(Context_t *ctx){
    if (ctx->selected->type == ButtonType){
        Button_t *btn = ctx->selected->item;
        SETBIT(btn->options, BUTTON_PRESSED, 0);
        if (btn->function != NULL)
            return btn->function(ctx);
    }
    else if (ctx->selected->type == ListViewType){
        ListView_t *lv = ctx->selected->item;
        SETBIT(lv->options, LIST_PRESSED, 0);
        if (lv->function != NULL)
            return lv->function(ctx);
    }

    return 0;
}

void ActivateSelection(Context_t *ctx){
    if (ctx->selected->type == ButtonType){
        ((Button_t*)ctx->selected->item)->options |= BUTTON_PRESSED;
    }
    else if (ctx->selected->type == ListViewType){
        ((ListView_t*)ctx->selected->item)->options |= LIST_PRESSED;
    }
}

void SelectSelection(Context_t *ctx){
    if (ctx->selected->type == ButtonType){
        ((Button_t*)ctx->selected->item)->options |= BUTTON_HIGHLIGHT;
    }
    else if (ctx->selected->type == ListViewType){
        ((ListView_t*)ctx->selected->item)->options |= LIST_SELECTED;
    }
}

int menuRun = 1;

Context_t MakeMenu(ShapeLinker_t *in, func_ptr buttonHandler){
    int selectionMade = 0, touchSelection = -1, timer = 0, timeOfTimer = 21;
    Context_t ctx = {0, 0, NULL, in, 0,0,0};
    
    for (ShapeLinker_t *iter = in; iter != NULL; iter = iter->next){
        if (iter->type >= ListViewType){
            ctx.selected = iter;
            break;
        }
        ctx.curOffset++;
    }

    if (ctx.selected == NULL)
        return (Context_t){0};
    
    SelectSelection(&ctx);

    while (menuRun){
        hidScanInput();
        ctx.kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        ctx.kUp = hidKeysUp(CONTROLLER_P1_AUTO);
        ctx.kHeld = hidKeysHeld(CONTROLLER_P1_AUTO);
        
        if (timer > 0){
            timer--;
        }

        if (ctx.kDown & KEY_A){
            ActivateSelection(&ctx);
            selectionMade = 1;
        }

        else if (ctx.kUp & KEY_A && selectionMade){
            selectionMade = 0;
            ctx.origin = OriginFunction;

            if (RunSelection(&ctx) < 0)
                return ctx; 
        }

        else if (ctx.kDown & KEY_PLUS){
            menuRun = 0;
            ctx.origin = OriginPlus;
            return ctx;
        }

        else if (ctx.kUp & KEY_TOUCH && touchSelection >= 0){

            if (ctx.selected->type == ButtonType){
                SETBIT(((Button_t*)ctx.selected->item)->options, BUTTON_HIGHLIGHT, 0);
            }
            else if (ctx.selected->type == ListViewType){
                SETBIT(((ListView_t*)ctx.selected->item)->options, LIST_SELECTED, 0);
            }

            ctx.curOffset = touchSelection;
            ctx.origin = OriginFunction;
            ctx.selected = ShapeLinkOffset(ctx.all, ctx.curOffset);
            touchSelection = -1;

            if (RunSelection(&ctx) < 0)
                return ctx; 
        }

        else if (ctx.kHeld & KEY_TOUCH){
            touchSelection = CheckTouchCollision(ctx.all);
        }

        else if (ctx.kHeld & (KEY_LSTICK_DOWN | KEY_LSTICK_LEFT | KEY_LSTICK_RIGHT | KEY_LSTICK_UP | KEY_DDOWN | KEY_DLEFT | KEY_DRIGHT | KEY_DUP | KEY_RSTICK_DOWN | KEY_RSTICK_LEFT | KEY_RSTICK_RIGHT | KEY_RSTICK_UP)){            
            int direction = 0, res = -1;
            
            if (timer > 1 && ctx.kHeld & (KEY_RSTICK_DOWN | KEY_RSTICK_LEFT | KEY_RSTICK_RIGHT | KEY_RSTICK_UP))
                timer = 1;

            if (timer == 0){
                timer = timeOfTimer;
                if (timeOfTimer > 6)
                    timeOfTimer -= 6;

                if (ctx.kHeld & (KEY_LSTICK_DOWN | KEY_DDOWN | KEY_RSTICK_DOWN))
                    direction = DirectionDown;
                else if (ctx.kHeld & (KEY_LSTICK_UP | KEY_DUP | KEY_RSTICK_UP))
                    direction = DirectionUp;
                else if (ctx.kHeld & (KEY_LSTICK_LEFT | KEY_DLEFT | KEY_RSTICK_LEFT))
                    direction = DirectionLeft;
                else if (ctx.kHeld & (KEY_LSTICK_RIGHT | KEY_DRIGHT | KEY_RSTICK_RIGHT))
                    direction = DirectionRight;

                if (ctx.selected->type == ListViewType){
                    ListView_t *lv = (ListView_t*)ctx.selected->item;

                    lv->options |= LIST_SELECTED;

                    int max = ShapeLinkCount(lv->text);
                    if (lv->highlight < max - 1 && direction == DirectionDown){
                        lv->highlight++;
                    }
                    else if (lv->highlight > 0 && direction == DirectionUp){
                        lv->highlight--;
                    }
                    else {
                        res = _JumpToClosestBox(ctx.all, direction, ctx.selected, ctx.curOffset);
                    }
                }
                else {
                    ((Button_t*)ctx.selected->item)->options |= BUTTON_HIGHLIGHT;
                    res = _JumpToClosestBox(ctx.all, direction, ctx.selected, ctx.curOffset);
                }

                if (res >= 0){
                    selectionMade = 0;

                    if (ctx.selected->type == ButtonType){
                        SETBIT(((Button_t*)ctx.selected->item)->options, (BUTTON_HIGHLIGHT | BUTTON_PRESSED), 0);
                    }
                    else if (ctx.selected->type == ListViewType){
                        SETBIT(((ListView_t*)ctx.selected->item)->options, (LIST_PRESSED | LIST_SELECTED), 0);
                    }

                    ctx.curOffset = res;
                    ctx.selected = ShapeLinkOffset(ctx.all, ctx.curOffset);

                    SelectSelection(&ctx);
                }
            }
        }
        else if (ctx.kDown){
            if (buttonHandler != NULL){
                ctx.origin = OriginButtonPress;
                if (buttonHandler(&ctx) < 0)
                    return ctx;
            }
        }
        else if (!(ctx.kHeld & (KEY_LSTICK_DOWN | KEY_LSTICK_LEFT | KEY_LSTICK_RIGHT | KEY_LSTICK_UP | KEY_DDOWN | KEY_DLEFT | KEY_DRIGHT | KEY_DUP | KEY_RSTICK_DOWN | KEY_RSTICK_LEFT | KEY_RSTICK_RIGHT | KEY_RSTICK_UP))){
            timer = 0;
            timeOfTimer = 21;
        }

        if (ctx.selected->type == ListViewType){
            if (((ListView_t *)ctx.selected->item)->changeSelection != NULL){
                ctx.origin = OriginFunction;
                ((ListView_t *)ctx.selected->item)->changeSelection(&ctx);
            }
                
        }

        RenderShapeLinkList(ctx.all);
    }

    ctx.origin = OriginPlus;
    return ctx;
}