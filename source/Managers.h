#include <d3d11.h>
#include <directxmath.h>
// Examples

struct Vector4 
{
  float X;
  float Y;
  float Z;
  float W = 1.f;
};

struct Model
{
  Vector4* vertex;
  Vector4 color;
  Vector4* texture; 
};

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

