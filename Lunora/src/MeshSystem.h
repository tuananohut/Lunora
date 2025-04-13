#ifndef MESHSYSTEM_H
#define MESHSYSTEM_H

#include "RenderData.h"
#include <d3d11.h>

void UpdateMeshSystem(ID3D11DeviceContext* DeviceContext,
		      const MeshData* MeshData); 

#endif
