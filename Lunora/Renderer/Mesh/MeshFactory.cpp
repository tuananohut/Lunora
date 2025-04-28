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
    
    // BuildMesh(DeviceManager, Mesh, vertices, 24, indices, 36);
}

void CreateUnitQuad(DeviceManager& DeviceManager,
		    RenderManager* Renderer, 
		    MeshGPUData& Mesh,
		    ShaderGPUData& Shader,
		    const LPCWSTR VSFilename,
		    const LPCWSTR PSFilename)
{
  const Vertex vertices[4] =
      {
	{{-0.5f,-0.5f,0},{0,0,1},{0,1}},
	{{-0.5f, 0.5f,0},{0,0,1},{0,0}},
	{{ 0.5f,-0.5f,0},{0,0,1},{1,1}},
	{{ 0.5f, 0.5f,0},{0,0,1},{1,0}}
      };
  
  const unsigned long indices[6] = {0,1,2, 2,1,3};

  BuildMesh(DeviceManager, Renderer,
	    &Mesh, &Shader,
	    vertices, 4,
	    indices, 6,
	    VSFilename, PSFilename);
}
