// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include the main libnx system header, for Switch development
#include <switch.h>
#include "GfxLib/baseSDL.h"
#include "GfxLib/gfxutils.h"
#include "GfxLib/shapes.h"
#include "GfxLib/list.h"
#include "GfxLib/renderer.h"
#include "GfxLib/menu.h"
#include "GfxLib/font.h"
#include "design.h"
#include "fs.h"


int main(int argc, char* argv[])
{
    int err = 0;

    InitSDL();
    err -= FontInit();
    err -= SetCfwFolder();

    if (err){
        /*
        ClearRenderer();
        DrawTextSDL(TextCreate(10, 10, "Init failed! Press + to exit", COLOR(255,255,255,255), FONT_TEXT[FSize23]));
        UpdateRenderer();

        u64 kDown = 0;
        while (!(kDown & KEY_PLUS)){
            hidScanInput();
            kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        }
        */

       // Just exit the app lol
    }
    else {
        ShapeLinker_t *mainMenu = CreateMainMenu();
    
        Context_t ctx = {5, OriginButtonPress, NULL, NULL, 0};
        while (ctx.origin < OriginFunction){
            ctx = MakeMenu(mainMenu, ctx.offset);
        }

        ShapeLinkDispose(&mainMenu);
    }

    FontExit();
    ExitSDL();
    return 0;
}
