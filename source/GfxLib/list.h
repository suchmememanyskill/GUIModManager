#pragma once
#include "shapes.h"

int ShapeLinkAdd(ShapeLinker_t **start, void *item, u8 type);
ShapeLinker_t *ShapeLinkOffset(ShapeLinker_t *start, int offset);
void ShapeLinkDispose(ShapeLinker_t **start);
int ShapeLinkCount(ShapeLinker_t *start);