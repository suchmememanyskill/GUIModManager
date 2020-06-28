#include "design.h"
#include "GfxLib/baseSDL.h"
#include "GfxLib/gfxutils.h"
#include "GfxLib/shapes.h"
#include "GfxLib/list.h"
#include "GfxLib/renderer.h"
#include "GfxLib/menu.h"
#include "GfxLib/font.h"
#include "payload.h"
#include "fs.h"
#include <unistd.h>
#include <switch.h>

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

int exitFunc(Context_t ctx){
    return -1;
}

int rebootRCM(Context_t ctx){
    if (R_FAILED(splInitialize())) 
        return -1;

    splSetConfig((SplConfigItem) 65001, 1);

    return -1;
}

int PowerOff(Context_t ctx){
    if (R_FAILED(spsmInitialize()))
        return -1;

    spsmShutdown(0);
    return -1;
}

int RebootToHekate(Context_t ctx){
    return RebootToPayload("/bootloader/update.bin");
}

int RebootToAtmosphere(Context_t ctx){
    return RebootToPayload("/atmosphere/reboot_payload.bin");
}

ShapeLinker_t *CreateMainMenu() { // Add functions later
    ShapeLinker_t *out = NULL;

    ShapeLinkAdd(&out, RectangleCreate(POS(0, 150, SCREEN_W, SCREEN_H - 150), COLOR_LESSBLUEGREY, 1), RectangleType);
    ShapeLinkAdd(&out, RectangleCreate(POS(0, 0, SCREEN_W, 50), COLOR_GREY, 1), RectangleType);
    ShapeLinkAdd(&out, RectangleCreate(POS(0, 50, SCREEN_W, 100), COLOR_DARKERGREY, 1), RectangleType);
    ShapeLinkAdd(&out, TextCenteredCreate(POS(0, 50, SCREEN_W, 100), "Welcome to [insert name here]", COLOR_WHITE, FONT_TEXT[FSize45]), TextCenteredType);
    ShapeLinkAdd(&out, ButtonCreate(POS(200,0,200,50), COLOR_LIGHTGREY, COLOR_MAGENTA, COLOR_WHITE, COLOR_GREYMAGENTA, 0, ButtonStyleBottomStrip, "Power", FONT_TEXT[FSize28], PowerMenu), ButtonType);

    ShapeLinker_t *diritems;
    diritems = ListFolder("/mods");

    if (diritems != NULL)
        ShapeLinkAdd(&out, ListViewCreate(POS(0, 150, SCREEN_W, SCREEN_H - 150), 50, COLOR_LESSBLUEGREY, COLOR_BLUEGREY, COLOR_AQUA, COLOR_WHITE, 0, diritems, ModFolderMenu, NULL, FONT_TEXT[FSize28]), ListViewType);
    
    ShapeLinkAdd(&out, ButtonCreate(POS(0,0,200,50), COLOR_LIGHTGREY, COLOR_AQUA, COLOR_WHITE, COLOR_BLUEGREY, 0, ButtonStyleBottomStrip, "Exit", FONT_TEXT[FSize28], exitFunc), ButtonType);
    ShapeLinkAdd(&out, GlyphCreate(9, 9, BUTTON_PLUS, COLOR_WHITE, FONT_BTN[FSize30]), GlyphType);

    if (diritems == NULL)
        ShapeLinkAdd(&out, TextCenteredCreate(POS(10, 160, SCREEN_W, SCREEN_H - 160), "No mods found!\nDoes the /mods folder exist? Does it have any mods?\n\nThe file structure is:\nsd:/mods/[nameofgame]/[nameofmod]/[Contents]/[ContentID]/..", COLOR_WHITE, FONT_TEXT[FSize28]), TextBoxType);

    return out;
}

ShapeLinker_t *CreatePowerMenu() {
    ShapeLinker_t *out = NULL;
    int payloadExists = 0;

    SDL_Texture *screenshot = ScreenshotToTexture();
    ShapeLinkAdd(&out, ImageCreate(screenshot, POS(0, 0, SCREEN_W, SCREEN_H)), ImageType);
    ShapeLinkAdd(&out, RectangleCreate(POS(0, 0, SCREEN_W, SCREEN_H), COLOR(0,0,0,130), 1), RectangleType);
    ShapeLinkAdd(&out, RectangleCreate(POS(200, 50, SCREEN_W - 400, 450), COLOR_GREY, 1), RectangleType);
    ShapeLinkAdd(&out, RectangleCreate(POS(200,0,200,50), COLOR_MAGENTA, 1), RectangleType);
    ShapeLinkAdd(&out, TextCenteredCreate(POS(200,0,200,50), "Power", COLOR_WHITE, FONT_TEXT[FSize28]), TextCenteredType);
    ShapeLinkAdd(&out, ButtonCreate(POS(200, 50, 200, 50), COLOR_DARKGREY, COLOR_MAGENTA, COLOR_WHITE, COLOR_GREYMAGENTA, 0, ButtonStyleTopStrip, "Back", FONT_TEXT[FSize28], exitFunc), ButtonType);

    payloadExists = (access("/atmosphere/reboot_payload.bin", F_OK) == -1) ? BUTTON_DISABLED : 0;
    ShapeLinkAdd(&out, ButtonCreate(POS(240, 150, 800, 50), COLOR_DARKERBLUE, COLOR_DARKERGREY, COLOR_WHITE, COLOR_DARKBLUE, payloadExists, ButtonStyleFlat, "Reboot to atmosphere/reboot_payload.bin", FONT_TEXT[FSize28], RebootToAtmosphere), ButtonType);

    payloadExists = (access("/bootloader/update.bin", F_OK) == -1) ? BUTTON_DISABLED : 0;
    ShapeLinkAdd(&out, ButtonCreate(POS(240, 150 + 80, 800, 50), COLOR_DARKERBLUE, COLOR_DARKERGREY, COLOR_WHITE, COLOR_DARKBLUE, payloadExists, ButtonStyleFlat, "Reboot to bootloader/update.bin", FONT_TEXT[FSize28], RebootToHekate), ButtonType);
    ShapeLinkAdd(&out, ButtonCreate(POS(240, 150 + 160, 800, 50), COLOR_DARKERORANGE, COLOR_DARKERGREY, COLOR_WHITE, COLOR_DARKORANGE, 0, ButtonStyleFlat, "Reboot to RCM", FONT_TEXT[FSize28], rebootRCM), ButtonType);
    ShapeLinkAdd(&out, ButtonCreate(POS(240, 150 + 240, 800, 50), COLOR_DARKERRED, COLOR_DARKERGREY, COLOR_WHITE, COLOR_DARKRED, 0, ButtonStyleFlat, "Power Off", FONT_TEXT[FSize28], PowerOff), ButtonType);

    return out;
}

