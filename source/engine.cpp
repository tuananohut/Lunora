#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <fstream>
#include "resource.h"

using namespace DirectX;
using namespace std;

#define DeleteObject(object) if ((object) != NULL) { delete object; object = NULL; }
#define DeleteObjects(objects) if ((objects) != NULL) { delete[] objects; objects = NULL; }
#define ReleaseObject(object) if ((object) != NULL) { object->Release(); object = NULL; }

static bool Running;

static ID3D11Device* Device = nullptr;
static ID3D11DeviceContext* DeviceContext = nullptr;
static IDXGISwapChain* SwapChain;
static ID3D11RenderTargetView* RenderTargetView = nullptr;  
static ID3D11DepthStencilView* DepthStencilView = nullptr;
static FLOAT BackgroundColor[4] = {0.141f, 0.137f, 0.365f, 1.f};

void InitializeDX11(HWND Window)
{
  HRESULT Result;
  UINT CreateDeviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
  CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
  
  D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_1;
  
  DXGI_SWAP_CHAIN_DESC SwapChainDesc;

  UINT MultiSamplingCount = 4;
  UINT MultiSamplingQualityLevels = 0;
  UINT ScreenWidth = 800;
  UINT ScreenHeight = 600;
  bool MultiSamplingEnabled = false; 

  ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
  SwapChainDesc.BufferDesc.Width = ScreenWidth;
  SwapChainDesc.BufferDesc.Height = ScreenHeight;
  SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  SwapChainDesc.BufferCount = 1;
  SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  SwapChainDesc.OutputWindow = Window;
  SwapChainDesc.Windowed = TRUE;
  SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
  SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
  SwapChainDesc.Flags = 0;
  
  if (MultiSamplingEnabled)
    {
      SwapChainDesc.SampleDesc.Count = MultiSamplingCount;
      SwapChainDesc.SampleDesc.Quality = MultiSamplingQualityLevels - 1;
    }
  else
    {
      SwapChainDesc.SampleDesc.Count = 1;
      SwapChainDesc.SampleDesc.Quality = 0;
    }
  
  Result = D3D11CreateDeviceAndSwapChain(NULL, 
					 D3D_DRIVER_TYPE_HARDWARE, 
					 NULL, 
					 0, 
					 &FeatureLevel, 
					 1,
					 D3D11_SDK_VERSION, 
					 &SwapChainDesc,
					 &SwapChain, 
					 &Device, 
					 NULL,
					 &DeviceContext); 
  if (FAILED(Result))
    {
      OutputDebugStringA("Device and swap chain");
    }
  
  IDXGIDevice* DXGIDevice = nullptr;
  Result = Device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&DXGIDevice));
  if (FAILED(Result))
    {
      OutputDebugStringA("Failed to get DXGI Device");
    }
  
  IDXGIAdapter* DXGIAdapter = nullptr;
  Result = DXGIDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&DXGIAdapter));
  if (FAILED(Result))
    {
      OutputDebugStringA("DXGI Adapter");
    }
    
  Result = Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM,
						 MultiSamplingCount,
						 &MultiSamplingQualityLevels);
  if (FAILED(Result))
    {
      MessageBoxA(Window, "Multi sample quality levels", "Error.", MB_OK | MB_ICONERROR);
    }

  ReleaseObject(DXGIDevice);
  ReleaseObject(DXGIAdapter);
  
  ID3D11Texture2D* BackBuffer;
  Result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&BackBuffer));
  if (FAILED(Result))
    {
      OutputDebugStringA("Back Buffer failed");
    }

  Result = Device->CreateRenderTargetView(BackBuffer, nullptr, &RenderTargetView); 
  if (FAILED(Result))
    {
      OutputDebugStringA("Render target view failed");
    }
  
  ReleaseObject(BackBuffer);
  
  ID3D11Texture2D* DepthStencilBuffer = nullptr;
  
  D3D11_TEXTURE2D_DESC DepthStencilDesc;
  ZeroMemory(&DepthStencilDesc, sizeof(DepthStencilDesc));
  DepthStencilDesc.Width = ScreenWidth;
  DepthStencilDesc.Height = ScreenHeight;
  DepthStencilDesc.MipLevels = 1;
  DepthStencilDesc.ArraySize = 1;
  DepthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  DepthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  DepthStencilDesc.Usage = D3D11_USAGE_DEFAULT;

  if (MultiSamplingEnabled)
    {
      DepthStencilDesc.SampleDesc.Count = MultiSamplingCount;
      DepthStencilDesc.SampleDesc.Quality = MultiSamplingQualityLevels - 1;
    }
  else
    {
      DepthStencilDesc.SampleDesc.Count = 1;
      DepthStencilDesc.SampleDesc.Quality = 0;
    }
  
  Result = Device->CreateTexture2D(&DepthStencilDesc, nullptr, &DepthStencilBuffer);
  if (FAILED(Result))
    {
      OutputDebugStringA("Depth Stencil View failed");
    }

  
  Result = Device->CreateDepthStencilView(DepthStencilBuffer, nullptr, &DepthStencilView);
  if (FAILED(Result))
    {
      OutputDebugStringA("Depth stencil buffer failed");
    }
  
  DeviceContext->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);
  
  D3D11_VIEWPORT Viewport;
  Viewport.TopLeftX = 0.f;
  Viewport.TopLeftY = 0.f;
  Viewport.Width = (float)ScreenWidth;
  Viewport.Height = (float)ScreenHeight;
  Viewport.MinDepth = 0.f;
  Viewport.MaxDepth = 1.f;

  DeviceContext->RSSetViewports(1, &Viewport);

  ID3D11RasterizerState* RasterizerState = nullptr;

  D3D11_RASTERIZER_DESC RasterizerDesc;
  ZeroMemory(&RasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

  RasterizerDesc.FillMode = D3D11_FILL_SOLID;
  RasterizerDesc.CullMode = D3D11_CULL_BACK; // Try also D3D11_CULL_NONE 
  RasterizerDesc.FrontCounterClockwise = FALSE; // Try also TRUE 
  RasterizerDesc.DepthBias = 0;
  RasterizerDesc.DepthBiasClamp = 0;
  RasterizerDesc.SlopeScaledDepthBias = 0;
  RasterizerDesc.DepthClipEnable = TRUE;
  RasterizerDesc.ScissorEnable = FALSE;
  RasterizerDesc.MultisampleEnable = FALSE;
  RasterizerDesc.AntialiasedLineEnable = FALSE;

  Result = Device->CreateRasterizerState(&RasterizerDesc, &RasterizerState);
  if (FAILED(Result))
    {
      OutputDebugStringA("Rasterizer failed");
    }

  DeviceContext->RSSetState(RasterizerState);
}

