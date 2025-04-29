#ifndef MESH_FACTORY_H
#define MESH_FACTORY_H

#include "../RenderData.h" 

void CreateUnitCube(DeviceManager& devMgr,
                    MeshGPUData* outMesh,
                    ShaderGPUData* shader);

void CreateUnitQuad(DeviceManager& DeviceManager,
		    RenderManager* Renderer,
                    MeshGPUData& Quad,
                    ShaderGPUData& Shader,
		    const LPCWSTR VSFilename,
		    const LPCWSTR PSFilename);

#endif
