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

    if (!err){
        ShapeLinker_t *mainMenu = CreateMainMenu();
    
        MakeMenu(mainMenu, NULL);

        ShapeLinkDispose(&mainMenu);
    }

    FontExit();
    ExitSDL();
    return 0;
}
