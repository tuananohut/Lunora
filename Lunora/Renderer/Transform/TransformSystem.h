#ifndef TRANSFORMSYSTEM_H
#define TRANSFORMSYSTEM_H

#include "Transform.h"

struct TransformSystem
{
  static XMMATRIX Compose(const Transform& transform)
  {
    return transform.Scale * transform.RotationMatrix * transform.Translation;
  }

  static Transform Identity()
  {
    return Transform();
  }
};

#endif
