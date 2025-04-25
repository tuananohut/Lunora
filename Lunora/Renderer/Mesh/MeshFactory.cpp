#include "MeshFactory.h"
#include "MeshBuilder.h"

void CreateUnitCube(DeviceManager& DeviceManager,
		    MeshGPUData* Mesh,
		    ShaderGPUData* Shader)
{
    Vertex vertices[24] =
      {
	
      };
    
    unsigned long indices[36] =
      {

      };

    BuildMesh(DeviceManager, Mesh, vertices, 24, indices, 36);
}

void CreateUnitQuad(DeviceManager& DeviceManager,
		    MeshGPUData* Mesh,
		    ShaderGPUData* Shader)
{
    Vertex vertices[4] =
      {
	{{-0.5f,-0.5f,0},{0,0,1},{0,1}},
	{{-0.5f, 0.5f,0},{0,0,1},{0,0}},
	{{ 0.5f,-0.5f,0},{0,0,1},{1,1}},
	{{ 0.5f, 0.5f,0},{0,0,1},{1,0}}
      };
    
    unsigned long indices[6] = {0,1,2, 2,1,3};

    BuildMesh(DeviceManager, Mesh, vertices, 4, indices, 6);
}
