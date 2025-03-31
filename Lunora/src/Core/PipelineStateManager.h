#ifndef PIPELINESTATEMANAGER_H
#define PIPELINESTATEMANAGER_H

#include "DeviceManager.h"

struct PipelineStateManager
{
  ID3D11RasterizerState* RasterizerState;

  void Initialize(DeviceManager& DeviceManager);
  void Cleanup();
};

#endif
