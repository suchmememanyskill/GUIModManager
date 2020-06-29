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
#include <stdio.h>

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
        ShapeLinkAdd(&out, ButtonCreate(POS(SCREEN_W / 2 + 10, SCREEN_H - 85, SCREEN_W / 2 - 20, 75), COLOR_LIGHTGREY, COLOR_AQUA, COLOR_WHITE, COLOR_DARKERGREY, 0, ButtonStyleBottomStrip, "Enable Mod", FONT_TEXT[FSize35], InstallMod), ButtonType);
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

ShapeLinker_t *CreateInstallScreen(){
    ShapeLinker_t *out = NULL;

    SDL_Texture *screenshot = ScreenshotToTexture();
    ShapeLinkAdd(&out, ImageCreate(screenshot, POS(0, 0, SCREEN_W, SCREEN_H)), ImageType);
    ShapeLinkAdd(&out, RectangleCreate(POS(0, 0, SCREEN_W, SCREEN_H), COLOR(0,0,0,130), 1), RectangleType);
    ShapeLinkAdd(&out, RectangleCreate(POS(300, SCREEN_H / 2 - 150, SCREEN_W - 600, 300), COLOR_GREY, 1), RectangleType);
    ShapeLinkAdd(&out, TextCenteredCreate(POS(300, SCREEN_H / 2 - 150, SCREEN_W - 600, 75), "Indexing folders...", COLOR_WHITE, FONT_TEXT[FSize35]), TextCenteredType);

    return out;
}

