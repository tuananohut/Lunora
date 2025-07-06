#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#include "Core/Device/DeviceManager.h"
#include "Core/Pipeline/PipelineStateManager.h"
#include "Core/Resource/RenderTargetManager.h"

#include "Camera/Camera.h"

using namespace DirectX;

#define DeleteObject(object) if ((object) != NULL) { delete object; object = NULL; }
#define DeleteObjects(objects) if ((objects) != NULL) { delete[] objects; objects = NULL; }
#define ReleaseObject(object) if ((object) != NULL) { object->Release(); object = NULL; }

const UINT ScreenWidth = 1280;
const UINT ScreenHeight = 720;

#endif
