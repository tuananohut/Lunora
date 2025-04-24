#include "MeshFactory.h"
#include "MeshBuilder.h"

void CreateUnitQuad(DeviceManager& devMgr, MeshGPUData* mesh, ShaderGPUData* shader)
{
    Vertex verts[4] =
      {
	{{-0.5f,-0.5f,0},{0,0,1},{0,1}},
	{{-0.5f, 0.5f,0},{0,0,1},{0,0}},
	{{ 0.5f,-0.5f,0},{0,0,1},{1,1}},
	{{ 0.5f, 0.5f,0},{0,0,1},{1,0}}
      };
    unsigned long inds[6] = {0,1,2, 2,1,3};

    BuildMesh(devMgr, mesh, verts, 4, inds, 6);
}

void CreateUnitCube(DeviceManager& devMgr, MeshGPUData* mesh, ShaderGPUData* shader)
{
    Vertex verts[24] = { /* … önceki kodun 24 vertex’ini buraya koy */ };
    unsigned long inds[36] = { /* … önceki 36 indeks */ };

    BuildMesh(devMgr, mesh, verts, 24, inds, 36);
}

void CreateUnitSphere(DeviceManager& devMgr, MeshGPUData* mesh, ShaderGPUData* shader,
                      int latSegs, int longSegs)
{
    int vertCount = (latSegs+1)*(longSegs+1);
    int idxCount  = latSegs*longSegs*6;
    Vertex* verts = new Vertex[vertCount];
    unsigned long* inds = new unsigned long[idxCount];



    BuildMesh(devMgr, mesh, verts, vertCount, inds, idxCount);

    delete[] verts;
    delete[] inds;
}
