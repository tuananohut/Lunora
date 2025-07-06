#ifndef MANAGERS_H
#define MANAGERS_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>

using namespace DirectX;

enum struct ShaderLayoutType
  {
    Color,
    Texture,
    Light
  };

struct RenderManager
{
  RenderManager();
  ~RenderManager();
  
  void InitializeLayouts();
  D3D11_INPUT_ELEMENT_DESC* GetLayout(ShaderLayoutType type);
  unsigned int GetLayoutElementCount(ShaderLayoutType type);
  
  static D3D11_INPUT_ELEMENT_DESC* ColorLayout;
  static D3D11_INPUT_ELEMENT_DESC* TextureLayout;
  static D3D11_INPUT_ELEMENT_DESC* LightLayout;
  static bool layoutsInitialized;   
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
