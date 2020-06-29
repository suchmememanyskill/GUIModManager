#pragma once
#include "GfxLib/shapes.h"

#define ACCESS(path) !access(path, F_OK)

extern char *cfwFolder;

ShapeLinker_t *ListFolder(char *path);
int SetCfwFolder();
ShapeLinker_t *GetAllFilesFromFolder(char *path);
ShapeLinker_t *MessWithPathList(int startStrip, char *replace, ShapeLinker_t *in);
ShapeLinker_t *MakeConflictList(ShapeLinker_t *in);
int CopyDir(const char *src, const char *dst);
int Copy(const char* src, const char* dst);
int CopyContentsDir(const char *src, const char *dst);
int mkDirBasedOnPathList(ShapeLinker_t *in);