#ifndef MESH_FACTORY_H
#define MESH_FACTORY_H

#include "Mesh.h" 

void CreateUnitCube(DeviceManager& devMgr,
                    MeshGPUData* outMesh,
                    ShaderGPUData* shader);

void CreateUnitQuad(DeviceManager& devMgr,
                    MeshGPUData* outMesh,
                    ShaderGPUData* shader);

#endif
