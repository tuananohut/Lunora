#include <stdint.h>
#include <windows.h>
#include <d3dcompiler.h>
#include <vector>

#include "resource.h"
#include "renderer.h"
#include "renderer.cpp"

using namespace std;

static bool Running;

static ID3D11VertexShader* VertexShader;
static ID3D11PixelShader* PixelShader;
static ID3D11Buffer *VertexBuffer; 
static ID3D11Buffer *IndexBuffer; 
static ID3D11Buffer *MatrixBuffer;
static ID3D11InputLayout *Layout;

struct Transform
{
  XMMATRIX Translation;
  XMMATRIX RotationMatrixX;
  XMMATRIX RotationMatrixY;
  XMMATRIX RotationMatrixZ;
  XMMATRIX RotationMatrix;
  XMMATRIX Scale; 
};

static float rotation = 0.f; 

static float moveX = 0.f; 
static float moveY = 0.f;
	
struct MatrixBufferType
{
  XMMATRIX World;
  XMMATRIX View;
  XMMATRIX Projection;
};

struct VertexBufferType
{
  XMFLOAT3 position;
  XMFLOAT4 color; 
};

struct Color
{
  float R;
  float G;
  float B;
  float A;
};

static std::vector<XMFLOAT4> VertexColors
  {
    XMFLOAT4(1.f, 0.f, 0.f, 1.f),
    XMFLOAT4(0.f, 1.f, 0.f, 1.f),
    XMFLOAT4(0.f, 0.f, 1.f, 1.f)
  };

static void ChangeColor(const Color& Color)
{
  VertexColors =
    {
      XMFLOAT4(Color.R, Color.G, Color.B, Color.A),
      XMFLOAT4(Color.R, Color.G, Color.B, Color.A),
      XMFLOAT4(Color.R, Color.G, Color.B, Color.A)
    };
}

static void ChangeColor(const std::vector<XMFLOAT4>& vertexColors)
{ 
  VertexColors[0] = vertexColors[0];
  VertexColors[1] = vertexColors[1];
  VertexColors[2] = vertexColors[2];
}

static void CreateCube(HWND Window, const LPCWSTR VSFileName, const LPCWSTR PSFileName)
{
  HRESULT Result; 

  ID3DBlob* VertexShaderBlob = nullptr;
  ID3DBlob* PixelShaderBlob = nullptr;
  ID3DBlob* ErrorBlob = nullptr;
  
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
  int VertexCount = 3;

  ZeroMemory(&VertexBufferDesc, sizeof(VertexBufferDesc));
  VertexBufferDesc.ByteWidth = sizeof(VertexBufferType) * VertexCount;
  VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC ;
  VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  VertexBufferDesc.MiscFlags = 0;
  VertexBufferDesc.StructureByteStride = 0;
  
  Result = Device->CreateBuffer(&VertexBufferDesc, nullptr, &VertexBuffer);
  if (FAILED(Result))
    {
      OutputDebugStringA("Could not create vertex buffer");
    }
  
  // Cube Index

  D3D11_BUFFER_DESC IndexBufferDesc;   
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

  delete[] Indices;
  Indices = 0;
}

