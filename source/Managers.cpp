#include "Managers.h"

RenderManager::RenderManager()
{
  
}

Vector4 Vector4::operator= (const Vector4& obj)
{
  Vector4 offset; 
  
  offset.X = obj.X;
  offset.Y = obj.Y;
  offset.Z = obj.Z;
  
  return offset; 
};

Vector4 UpdatePosition(const Vector4& position)
{
  // We need another solution for this 

  Vector4 pos;
  pos.X = 1.f;
  pos.Y = 0.f;
  pos.Z = 0.f;

  Vector4 scale;
  scale.X = 2.f;
  scale.Y = 1.f;
  scale.Z = 2.f;
  
  Vector4 rotation;
  rotation.X = 200.f;
  rotation.Y = 100.f;
  rotation.Z = 25.f;
  
  DirectX::XMMATRIX Posit = DirectX::XMMatrixTranslation(pos.X, pos.Y, pos.Z);
  DirectX::XMMATRIX Scale = DirectX::XMMatrixScaling(scale.X, scale.Y, scale.Z);

  // This is not useful for real time rendering, it is not going to animate! 
  DirectX::XMMATRIX Rotate = DirectX::XMMatrixRotationX(rotation.X);
  Rotate = DirectX::XMMatrixRotationY(rotation.Y);
  Rotate = DirectX::XMMatrixRotationZ(rotation.Z);
  
  Model entity; 
  
  entity.vertex->X = 1.f; 
  
  entity.color.X = 1.f; 
  
  return pos; 
}
