#ifndef SCENEMANAGEMENTSYSTEM_H
#define SCENEMANAGEMENTSYSTEM_H

#include <d3d11.h>
#include <DirectXMath.h>

#include "Core/DeviceManager.cpp"

using namespace DirectX; 

struct Entity
{
  uint32_t id;
  uint32_t meshIndex;
  uint32_t lightIndex;

  XMFLOAT3 position;
  XMFLOAT3 scale;
  XMFLOAT3 rotation;
};

struct Mesh
{
  ID3D11Buffer* vertexBuffer;
  ID3D11Buffer* indexBuffer;
  uint32_t vertexCount;
  uint32_t indexCount;
  ID3D11ShaderResourceView* texture; 
};

struct Light
{
  XMFLOAT3 position;
  XMFLOAT3 color;
  float intensity;
};

struct Camera
{
  XMFLOAT3 position;
  XMFLOAT3 target;
  XMFLOAT3 up;

  float fov;
  float nearPlane;
  float farPlane;
  float aspectRatio; 
};

struct SceneManager
{
  SceneManager(DeviceManager& DeviceManager);
  ~SceneManager();

  void AddEntity(const Entity& entity);
  void AddMesh(const Mesh& mesh);
  void AddLight(const Light& light);
  void SetCamera(const Camera& camera);

  void Update(float deltaTime);
  void Render(DeviceManager* DeviceManager);

  Entity m_entities[];
  Mesh m_meshes[];
  Light m_lights[];
  Camera m_camera; 
};

#endif
