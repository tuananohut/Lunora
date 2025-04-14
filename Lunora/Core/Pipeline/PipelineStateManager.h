#ifndef PIPELINESTATEMANAGER_H
#define PIPELINESTATEMANAGER_H

struct PipelineStateManager
{
  ID3D11RasterizerState* RasterizerState;

  void Initialize(DeviceManager& DeviceManager);
  void Cleanup();
};

#endif
