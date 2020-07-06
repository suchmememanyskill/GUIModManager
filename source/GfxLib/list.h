#pragma once
#include "shapes.h"

int ShapeLinkAdd(ShapeLinker_t **start, void *item, u8 type);
ShapeLinker_t *ShapeLinkOffset(ShapeLinker_t *start, int offset);
void ShapeLinkDispose(ShapeLinker_t **start);
int ShapeLinkCount(ShapeLinker_t *start);
void ShapeLinkMergeLists(ShapeLinker_t **dst, ShapeLinker_t *add);
ShapeLinker_t *ShapeLinkFind(ShapeLinker_t  *start, int find);