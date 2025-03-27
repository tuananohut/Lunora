#ifndef MANAGERS_H
#define MANAGERS_H

#include <d3d11.h>
#include <DirectXMath.h>

// Is it efficient ?
#include <array>
#include <atomic>
#include <thread>
#include <algorithm>

using namespace DirectX;

/**/

struct Vertex
{
    XMFLOAT3 Position;
    XMFLOAT4 Color;
};

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
  auto count = g_renderQueue.objectCount.load(std::memory_order_relaxed);
  
  if (count > 0)
    {
      auto transparentStart = std::stable_partition(g_renderQueue.objects, g_renderQueue.objects + count, [](const RenderableObject& obj) { return obj.layer != RenderLayer::Transparent; });

      std::sort(transparentStart, g_renderQueue.objects + count, 
		[](const RenderableObject& a, const RenderableObject& b) { return a.depth > b.depth; });

    }
  
}

void RenderLayerObjects(ID3D11DeviceContext* context, RenderLayer layer)
{
  UINT stride = sizeof(Vertex);
  UINT offset = 0;
       
  for (unsigned int i = 0;
       i < g_renderQueue.objectCount.load(std::memory_order_relaxed);
       ++i)
    {
      const auto& obj = g_renderQueue.objects[i];
      if (obj.layer != layer) continue;
      
      ID3D11Buffer* vBuffers[] = { obj.vertexBuffer };
      
      context->IASetVertexBuffers(0, 1, vBuffers, &stride, &offset);
      context->IASetIndexBuffer(obj.indexBuffer, DXGI_FORMAT_R32_UINT, 0);
      context->DrawIndexed(obj.indexCount, 0, 0);
    }
}

void RenderScene(ID3D11DeviceContext* context)
{
  SortRenderQueue();

  ID3D11Device* device = nullptr;
  context->GetDevice(&device);
  
  ID3D11DeviceContext* deferredContextOpaque = nullptr;
  ID3D11DeviceContext* deferredContextTransparent = nullptr;
  
  device->CreateDeferredContext(0, &deferredContextOpaque);
  device->CreateDeferredContext(0, &deferredContextTransparent);
 
  std::thread opaqueThread([&]() { RenderLayerObjects(deferredContextOpaque, RenderLayer::Opaque); });
  
  std::thread transparentThread([&]() { RenderLayerObjects(deferredContextTransparent, RenderLayer::Transparent); });

  opaqueThread.join();
  transparentThread.join();

  ID3D11CommandList* commandListOpaque = nullptr;
  ID3D11CommandList* commandListTransparent = nullptr;
  
  deferredContextOpaque->FinishCommandList(FALSE, &commandListOpaque);
  deferredContextTransparent->FinishCommandList(FALSE, &commandListTransparent);

  context->ExecuteCommandList(commandListOpaque, FALSE);
  context->ExecuteCommandList(commandListTransparent, FALSE);

  commandListOpaque->Release();
  commandListTransparent->Release();
  deferredContextOpaque->Release();
  deferredContextTransparent->Release();
  device->Release();
  
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

  RenderQueue renderQueue;

  void AddObjectToQueue(ID3D11Buffer* vBuffer, ID3D11Buffer* iBuffer, 
			unsigned int idxCount, float depth, RenderLayer layer, bool alphaBlending)
  {
    renderQueue.AddObject(vBuffer, iBuffer, idxCount, depth, layer, alphaBlending);
  }

  void RenderScene(ID3D11DeviceContext* context);
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
