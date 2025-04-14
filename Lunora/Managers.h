#ifndef MANAGERS_H
#define MANAGERS_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>

using namespace DirectX;

EXTERN_C const IID IID_ID3D11ShaderReflection = { 0x8d536ca1, 0x0cca, 0x4956,{ 0xa8, 0x37, 0x78, 0x69, 0x88, 0x30, 0x7b, 0x02 } };

#define MAX_INPUT_ELEMENTS 8

struct RenderManager
{
  RenderManager();
  ~RenderManager();
  
  static D3D11_INPUT_ELEMENT_DESC Layouts[3][2]; 
  
  D3D11_INPUT_ELEMENT_DESC (&RenderManager::ColorShader())[2];
  D3D11_INPUT_ELEMENT_DESC (&RenderManager::TextureShader())[2];
  D3D11_INPUT_ELEMENT_DESC (&RenderManager::LightShader())[2];
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