static void RenderCube(ID3D11VertexShader *VertexShader,
		       ID3D11PixelShader *PixelShader,
		       int IndexCount,
		       ID3D11Buffer *MatrixBuffer,
		       XMMATRIX WorldMatrix,
		       XMMATRIX ViewMatrix,
		       XMMATRIX ProjectionMatrix,
		       ID3D11Buffer *VertexBuffer,
		       ID3D11Buffer *IndexBuffer,
		       ID3D11InputLayout *Layout)
{
  HRESULT Result;
  unsigned int stride = sizeof(VertexBufferType);
  unsigned int offset = 0;
  
  D3D11_MAPPED_SUBRESOURCE MappedResource; 
  
  MatrixBufferType* MatrixBufferTypePointer;
  unsigned int BufferNumber;

  VertexBufferType* VertexBufferTypePointer;

  WorldMatrix = XMMatrixTranspose(WorldMatrix);
  ViewMatrix = XMMatrixTranspose(ViewMatrix);
  ProjectionMatrix = XMMatrixTranspose(ProjectionMatrix);

  Result = DeviceContext->Map(VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
  if (FAILED(Result))
    {
      OutputDebugStringA("Could not map vertex buffer");
    }
   
  VertexBufferTypePointer = (VertexBufferType*)MappedResource.pData;

  // TODO: Automate 
  VertexBufferType vertexBufferType[3]; 
  vertexBufferType[0].position = XMFLOAT3(-2.f, -2.f, 0.f);
  
  // VertexBufferTypePointer[0].position = XMFLOAT3(-2.f, -2.f, 0.f);
  VertexBufferTypePointer[0].position = vertexBufferType[0].position;
  VertexBufferTypePointer[0].color = VertexColors[1];

  VertexBufferTypePointer[1].position = XMFLOAT3(-1.f, 2.f, 0.f);
  VertexBufferTypePointer[1].color = VertexColors[0]; 
  
  VertexBufferTypePointer[2].position = XMFLOAT3(0.f, -1.f, 0.f);
  VertexBufferTypePointer[2].color = VertexColors[2]; 

  DeviceContext->Unmap(VertexBuffer, 0);
 
  DeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
  DeviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
  
  DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  
  //
  
  Result = DeviceContext->Map(MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
  if (FAILED(Result))
    {
      OutputDebugStringA("Could not map matrix buffer");
    }
  
  MatrixBufferTypePointer = (MatrixBufferType*)MappedResource.pData;

  MatrixBufferTypePointer->World = WorldMatrix;
  MatrixBufferTypePointer->View = ViewMatrix;
  MatrixBufferTypePointer->Projection = ProjectionMatrix;
  
  DeviceContext->Unmap(MatrixBuffer, 0);
  
  DeviceContext->VSSetConstantBuffers(0, 1, &MatrixBuffer); 
  
  DeviceContext->IASetInputLayout(Layout);

  DeviceContext->VSSetShader(VertexShader, NULL, 0);
  DeviceContext->PSSetShader(PixelShader, NULL, 0);
  
  DeviceContext->DrawIndexed(IndexCount, 0, 0);
}

LRESULT CALLBACK WindowProc(HWND Window, 
                            UINT Message, 
                            WPARAM WParam, 
                            LPARAM LParam)
{
  LRESULT Result = 0;
  HRESULT ResultE;
  
  switch (Message)
    {
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_KEYDOWN:
    case WM_KEYUP:  
      {
	uint32_t VKCode = WParam;
	int32_t WasDown = ((LParam & (1 << 30)) != 0);
	int32_t IsDown = ((LParam & (1 << 31)) == 0);
	
	Color Red = {1.f, 0.f, 0.f, 1.f}; 
	Color Green = {0.f, 1.f, 0.f, 1.f}; 
	Color Blue = {0.f, 0.f, 1.f, 1.f}; 
	
	if (WasDown != IsDown)
	  {
	    if (VKCode == 'W')
	      {
		if(IsDown)
		  {
		    if (moveY >= 3.5f)
		      {
			moveY = 3.f;
		      }
		    else
		      {
			moveY += 1.f;
		      }
		  }
		
		ChangeColor(Red);
	      }

	    else if (VKCode == 'A')
	      {
		if (IsDown)
		  {
		    if (moveX <= -3.5f)
		      {
			moveX = -3.f; 
		      }
		    else
		      {
			moveX -= 1.f;
		      }
		  }

		ChangeColor(Green);
	      }
	    
	    else if (VKCode == 'S')
	      {
		if (IsDown)
		  {
		    if (moveY <= -3.5f)
		      {
			moveY = -3.f; 
		      }
		    else
		      {
			moveY -= 1.f;
		      }
		  }
		
		ChangeColor(Blue);
	      }
	    
	    else if (VKCode == 'D')
	      {
		if (IsDown)
		  {
		    if (moveX >= 3.5f)
		      {
			moveX = 3.f;
		      }
		    else
		      {
			moveX += 1.f; 
		      }
		  }
		
		std::vector<XMFLOAT4> vertexColors
		  {
		    XMFLOAT4(1.f, 0.f, 0.f, 1.f),
		    XMFLOAT4(0.f, 1.f, 0.f, 1.f),
		    XMFLOAT4(0.f, 0.f, 1.f, 1.f)
		  };
		
		ChangeColor(vertexColors);
	      }
	    
	    else if (VKCode == 'Q') {}
	    else if (VKCode == 'E') {}
	    else if (VKCode == VK_UP) {}
	    else if (VKCode == VK_LEFT) {}
	    else if (VKCode == VK_DOWN) {}
	    else if (VKCode == VK_RIGHT) {}
	    else if (VKCode == VK_ESCAPE) {}
	    else if (VKCode == VK_SPACE) {}
	  
	  }
	
	int32_t AltKeyWasDown = (LParam & (1 << 29));
	if ((VKCode == VK_F4) && AltKeyWasDown)
	  {
	    Running = false; 
	  } 
	
      }break;
      
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
	
	ReleaseObject(VertexShader);
	ReleaseObject(PixelShader);
	ReleaseObject(VertexBuffer);
	ReleaseObject(IndexBuffer);
	ReleaseObject(MatrixBuffer);
	ReleaseObject(Layout);
	
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
				    x, y, ScreenWidth, ScreenHeight,
				    NULL,       
				    NULL,       
				    Instance,  
				    NULL);

      
  
      if (Window)
	{
	  InitializeDX11(Window);
	  
	  CreateCube(Window, VSFileName, PSFileName);

	  WorldMatrix = XMMatrixIdentity();
	  ViewMatrix = XMMatrixLookAtLH
	    (
	     XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f), // -1
	     XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
	     XMVectorSet(0.0f, 10.0f, 0.0f, 0.0f)
	     );
	  ProjectionMatrix = XMMatrixPerspectiveFovLH
	    (
	     XM_PIDIV4,
	     ScreenWidth / ScreenHeight,
	     0.1f,
	     1000.0f
	     );
	  
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
	      DeviceContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0);
	      
	      const float half_gravity = 5.f;
	      static float height = 0.35f;
	      static float time = 0.0174532925f; // 1.f/60.f;
	      
	      height -= (half_gravity * time * time) + (0.0025f); 
	      if (height < -0.5f)
		{
		  height = 0.5f;
		}
	      
	      rotation -= 0.0174532925f * 2.f;
	      if(rotation < 0.0f)
		{
		  rotation += 360.0f;
		}

	      Transform CubeTransform;
	      
	      CubeTransform.Translation = XMMatrixTranslation(moveX, moveY, 0.f);

	      CubeTransform.RotationMatrixX = XMMatrixRotationX(rotation);
	      CubeTransform.RotationMatrixY = XMMatrixRotationY(rotation);
	      CubeTransform.RotationMatrixZ = XMMatrixRotationZ(rotation);

	      CubeTransform.RotationMatrix = XMMatrixMultiply(CubeTransform.RotationMatrixX, CubeTransform.RotationMatrixY);
	      CubeTransform.RotationMatrix = XMMatrixMultiply(CubeTransform.RotationMatrix, CubeTransform.RotationMatrixZ);
	      
	      XMMATRIX TranslationAndRotation = XMMatrixMultiply(CubeTransform.RotationMatrix, CubeTransform.Translation);
	      
	      WorldMatrix = TranslationAndRotation; 
	      
	      RenderCube(VertexShader,
			 PixelShader,
			 3,
			 MatrixBuffer,
			 WorldMatrix,
			 ViewMatrix,
			 ProjectionMatrix,
			 VertexBuffer,
			 IndexBuffer,
			 Layout);

	      Transform CubeTransform2;
	      CubeTransform2.Translation = XMMatrixTranslation(-1.f, 0.f, 0.f);
	      CubeTransform2.RotationMatrixX = XMMatrixRotationX(rotation);

	      CubeTransform2.RotationMatrix = XMMatrixMultiply(CubeTransform2.Translation, CubeTransform2.RotationMatrixX); 

	      WorldMatrix = CubeTransform2.RotationMatrix;
			      
	      RenderCube(VertexShader,
			 PixelShader,
			 3,
			 MatrixBuffer,
			 WorldMatrix,
			 ViewMatrix,
			 ProjectionMatrix,
			 VertexBuffer,
			 IndexBuffer,
			 Layout);

	      
	      SwapChain->Present(1, 0);
	    } 
	}
    }
   
  return 0;
}