// For shading
struct MatrixBufferType
{
  XMMATRIX World;
  XMMATRIX View;
  XMMATRIX Projection;
};

struct VertexType
{
  XMFLOAT3 position;
  XMFLOAT4 color; 
};

void CreateCube(HWND Window)
{
  HRESULT Result; 

  ID3DBlob* VertexShaderBlob = nullptr;
  ID3DBlob* PixelShaderBlob = nullptr;
  ID3DBlob* ErrorBlob = nullptr;
 
  LPCWSTR VSFileName = L"../source/color.vs";
  LPCWSTR PSFileName = L"../source/color.ps";
  
  ID3D11VertexShader* VertexShader;
  ID3D11PixelShader* PixelShader;
  
  Result = D3DCompileFromFile(VSFileName,
			      NULL,
			      NULL,
			      "ColorVertexShader",
			      "vs_5_0",
			      D3D10_SHADER_ENABLE_STRICTNESS,
			      0,
			      &VertexShaderBlob,
			      &ErrorBlob);
  if(FAILED(Result))
    {
      if (ErrorBlob)
	{
	  OutputDebugStringA((char*)ErrorBlob->GetBufferPointer());
	  ErrorBlob->Release();
	}
      else
	{
	  OutputDebugStringA("Could not compile from file");
	}
    }

  Result = Device->CreateVertexShader(VertexShaderBlob->GetBufferPointer(),
				      VertexShaderBlob->GetBufferSize(),
				      NULL,
				      &VertexShader);
  if(FAILED(Result))
    {
      OutputDebugStringA("Could not create vertex shader");
    }
  
  Result = D3DCompileFromFile(PSFileName,
			      NULL,
			      NULL,
			      "ColorPixelShader",
			      "ps_5_0",
			      D3D10_SHADER_ENABLE_STRICTNESS,
			      0,
			      &PixelShaderBlob,
			      &ErrorBlob);
  if(FAILED(Result))
    {
      if (ErrorBlob)
	{
	  OutputDebugStringA((char*)ErrorBlob->GetBufferPointer());
	  ErrorBlob->Release();
	}
      else
	{
	  OutputDebugStringA("Could not compile from file");
	}
    }
  
  Result = Device->CreatePixelShader(PixelShaderBlob->GetBufferPointer(),
				     PixelShaderBlob->GetBufferSize(),
				     NULL,
				     &PixelShader);
  if(FAILED(Result))
    {
      OutputDebugStringA("Could not create pixel shader"); 
    }
  
  D3D11_INPUT_ELEMENT_DESC PolygonLayout[2];
  ID3D11InputLayout* Layout; 
  int NumElements; 

  PolygonLayout[0].SemanticName = "POSITION";
  PolygonLayout[0].SemanticIndex = 0;
  PolygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
  PolygonLayout[0].InputSlot = 0;
  PolygonLayout[0].AlignedByteOffset = 0;
  PolygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  PolygonLayout[0].InstanceDataStepRate = 0;
  
  PolygonLayout[1].SemanticName = "COLOR";
  PolygonLayout[1].SemanticIndex = 0;
  PolygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
  PolygonLayout[1].InputSlot = 0;
  PolygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
  PolygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  PolygonLayout[1].InstanceDataStepRate = 0;

  NumElements = sizeof(PolygonLayout) / sizeof(PolygonLayout[0]);

  Result = Device->CreateInputLayout(PolygonLayout, NumElements,
				     VertexShaderBlob->GetBufferPointer(),
				     VertexShaderBlob->GetBufferSize(), &Layout);
  if (FAILED(Result))
    {
      OutputDebugStringA("Could not create layout"); 
    }
  
  if (VertexShaderBlob) VertexShaderBlob->Release();
  if (PixelShaderBlob) PixelShaderBlob->Release();
  if (ErrorBlob) VertexShaderBlob->Release();

  // UpdateRender   

  // Shader
  
  ID3D11Buffer* MatrixBuffer;

  D3D11_BUFFER_DESC BufferDesc;
  ZeroMemory(&BufferDesc, sizeof(BufferDesc));
  BufferDesc.ByteWidth = sizeof(MatrixBufferType);
  BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  BufferDesc.MiscFlags = 0;
  BufferDesc.StructureByteStride = 0;
  
  Result = Device->CreateBuffer(&BufferDesc, nullptr, &MatrixBuffer);
  if (FAILED(Result))
    {
      OutputDebugStringA("Could not create vertex shader");
      MatrixBuffer = nullptr;
    }

  // Cube Vertex

  D3D11_BUFFER_DESC VertexBufferDesc; 
  ID3D11Buffer *VertexBuffer; 
  int VertexCount = 3;
  VertexType* Vertices = new VertexType[VertexCount];

  Vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f); 
  Vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

  Vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f); 
  Vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

  Vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
  Vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

  ZeroMemory(&VertexBufferDesc, sizeof(VertexBufferDesc));
  VertexBufferDesc.ByteWidth = sizeof(VertexType) * VertexCount;
  VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  VertexBufferDesc.CPUAccessFlags = 0;
  VertexBufferDesc.MiscFlags = 0;
  VertexBufferDesc.StructureByteStride = 0;
  
  D3D11_SUBRESOURCE_DATA VertexData; 
  VertexData.pSysMem = Vertices;
  VertexData.SysMemPitch = 0;
  VertexData.SysMemSlicePitch = 0;
  
  Result = Device->CreateBuffer(&VertexBufferDesc, &VertexData, &VertexBuffer);
  if (FAILED(Result))
    {
      OutputDebugStringA("Could not create vertex buffer");
    }
  
  // Index Vertex

  D3D11_BUFFER_DESC IndexBufferDesc; 
  ID3D11Buffer *IndexBuffer; 
  int IndexCount = 3;
  unsigned long *Indices = new unsigned long[IndexCount];

  Indices[0] = 0;
  Indices[1] = 1;
  Indices[2] = 2;
  
  ZeroMemory(&IndexBufferDesc, sizeof(IndexBufferDesc));
  IndexBufferDesc.ByteWidth = sizeof(unsigned long) * IndexCount;
  IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  IndexBufferDesc.CPUAccessFlags = 0;
  IndexBufferDesc.MiscFlags = 0;
  IndexBufferDesc.StructureByteStride = 0;
  
  D3D11_SUBRESOURCE_DATA IndexData; 
  IndexData.pSysMem = Indices;
  IndexData.SysMemPitch = 0;
  IndexData.SysMemSlicePitch = 0;
  
  Result = Device->CreateBuffer(&IndexBufferDesc, &IndexData, &IndexBuffer);
  if (FAILED(Result))
    {
      OutputDebugStringA("Could not create index buffer");
    }

  delete[] Vertices;
  Vertices = 0;

  delete[] Indices;
  Indices = 0;

  unsigned int stride = sizeof(VertexType);
  unsigned int offset = 0;

  DeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
  DeviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

  DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  
  D3D11_MAPPED_SUBRESOURCE MappedResource; 
  MatrixBufferType* MatrixBufferTypePointer;
  unsigned int BufferNumber;
  // XMMATRIX WorldMatrix = ;
  // XMMATRIX ViewMatrix = ;
  // XMMATRIX ProjectionMatrix = ;
  
  Result = DeviceContext->Map(MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
  if (FAILED(Result))
    {
      OutputDebugStringA("Could not map matrix buffer");
    }

  MatrixBufferTypePointer = (MatrixBufferType*)MappedResource.pData;

  // MatrixBufferTypePointer->World = WorldMatrix;
  // MatrixBufferTypePointer->View = ViewMatrix;
  // MatrixBufferTypePointer->Projection = ProjectionMatrix;

  DeviceContext->Unmap(MatrixBuffer, 0);

  DeviceContext->VSSetConstantBuffers(0, 1, &MatrixBuffer); 
  
  DeviceContext->IASetInputLayout(Layout);

  DeviceContext->VSSetShader(VertexShader, NULL, 0);
  DeviceContext->PSSetShader(PixelShader, NULL, 0);

  int indexCount = 3;
  
  DeviceContext->DrawIndexed(indexCount, 0, 0);
}


