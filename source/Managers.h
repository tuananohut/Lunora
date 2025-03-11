#include <d3d11.h>

// Examples

struct RenderManager
{
  RenderManager();
  ~RenderManager();

  void StartUp(); 
  void ShutDown();
};

struct PhysicsManager
{
  PhysicsManager();
  ~PhysicsManager();

  void StartUp();
  void ShutDown();
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

