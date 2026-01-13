#ifndef HEMISPHERICMESH_H
#define HEMISPHERICMESH_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <stdio.h>

#include "../Engine/src/Renderer.h"

using namespace DirectX;
using namespace LunoraEngine; 

struct VertexHemispheric
{
  XMFLOAT3 position;
  XMFLOAT2 texture;
  XMFLOAT3 normal; 
};

#endif
