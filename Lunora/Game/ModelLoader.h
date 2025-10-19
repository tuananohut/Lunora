#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

typedef struct {
    XMFLOAT3 position;
    XMFLOAT4 color;
} SimpleVertexCombined;

bool LoadModelFromFile(char filename[],
		       SimpleVertexCombined** outVertices,
		       UINT* outVertexCount,
		       unsigned long** outIndices,
		       UINT* outIndexCount);

#endif 
