#include <d3d11.h>
#include <directxmath.h>
// Examples

struct Vertex // Name can change
{
  float X;
  float Y;
  float Z;
  float W = 1.f;

  Vertex operator= (const Vertex& obj)
    {
      Vertex offset; 
      
      offset.X = obj.X;
      offset.Y = obj.Y;
      offset.Z = obj.Z;

      return offset; 
    };
};

struct Entity
{
  Vertex* vertex;
  Vertex color;
  Vertex* texture; 
};

Vertex UpdatePosition(const Vertex& position)
{
  // We need another solution for this 

  Vertex pos;
  pos.X = 1.f;
  pos.Y = 0.f;
  pos.Z = 0.f;

  Vertex scale;
  scale.X = 2.f;
  scale.Y = 1.f;
  scale.Z = 2.f;
  
  Vertex rotation;
  rotation.X = 200.f;
  rotation.Y = 100.f;
  rotation.Z = 25.f;
  
  DirectX::XMMATRIX Posit = DirectX::XMMatrixTranslation(pos.X, pos.Y, pos.Z);
  DirectX::XMMATRIX Scale = DirectX::XMMatrixScaling(scale.X, scale.Y, scale.Z);

  // This is not useful for real time rendering, it is not going to animate! 
  DirectX::XMMATRIX Rotate = DirectX::XMMatrixRotationX(rotation.X);
  Rotate = DirectX::XMMatrixRotationY(rotation.Y);
  Rotate = DirectX::XMMatrixRotationZ(rotation.Z);
  
  Entity entity; 
  
  entity.vertex->X = 1.f; 
  
  entity.color.X = 1.f; 
  
  return pos; 
}

struct RenderManager
{
  RenderManager();
  ~RenderManager();


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

