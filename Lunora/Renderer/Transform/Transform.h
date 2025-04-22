#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <DirectXMath.h>

using namespace DirectX;

struct Transform
{
  XMMATRIX Translation = XMMatrixIdentity();
  XMMATRIX RotationMatrixX = XMMatrixIdentity();
  XMMATRIX RotationMatrixY = XMMatrixIdentity();
  XMMATRIX RotationMatrixZ = XMMatrixIdentity();
  XMMATRIX RotationMatrix = XMMatrixIdentity();
  XMMATRIX Scale = XMMatrixIdentity();
};

#endif