LRESULT CALLBACK WindowProc(HWND Window, 
                            UINT Message, 
                            WPARAM WParam, 
                            LPARAM LParam)
{
  LRESULT Result = 0;

  switch (Message)
    {      
    case WM_CLOSE: 
      {
	Running = false;
      } break;

    case WM_DESTROY:
      {
	ReleaseObject(DeviceContext);
	ReleaseObject(RenderTargetView);
	ReleaseObject(DepthStencilView);
	ReleaseObject(SwapChain);
	ReleaseObject(Device);

	Running = false;		
      } break;
      
    default:
      {
	Result = DefWindowProc(Window, Message, WParam, LParam);
      } break;
    }
    
  return Result;
}

int WINAPI WinMain(HINSTANCE Instance,
		   HINSTANCE PrevInstance,
		   LPSTR CommandLine,
		   int ShowCode)
{
  WNDCLASSEXA wc = {};

  wc.cbSize = sizeof(WNDCLASSEXA);
  wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; 
  wc.lpfnWndProc = WindowProc;
  wc.hInstance = Instance;
  wc.hIcon = LoadIcon(Instance, MAKEINTRESOURCE(IDI_ICON1));;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.lpszClassName = "Lunora";

  HRESULT Result; 
  
  if (RegisterClassExA(&wc))
    {
      int x = CW_USEDEFAULT;
      int y = CW_USEDEFAULT;

      HWND Window = CreateWindowExA(0,                   
				    wc.lpszClassName,          
				    "Lunora",  
				    WS_OVERLAPPEDWINDOW | WS_VISIBLE,        
				    x, y, 1280, 720,
				    NULL,       
				    NULL,       
				    Instance,  
				    NULL);

      
  
      if (Window)
	{
	  InitializeDX11(Window);
	  CreateCube(Window); 
	  Running = true;
	  while(Running)
	    {
	      MSG Message;
	      while(PeekMessageA(&Message, 0, 0, 0, PM_REMOVE))
		{		  
		  if (Message.message == WM_QUIT)
		    {
		      Running = false; 
		    }

		  TranslateMessage(&Message);
		  DispatchMessageA(&Message);
		}
	      DeviceContext->ClearRenderTargetView(RenderTargetView, BackgroundColor);
	      DeviceContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	      SwapChain->Present(0, 0);
	    } 
	}
    }

  return 0;
}

