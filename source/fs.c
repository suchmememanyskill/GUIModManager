#include "fs.h"
#include "GfxLib/gfxutils.h"
#include "GfxLib/list.h"
#include "GfxLib/shapes.h"
#include <switch.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

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

    closedir(dr);
    return out;
}

ShapeLinker_t *ListFolderWithEnabledEntries(char *path){
    ShapeLinker_t *out = NULL;

    struct dirent *de;
    DIR *dr = opendir(path);

    if (dr == NULL)
        return NULL;

    while ((de = readdir(dr)) != NULL){
        if (de->d_type == DT_DIR){
            char *test = CopyTextArgsUtil("%s/%s/ENABLED", path, de->d_name);
            ShapeLinkAdd(&out, CopyTextUtil(de->d_name), ACCESS(test) ? 2 : 1);
            free(test);
        }
    }

    closedir(dr);
    return out;
}

char *cfwFolder = NULL;

int SetCfwFolder(){
    if (ACCESS("/atmosphere"))
        cfwFolder = "/atmosphere";
    else if (ACCESS("/reinx"))
        cfwFolder = "/reinx";
    else if (ACCESS("/sxos"))
        cfwFolder = "/sxos";

    return (cfwFolder == NULL) ? 1 : 0;
}

ShapeLinker_t *GetAllFilesFromFolder(char *path){
    ShapeLinker_t *out = NULL;

    struct dirent *de;
    DIR *dr = opendir(path);
    if (dr == NULL)
        return NULL;

    while ((de = readdir(dr)) != NULL){
        if (de->d_type == DT_REG){
            if (!strcmp(de->d_name, "DESCRIPTION"))
                continue;
            ShapeLinkAdd(&out, CopyTextArgsUtil("%s/%s", path, de->d_name), 0);
        }
        else if (de->d_type == DT_DIR){
            char *temppath = CopyTextArgsUtil("%s/%s", path, de->d_name);
            ShapeLinker_t *temp = GetAllFilesFromFolder(temppath);
            ShapeLinkMergeLists(&out, temp);
            free(temppath);
        }
    }

    closedir(dr);
    return out;
}

ShapeLinker_t *MessWithPathList(int startStrip, char *replace, ShapeLinker_t *in){
    ShapeLinker_t *out = NULL;

    for (ShapeLinker_t *iter = in; iter != NULL; iter = iter->next){
        char *temp = CopyTextArgsUtil("%s%s", replace, CAST(char, iter->item) + startStrip);
        ShapeLinkAdd(&out, temp, 0);
    }

    return out;
}

ShapeLinker_t *MakeConflictList(ShapeLinker_t *in){
    ShapeLinker_t *out = NULL;

    for (ShapeLinker_t *iter = in; iter != NULL; iter = iter->next){
        if (ACCESS(CAST(char, iter->item))){
            ShapeLinkAdd(&out, CAST(char, iter->item), 0);
        }
    }

    return out;
}

#define BUFSIZE 0x80000

int Copy(const char* src, const char* dst){
    FILE* in = fopen(src, "rb");
    FILE* out = fopen(dst, "wb");

    if (in == NULL){
        return -1;
    }
        
	if (out == NULL){
        return -2;
    }

    size_t len = 0;
    u8 *buffer = malloc(BUFSIZE);
    while((len = fread(buffer, 1, BUFSIZE, in)) > 0)
        fwrite(buffer, 1, len, out);
    
    fclose(in);
    fclose(out);
    free(buffer);

	return 0;
}

int CopyDir(const char *src, const char *dst){ // Doesn't work?
    char *srcDirName = strrchr(src, '/') + 1;
    char *dstPath = CopyTextArgsUtil("%s/%s", dst, srcDirName);
    char *in, *out;

    if (!ACCESS(dstPath))
        mkdir(dstPath, 0777);

    struct dirent *de;
    DIR *dr = opendir(src);
    if (dr == NULL)
        return -1;

    while ((de = readdir(dr)) != NULL){
        if (de->d_type == DT_REG){
            in = CopyTextArgsUtil("%s/%s", src, de->d_name);
            out = CopyTextArgsUtil("%s/%s", dstPath, de->d_name);

            if (Copy(in, out))
                return -1;

            free(in);
            free(out);
        }
        else if (de->d_type == DT_DIR){
            in = CopyTextArgsUtil("%s/%s", src, de->d_name);
            out = CopyTextArgsUtil("%s/%s", dstPath, de->d_name);

            CopyDir(in, out);

            free(in);
            free(out);
        }
    }

    closedir(dr);
    free(dstPath);
    return 0;
}

int CopyContentsDir(const char *src, const char *dst){ // Doesn't work?
    struct dirent *de;
    DIR *dr = opendir(src);
    if (dr == NULL)
        return -1;

    while ((de = readdir(dr)) != NULL){
        if (de->d_type == DT_DIR){
            char *in = CopyTextArgsUtil("%s/%s", src, de->d_name);
            CopyDir(in, dst);
            free(in);
        }
    }

    closedir(dr);
    return 0;
}

int mkDirRecursive(char *in){
    char *search = in + 1;

    while (1){
        while (*search != '/'){
            if (*search == '\0')
                goto out;

            search++;
        }

        char *out = CopyTextLenUtil(in, (int)(search - in));
        search++;

        if (!ACCESS(out))
            mkdir(out, 0777);
    }

    out:;
    return 0;
}

int mkDirBasedOnPathList(ShapeLinker_t *in){
    for (ShapeLinker_t *iter = in; iter != NULL; iter = iter->next){
        char *path = CAST( char, iter->item);
        /*
        char *file = strrchr(path, '/');

        char *out = CopyTextLenUtil(path, (int)(file - path));

        temp = !ACCESS(out);
        Log(CopyTextArgsUtil("%d %s\n", temp, out));
        if (temp)
            Log(CopyTextArgsUtil("%d\n", mkdir(out, 0777)));
        
            

        free(out);
        */
       mkDirRecursive(path);
    }

    return 0;
}