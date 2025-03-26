#ifndef MANAGERS_H
#define MANAGERS_H

#include <d3d11.h>
#include <DirectXMath.h>

// Is it efficient ?
#include <array>
#include <atomic>
#include <thread>
#include <algorithms>

using namespace DirectX;

/**/

constexpr int MAX_OBJECTS = 10000;

enum class RenderLayer
  {
    Opaque,
    Transparent,
    ShadowPass
  };

struct RenderableObject
{
  ID3D11Buffer* vertexBuffer;
  ID3D11Buffer* indexBuffer;
  unsigned int indexCount;
  float depth;
  RenderLayer layer;
  bool usesAlphaBlending; 
};

struct RenderQueue
{
  RenderableObject objects[MAX_OBJECTS];
  std::atomic<unsigned int> objectCount = 0;
  
  void AddObject(ID3D11Buffer* vBuffer,
		 ID3D11Buffer* iBuffer,
		 unsigned int idxCount, float depth,
		 RenderLayer layer,
		 bool alphaBlending)
  {
    unsigned int index = objectCount.fetch_add(1, std::memory_order_relaxed);
    if (index < MAX_OBJECTS)
      {
	objects[index] = { vBuffer, iBuffer, idxCount, depth, layer, alphaBlending };
      }
  }
};

RenderQueue g_renderQueue;

void SortRenderQueue()
{
  std::sort(g_renderQueue.objects,
	    g_renderQueue.objects + g_renderQueue.objectCount.load(std::memory_order_relaxed),
	    [](const RenderableObject& a,
	       const RenderableObject& b)
    {
      return (a.layer == RenderLayer::Transparent) ? (a.depth > b.depth) : (a.layer < b.layer);
    });
}

void RenderLayerObjects(ID3D11DeviceContext* context, RenderLayer layer)
{
  for (uint i = 0;
       i < g_renderQueue.objectCount.load(std::memory_order_relaxed);
       ++i)
    {
      const auto& obj = g_renderQueue.objects[i];
      if (obj.layer != layer) continue;

      UINT stride = sizeof(Vertex);
      UINT offset = 0;
      context->IASetVertexBuffers(0, 1, &obj.vertexBuffer, &stride, &offset);
      context->IASetIndexBuffer(obj.indexBuffer, DXGI_FORMAT_R32_UINT, 0);
      context->DrawIndexed(obj.indexCount, 0, 0);
    }
}

void RenderScene(ID3D11DeviceContext* context)
{
  SortRenderQueue();

  std::thread opaqueThread(RenderLayerObjects, context, RenderLayer::Opaque);
  std::thread transparentThread(RenderLayerObjects, context, RenderLayer::Transparent);

  opaqueThread.join();
  transparentThread.join();

  g_renderQueue.objectCount.store(0, std::memory_order_relaxed);
}

/**/

struct RenderManager
{
  RenderManager();
  ~RenderManager();

  static D3D11_INPUT_ELEMENT_DESC Layouts[3][2]; 
 
  D3D11_INPUT_ELEMENT_DESC (&RenderManager::ColorShader())[2];
  D3D11_INPUT_ELEMENT_DESC (&RenderManager::TextureShader())[2];
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
