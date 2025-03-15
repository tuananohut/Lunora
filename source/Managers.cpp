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