ShapeLinker_t *CreateConflictMenu(ShapeLinker_t *list){
    ShapeLinker_t *out = NULL;

    ShapeLinkAdd(&out, ButtonCreate(POS(0, 0, SCREEN_W, 50), COLOR_GREY, COLOR_AQUA, COLOR_WHITE, COLOR_BLUEGREY, 0, ButtonStyleBottomStrip, "Back", FONT_TEXT[FSize28], exitFunc), ButtonType);
    ShapeLinkAdd(&out, ListViewCreate(POS(0, 50, SCREEN_W, SCREEN_H - 50), 50, COLOR_DARKERGREY, COLOR_BLUEGREY, COLOR_BLUEGREY, COLOR_WHITE, 0, list, NULL, NULL, FONT_TEXT[FSize28]), ListViewType);

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

int ModMenuSelectionChange(Context_t ctx){
    static int lastSelection = -1;
    ListView_t *lv = CAST(ListView_t, ctx.item->item);
    if (lv->highlight != lastSelection){
        lastSelection = lv->highlight;
        
        ShapeLinker_t *gameitem = ShapeLinkOffset(ctx.all, 2);
        TextCentered_t *gameitemcast = CAST(TextCentered_t, gameitem->item);
        char *gamename = gameitemcast->text.text;

        ShapeLinker_t *modentry = ShapeLinkOffset(lv->text, lv->highlight);
        char *modname = CAST(char, modentry->item);

        char *fullpath = CopyTextArgsUtil("/mods/%s/%s", gamename, modname);
        char *checkpath = CopyTextArgsUtil("%s/ENABLED", fullpath);
        char *descpath = CopyTextArgsUtil("%s/DESCRIPTION", fullpath);

        ShapeLinker_t *descobj = ShapeLinkOffset(ctx.all, 7);
        TextCentered_t *desctextbox = CAST(TextCentered_t, descobj->item);

        ShapeLinker_t *installobj = descobj->next;
        Button_t *installbtn = CAST(Button_t, installobj->item);

        free(installbtn->text);

        if (ACCESS(checkpath)){
            installbtn->text = CopyTextUtil("Disable Mod");
        }
        else {
            installbtn->text = CopyTextUtil("Enable Mod");
        }
    }
}

int InstallMod(Context_t ctx){
    ShapeLinker_t *menu = CreateInstallScreen();
    RenderShapeLinkList(menu);

    ShapeLinker_t *gameitem = ShapeLinkOffset(ctx.all, 2);
    TextCentered_t *gameitemcast = CAST(TextCentered_t, gameitem->item);
    char *gamename = gameitemcast->text.text;
   
    ListView_t *lv = CAST(ListView_t, ShapeLinkOffset(ctx.all, 5)->item);
    ShapeLinker_t *selectedmodentry = ShapeLinkOffset(lv->text, lv->highlight);
    char *modname = CAST(char, selectedmodentry->item);

    char *fullpath = CopyTextArgsUtil("/mods/%s/%s", gamename, modname);
    char *checkpath = CopyTextArgsUtil("%s/ENABLED", fullpath);
    ShapeLinker_t *files = GetAllFilesFromFolder(fullpath);
    ShapeLinker_t *conv = MessWithPathList(strlen(fullpath), cfwFolder, files);
    ShapeLinker_t *conflict = MakeConflictList(conv);

    int len = ShapeLinkCount(files);

    ShapeLinker_t *conflictMenu = CreateConflictMenu(conflict);

    ShapeLinker_t *toptextitem = ShapeLinkOffset(menu, 3);
    TextCentered_t *toptext = CAST(TextCentered_t, toptextitem->item);

    Button_t *installbtn = CAST(Button_t, ctx.item->item);

    free(installbtn->text);

    if (!ACCESS(checkpath)){
        if (conflict != NULL){
            free(toptext->text.text);
            toptext->text.text = CopyTextArgsUtil("%d file conflicts found!", ShapeLinkCount(conflict));
            ShapeLinker_t *last = ShapeLinkOffset(menu, ShapeLinkCount(menu) - 1);

            ShapeLinkAdd(&menu, ButtonCreate(POS(300, 435, 226, 75), COLOR_DARKGREY, COLOR_BLUE, COLOR_WHITE, COLOR_BLUEGREY, 0, ButtonStyleBottomStrip, "Show Conflicts", FONT_TEXT[FSize28], exitFunc), ButtonType);
            ShapeLinkAdd(&menu, ButtonCreate(POS(526, 435, 227, 75), COLOR_DARKGREY, COLOR_AQUA, COLOR_WHITE, COLOR_BLUEGREY, 0, ButtonStyleBottomStrip, "Back", FONT_TEXT[FSize28], exitFunc), ButtonType);
            ShapeLinkAdd(&menu, ButtonCreate(POS(753, 435, 227, 75), COLOR_DARKGREY, COLOR_RED, COLOR_WHITE, COLOR_DARKRED, 0, ButtonStyleBottomStrip, "Overwrite", FONT_TEXT[FSize28], exitFunc), ButtonType);

            
            Context_t newCtx = {6, OriginButtonPress, NULL, NULL, 0};
            while (newCtx.origin < OriginFunction){
                newCtx = MakeMenu(menu, newCtx.offset);
                if (newCtx.kDown & KEY_B) 
                    break;

                if (newCtx.offset == 4 && newCtx.origin == OriginFunction){
                    Context_t newerCtx = {0, OriginButtonPress, NULL, NULL, 0};

                    while (newerCtx.origin < OriginFunction){
                        newerCtx = MakeMenu(conflictMenu, newerCtx.offset);
                        if (newerCtx.kDown & KEY_B) 
                            break;
                    }

                    newCtx.origin = OriginButtonPress;
                }
                if ((newCtx.offset == 5 && newCtx.origin == OriginFunction) || newCtx.origin == OriginPlus){
                    goto out;
                }
            }

            ButtonFree(CAST(Button_t, last->next->next->next->item));
            ButtonFree(CAST(Button_t, last->next->next->item));
            ButtonFree(CAST(Button_t, last->next->item));
            last->next = NULL;
        }

        free(toptext->text.text);
        toptext->text.text = CopyTextUtil("Copying files...");
        
        int count = 0;
        ProgressBar_t *pb = ProgressBarCreate(POS(300, 460, 680, 50), COLOR_GREEN, COLOR_DARKERGREY, ProgressBarStyleFlat, 0);
        ShapeLinkAdd(&menu, pb, ProgressBarType);

        mkDirBasedOnPathList(conv);

        ShapeLinker_t *srciter = files, *dstiter = conv;
        for (; srciter != NULL && dstiter != NULL; srciter = srciter->next, dstiter = dstiter->next){
            Copy(CAST(char, srciter->item), CAST(char, dstiter->item));
            count++;
            pb->percentage = count * 100 / len;
            RenderShapeLinkList(menu);
        }
        
        FILE *checkfile = fopen(checkpath, "w");
        fclose(checkfile);

        installbtn->text = CopyTextUtil("Disable Mod");
    }
    else {
        free(toptext->text.text);
        toptext->text.text = CopyTextUtil("Removing files");

        int delcount = 0;
        ProgressBar_t *pb = ProgressBarCreate(POS(300, 460, 680, 50), COLOR_GREEN, COLOR_DARKERGREY, ProgressBarStyleFlat, 0);
        ShapeLinkAdd(&menu, pb, ProgressBarType);

        for (ShapeLinker_t *iter = conv; iter != NULL; iter = iter->next){
            remove(CAST(char, iter->item));
            delcount++;
            pb->percentage = delcount * 100 / len;
            RenderShapeLinkList(menu);
        }

        remove(checkpath);

        installbtn->text = CopyTextUtil("Enable Mod");
    }
    

    out:;
    free(fullpath);
    free(checkpath);
    ShapeLinkDispose(&menu);
    ShapeLinkDispose(&conflictMenu);
    // Clean up old text here too!
    return 0;
}