ShapeLinker_t *CreateModFolderMenu(char *gameName){
    ShapeLinker_t *out = NULL;
    char *folder = CopyTextArgsUtil("/mods/%s", gameName);

    ShapeLinkAdd(&out, RectangleCreate(POS(0, 150, SCREEN_W, SCREEN_H - 150), COLOR_LESSBLUEGREY, 1), RectangleType);
    ShapeLinkAdd(&out, RectangleCreate(POS(0, 50, SCREEN_W, 100), COLOR_DARKERGREY, 1), RectangleType);
    ShapeLinkAdd(&out, TextCenteredCreate(POS(0, 50, SCREEN_W, 100), gameName, COLOR_WHITE, FONT_TEXT[FSize35]), TextCenteredType);
    ShapeLinkAdd(&out, RectangleCreate(POS(0, 0, SCREEN_W, 50), COLOR_GREY, 1), RectangleType);  
    ShapeLinkAdd(&out, ButtonCreate(POS(200,0,200,50), COLOR_LIGHTGREY, COLOR_MAGENTA, COLOR_WHITE, COLOR_GREYMAGENTA, 0, ButtonStyleBottomStrip, "Power", FONT_TEXT[FSize28], PowerMenu), ButtonType);
    
    ShapeLinker_t *diritems;
    diritems = ListFolder(folder);

    if (diritems != NULL){
        ShapeLinkAdd(&out, ListViewCreate(POS(0, 150, SCREEN_W / 2, SCREEN_H - 150), 50, COLOR_LESSBLUEGREY, COLOR_BLUEGREY, COLOR_BLUEGREY, COLOR_WHITE, 0, diritems, NULL, NULL, FONT_TEXT[FSize28]), ListViewType);
        ShapeLinkAdd(&out, RectangleCreate(POS(SCREEN_W / 2, 150, SCREEN_W / 2, SCREEN_H - 150), COLOR_BLUEGREY, 1), RectangleType);
        ShapeLinkAdd(&out, TextCenteredCreate(POS(SCREEN_W / 2 + 10, 160, SCREEN_W / 2 - 20, SCREEN_H - 260), "No description...", COLOR_WHITE, FONT_TEXT[FSize28]), TextBoxType);
        ShapeLinkAdd(&out, ButtonCreate(POS(SCREEN_W / 2 + 10, SCREEN_H - 85, SCREEN_W / 2 - 20, 75), COLOR_LIGHTGREY, COLOR_AQUA, COLOR_WHITE, COLOR_DARKERGREY, 0, ButtonStyleBottomStrip, "Enable Mod", FONT_TEXT[FSize35], NULL), ButtonType);
    }
    
    ShapeLinkAdd(&out, ButtonCreate(POS(0,0,200,50), COLOR_LIGHTGREY, COLOR_AQUA, COLOR_WHITE, COLOR_BLUEGREY, 0, ButtonStyleBottomStrip, "Back", FONT_TEXT[FSize28], exitFunc), ButtonType);

    if (diritems == NULL){
        char *emptymessage = CopyTextArgsUtil("This is an empty folder :(\nPuts some mods in me!\n\nPath: %s", folder);
        ShapeLinkAdd(&out, TextCenteredCreate(POS(10, 160, SCREEN_W, SCREEN_H - 160), emptymessage, COLOR_WHITE, FONT_TEXT[FSize28]), TextBoxType);
        free(emptymessage);
    }

    free(folder);
    return out;
}

int PowerMenu(Context_t ctx){
    ShapeLinker_t *menu = CreatePowerMenu();

    Context_t newCtx = {5, OriginButtonPress, NULL, NULL, 0};
    while (newCtx.origin < OriginFunction){
        newCtx = MakeMenu(menu, newCtx.offset);
        if (newCtx.kDown & KEY_B)
            break;
    }

    ShapeLinkDispose(&menu);

    return 0;
}

int ModFolderMenu(Context_t ctx){
    ListView_t *lv = CAST(ListView_t, ctx.item->item);
    char *selection = CAST(char, ShapeLinkOffset(lv->text, lv->highlight)->item);
    ShapeLinker_t *menu = CreateModFolderMenu(selection);

    Context_t newCtx = {5, OriginButtonPress, NULL, NULL, 0};
    while (newCtx.origin < OriginFunction){
        newCtx = MakeMenu(menu, newCtx.offset);
        if (newCtx.kDown & KEY_B)
            break;
    }

    ShapeLinkDispose(&menu);

    return 0;
}