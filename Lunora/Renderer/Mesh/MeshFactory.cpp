#include "MeshFactory.h"
#include "MeshBuilder.h"

void CreateUnitCube(DeviceManager& DeviceManager,
		    RenderManager* Renderer,
		    MeshGPUData& Cube,
		    ShaderGPUData& Shader,
		    const LPCWSTR VSFilename,
		    const LPCWSTR PSFilename) 
{
  const Vertex vertices[24] =
    {
      // Front face (Z+)
      {{-0.5f, -0.5f,  0.5f}, { 0, 0, 1 }, { 0, 1 }},
      {{-0.5f,  0.5f,  0.5f}, { 0, 0, 1 }, { 0, 0 }},
      {{ 0.5f,  0.5f,  0.5f}, { 0, 0, 1 }, { 1, 0 }},
      {{ 0.5f, -0.5f,  0.5f}, { 0, 0, 1 }, { 1, 1 }},

      // Back face (Z-)
      {{ 0.5f, -0.5f, -0.5f}, { 0, 0, -1 }, { 0, 1 }},
      {{ 0.5f,  0.5f, -0.5f}, { 0, 0, -1 }, { 0, 0 }},
      {{-0.5f,  0.5f, -0.5f}, { 0, 0, -1 }, { 1, 0 }},
      {{-0.5f, -0.5f, -0.5f}, { 0, 0, -1 }, { 1, 1 }},

      // Left face (X-)
      {{-0.5f, -0.5f, -0.5f}, { -1, 0, 0 }, { 0, 1 }},
      {{-0.5f,  0.5f, -0.5f}, { -1, 0, 0 }, { 0, 0 }},
      {{-0.5f,  0.5f,  0.5f}, { -1, 0, 0 }, { 1, 0 }},
      {{-0.5f, -0.5f,  0.5f}, { -1, 0, 0 }, { 1, 1 }},

      // Right face (X+)
      {{ 0.5f, -0.5f,  0.5f}, { 1, 0, 0 }, { 0, 1 }},
      {{ 0.5f,  0.5f,  0.5f}, { 1, 0, 0 }, { 0, 0 }},
      {{ 0.5f,  0.5f, -0.5f}, { 1, 0, 0 }, { 1, 0 }},
      {{ 0.5f, -0.5f, -0.5f}, { 1, 0, 0 }, { 1, 1 }},

      // Top face (Y+)
      {{-0.5f,  0.5f,  0.5f}, { 0, 1, 0 }, { 0, 1 }},
      {{-0.5f,  0.5f, -0.5f}, { 0, 1, 0 }, { 0, 0 }},
      {{ 0.5f,  0.5f, -0.5f}, { 0, 1, 0 }, { 1, 0 }},
      {{ 0.5f,  0.5f,  0.5f}, { 0, 1, 0 }, { 1, 1 }},

      // Bottom face (Y-)
      {{-0.5f, -0.5f, -0.5f}, { 0, -1, 0 }, { 0, 1 }},
      {{-0.5f, -0.5f,  0.5f}, { 0, -1, 0 }, { 0, 0 }},
      {{ 0.5f, -0.5f,  0.5f}, { 0, -1, 0 }, { 1, 0 }},
      {{ 0.5f, -0.5f, -0.5f}, { 0, -1, 0 }, { 1, 1 }},
    };

    
  const unsigned long indices[36] =
    {
      0,  1,  2,  0,  2,  3,   // Front
      4,  5,  6,  4,  6,  7,   // Back
      8,  9, 10,  8, 10, 11,   // Left
      12, 13, 14, 12, 14, 15,   // Right
      16, 17, 18, 16, 18, 19,   // Top
      20, 21, 22, 20, 22, 23    // Bottom
    };

    BuildMesh(DeviceManager, Renderer, Cube, Shader, vertices, 24, indices, 36, VSFilename, PSFilename);
}

void CreateUnitQuad(DeviceManager& DeviceManager,
                    RenderManager* Renderer,  
                    MeshGPUData& Quad,
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

  // Pass Renderer as a single pointer, not a pointer to a pointer
  BuildMesh(DeviceManager, Renderer, Quad, Shader, vertices, 4, indices, 6, VSFilename, PSFilename);
}

