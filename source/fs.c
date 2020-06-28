#include "fs.h"
#include "GfxLib/gfxutils.h"
#include "GfxLib/list.h"
#include "GfxLib/shapes.h"
#include <switch.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

ShapeLinker_t *ListFolder(char *path){
    ShapeLinker_t *out = NULL;

    struct dirent *de;
    DIR *dr = opendir(path);

    if (dr == NULL)
        return NULL;

    while ((de = readdir(dr)) != NULL){
        if (de->d_type == DT_DIR){
            ShapeLinkAdd(&out, CopyTextUtil(de->d_name), 0);
        }
    }

    return out;
}