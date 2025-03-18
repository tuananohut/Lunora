#ifndef MANAGERS_H
#define MANAGERS_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

struct RenderManager
{
  RenderManager();
  ~RenderManager();

  std::vector<D3D11_INPUT_ELEMENT_DESC> Layouts[3]; 
  D3D11_INPUT_ELEMENT_DESC PolygonLayoutColor[2]; 

  // Renk ayrý, texture ayrý olmalý 
  
  
  PolygonLayoutColor[0].SemanticName = "POSITION";
  PolygonLayoutColor[0].SemanticIndex = 0;
  PolygonLayoutColor[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
  PolygonLayoutColor[0].InputSlot = 0;
  PolygonLayoutColor[0].AlignedByteOffset = 0;
  PolygonLayoutColor[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  PolygonLayoutColor[0].InstanceDataStepRate = 0;
  
  PolygonLayoutColor[1].SemanticName = "COLOR";
  PolygonLayoutColor[1].SemanticIndex = 0;
  PolygonLayoutColor[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
  PolygonLayoutColor[1].InputSlot = 0;
  PolygonLayoutColor[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
  PolygonLayoutColor[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  PolygonLayoutColor[1].InstanceDataStepRate = 0;

  Layouts[0].assign(std::begin(PolygonLayoutColor), std::end(PolygonLayoutColor));



};

struct PhysicsManager
{
  PhysicsManager();
  ~PhysicsManager();
};

struct MemoryManager{};

// Model and texture loading
struct FileSystemManager{};

struct ControllerManager{};

struct SceneManagerEnumerator{};

struct SceneManager{};

struct ArchiveManager{};

struct MaterialManager{};

struct MeshManager{};

#endif
