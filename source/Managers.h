#ifndef MANAGERS_H
#define MANAGERS_H

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

struct RenderManager
{
  RenderManager();
  ~RenderManager();

  static D3D11_INPUT_ELEMENT_DESC Layouts[3][2]; 
 
  D3D11_INPUT_ELEMENT_DESC (&RenderManager::ColorShader())[2];
  D3D11_INPUT_ELEMENT_DESC TextureShader();
  D3D11_INPUT_ELEMENT_DESC LightShader(); 
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
