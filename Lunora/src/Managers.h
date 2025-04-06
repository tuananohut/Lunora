#ifndef MANAGERS_H
#define MANAGERS_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>

using namespace DirectX;

EXTERN_C const IID IID_ID3D11ShaderReflection = { 0x8d536ca1, 0x0cca, 0x4956,{ 0xa8, 0x37, 0x78, 0x69, 0x88, 0x30, 0x7b, 0x02 } };

#define MAX_INPUT_ELEMENTS 8

struct RenderManager
{
  RenderManager();
  ~RenderManager();
  
  static D3D11_INPUT_ELEMENT_DESC Layouts[3][2]; 
  
  D3D11_INPUT_ELEMENT_DESC (&RenderManager::ColorShader())[2];
  D3D11_INPUT_ELEMENT_DESC (&RenderManager::TextureShader())[2];
  D3D11_INPUT_ELEMENT_DESC (&RenderManager::LightShader())[2];
};

struct InputLayout
{
  D3D11_INPUT_ELEMENT_DESC Elements[MAX_INPUT_ELEMENTS];
  UINT Count;
  ID3D11InputLayout* Layout; 
};

struct InputLayoutManager
{
  InputLayout Layouts[16];
  UINT LayoutCount;

  void Init()
  {
    LayoutCount = 0;
  }

  InputLayout* CreateLayout(ID3D11Device* Device, ID3DBlob* VertexShaderBlob)
  {
    ID3D11ShaderReflection* Reflector = 0;

    HRESULT Result = D3DReflect(VertexShaderBlob->GetBufferPointer(),
				VertexShaderBlob->GetBufferSize(),
				IID_ID3D11ShaderReflection,
				(void**)&Reflector);
    if(FAILED(Result))
      {
	return 0; 
      }

    D3D11_SHADER_DESC ShaderDesc;
    Reflector->GetDesc(&ShaderDesc);

    if (LayoutCount >= 16)
      {
	return 0;
      }

    InputLayout* Layout = &Layouts[LayoutCount++];
    Layout->Count = 0;

    UINT Offset;

    for (UINT i = 0;
	 i < ShaderDesc.InputParameters && i < MAX_INPUT_ELEMENTS;
	 ++i)
      {
	D3D11_SIGNATURE_PARAMETER_DESC ParamDesc;
	Reflector->GetInputParameterDesc(i, &ParamDesc);
	
	D3D11_INPUT_ELEMENT_DESC* Desc = &Layout->Elements[i];
	Desc->SemanticName = ParamDesc.SemanticName;
	Desc->SemanticIndex = ParamDesc.SemanticIndex;
	Desc->InputSlot = 0;
	Desc->AlignedByteOffset = Offset;
	Desc->InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	Desc->InstanceDataStepRate = 0;

	if (ParamDesc.Mask = 1)
	  Desc->Format = DXGI_FORMAT_R32_FLOAT, Offset += 4;
	else if (ParamDesc.Mask == 3)
	  Desc->Format = DXGI_FORMAT_R32G32_FLOAT, Offset += 8;
	else if (ParamDesc.Mask == 7)
	  Desc->Format = DXGI_FORMAT_R32G32B32_FLOAT, Offset += 12;
	else if (ParamDesc.Mask == 15)
	  Desc->Format = DXGI_FORMAT_R32G32B32A32_FLOAT, Offset += 16;

	Layout->Count++;
      }

    Result = Device->CreateInputLayout(Layout->Elements, Layout->Count,
				       VertexShaderBlob->GetBufferPointer(),
				       VertexShaderBlob->GetBufferSize(),
				       &Layout->Layout);

    Reflector->Release();
    return SUCCEEDED(Result) ? Layout: 0; 
  }
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

#endif
