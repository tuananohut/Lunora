#include <windows.h>
#include <d3dcompiler.h>

#include "managers.cpp"
#include "resource.h"
#include "renderer.cpp"
#include "Renderer/Shader/ShaderSystem.cpp"
#include "Renderer/Mesh/MeshBuilder.cpp"
#include "Renderer/Mesh/MeshFactory.cpp"

using namespace std;

static bool Running;

static RenderManager* Renderer;
static Camera Camera;

static float rotation = 0.f; 	

ID3D11SamplerState* TextureSamplerState;

void AssignXMFLOAT4(XMFLOAT4& dest, const XMFLOAT4* src)
{
  if (src != nullptr)
    {
      dest.x = src->x;
      dest.y = src->y;
      dest.z = src->z;
      dest.w = src->w;
    }
}

static void ChangeColor(const XMFLOAT4& Color)
{
  ambientColor = Color; 
}

static void CreateCube(DeviceManager& DeviceManager,
		       HWND Window,
		       ShaderGPUData* Shader,
		       MeshGPUData* Mesh,
		       const LPCWSTR VSFilename,
		       const LPCWSTR PSFilename)
{
  HRESULT Result;
  bool result;

  if (!LoadShader(DeviceManager.Device,
		  VSFilename, PSFilename,
		  Shader, Renderer))
    {
      OutputDebugStringA("Could not load shader!");
    }
  
  D3D11_BUFFER_DESC BufferDesc;
  ZeroMemory(&BufferDesc, sizeof(BufferDesc));
  BufferDesc.ByteWidth = sizeof(MatrixBufferType);
  BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  BufferDesc.MiscFlags = 0;
  BufferDesc.StructureByteStride = 0;
  
  Result = DeviceManager.Device->CreateBuffer(&BufferDesc, nullptr, &Mesh->MatrixBuffer);
  if(FAILED(Result))
    {
      OutputDebugStringA("Could not create matrix buffer.");
      Mesh->MatrixBuffer = nullptr;
    }

  D3D11_BUFFER_DESC LightBufferDesc;
  ZeroMemory(&LightBufferDesc, sizeof(LightBufferDesc));
  LightBufferDesc.Usage = D3D11_USAGE_DYNAMIC; 
  LightBufferDesc.ByteWidth = sizeof(LightBufferType);
  LightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  LightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  LightBufferDesc.MiscFlags = 0;
  LightBufferDesc.StructureByteStride = 0;

  Result = DeviceManager.Device->CreateBuffer(&LightBufferDesc, nullptr, &Shader->LightBuffer);
  if (FAILED(Result))
    {
      OutputDebugStringA("Could not create light buffer.");
      Shader->LightBuffer = nullptr; 
    }
  else
    {
      OutputDebugStringA("Light buffer created successfully.\n");
    }
  
  // Cube Vertex
  D3D11_BUFFER_DESC VertexBufferDesc; 
  int VertexCount = 3;

  ZeroMemory(&VertexBufferDesc, sizeof(VertexBufferDesc));
  VertexBufferDesc.ByteWidth = sizeof(VertexBufferType) * VertexCount;
  VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  VertexBufferDesc.MiscFlags = 0;
  VertexBufferDesc.StructureByteStride = 0;
  
  Result = DeviceManager.Device->CreateBuffer(&VertexBufferDesc, nullptr, &Mesh->VertexBuffer);
  if (FAILED(Result))
    {
      OutputDebugStringA("Could not create vertex buffer");
    }

  VertexBufferType Vertices[3];
  Vertices[0].position = XMFLOAT3(-2.f, -2.f, 0.f);
  Vertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

  Vertices[1].position = XMFLOAT3(-1.f, 2.f, 0.f);
  Vertices[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

  Vertices[2].position = XMFLOAT3(0.f, -1.f, 0.f);
  Vertices[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

  D3D11_SUBRESOURCE_DATA VertexData;
  VertexData.pSysMem = Vertices;
  VertexData.SysMemPitch = 0;
  VertexData.SysMemSlicePitch = 0;

  Result = DeviceManager.Device->CreateBuffer(&VertexBufferDesc, &VertexData, &Mesh->VertexBuffer);

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
  
  Result = DeviceManager.Device->CreateBuffer(&IndexBufferDesc, &IndexData, &Mesh->IndexBuffer);
  if (FAILED(Result))
    {
      OutputDebugStringA("Could not create index buffer");
    }

  delete[] Indices;
  Indices = 0;
}

static void RenderCube(DeviceManager& DeviceManager, 
		       ShaderGPUData* Shader, 
		       MeshGPUData* Mesh,
		       XMMATRIX WorldMatrix,
		       XMMATRIX ViewMatrix,
		       XMMATRIX ProjectionMatrix)
{
  HRESULT Result;
  unsigned int stride = sizeof(VertexBufferType);
  unsigned int offset = 0;
  
  D3D11_MAPPED_SUBRESOURCE MappedResource; 
  
  MatrixBufferType* MatrixBufferTypePointer;
  LightBufferType* LightBufferTypePointer;
  unsigned int BufferNumber;

  VertexBufferType* VertexBufferTypePointer;
  
  WorldMatrix = XMMatrixTranspose(WorldMatrix);
  ViewMatrix = XMMatrixTranspose(ViewMatrix);
  ProjectionMatrix = XMMatrixTranspose(ProjectionMatrix);
 
  DeviceManager.DeviceContext->IASetVertexBuffers(0, 1, &Mesh->VertexBuffer, &stride, &offset);
  DeviceManager.DeviceContext->IASetIndexBuffer(Mesh->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
   
  auto triangle_list = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
  auto triangle_strip = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
  
  DeviceManager.DeviceContext->IASetPrimitiveTopology(triangle_list);
  
  // Matrix Buffer Mapping
  
  Result = DeviceManager.DeviceContext->Map(Mesh->MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
  if (FAILED(Result))
    {
      OutputDebugStringA("Could not map matrix buffer");
    }
  
  MatrixBufferTypePointer = (MatrixBufferType*)MappedResource.pData;

  MatrixBufferTypePointer->World = WorldMatrix;
  MatrixBufferTypePointer->View = ViewMatrix;
  MatrixBufferTypePointer->Projection = ProjectionMatrix;
  
  DeviceManager.DeviceContext->Unmap(Mesh->MatrixBuffer, 0);

  // Light Buffer Mapping
  if (!Shader->LightBuffer)
    {
      OutputDebugStringA("LightBuffer is null before Map! Was it created?\n");
    }
  
  Result = DeviceManager.DeviceContext->Map(Shader->LightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);

  LightBufferTypePointer = (LightBufferType*)MappedResource.pData;

  LightBufferTypePointer->ambientColor = ambientColor;
  LightBufferTypePointer->diffuseColor = diffuseColor;
  LightBufferTypePointer->lightDirection = lightDirection;
  LightBufferTypePointer->padding = 0.f;

  DeviceManager.DeviceContext->Unmap(Shader->LightBuffer, 0);
  
  DeviceManager.DeviceContext->VSSetConstantBuffers(0, 1, &Mesh->MatrixBuffer); 
  DeviceManager.DeviceContext->PSSetConstantBuffers(0, 1, &Shader->LightBuffer); 
  UseShader(DeviceManager.DeviceContext, Shader);
  
  DeviceManager.DeviceContext->DrawIndexed(Mesh->indexCount, 0, 0);
}

static bool KeyW;
static bool KeyA;
static bool KeyD;
static bool KeyS;

void UpdateInput()
{
    if (KeyW)
      {
        Camera.m_positionZ += 0.1f;
        ChangeColor(XMFLOAT4{1.f, 0.f, 0.f, 1.f}); 
    }
    if (KeyA)
    {
        Camera.m_positionX += 0.1f;
        ChangeColor(XMFLOAT4{0.f, 1.f, 0.f, 1.f}); 
    }
    if (KeyS)
    {
        Camera.m_positionZ -= 0.1f;
        ChangeColor(XMFLOAT4{0.f, 0.f, 1.f, 1.f}); 
    }
    if (KeyD)
    {
        Camera.m_positionX -= 0.1f;
        ChangeColor(XMFLOAT4{0.15f, 0.15f, 0.15f, 1.f});
    }
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

	switch(VKCode)
	  {
	  case 'W':
	    {
	      KeyW = IsDown;
	    } break;
	  case 'A':
	    {
	      KeyA = IsDown;
	    } break;
	  case 'S':
	    {
	      KeyS = IsDown;
	    } break;
	  case 'D':
	    {
	      KeyD = IsDown;
	    } break; 
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
	  DeviceManager DeviceManager;
	  DeviceManager.Initialize(Window, ScreenWidth, ScreenHeight);
	  
	  RenderTargetManager RenderTargetManager;
	  RenderTargetManager.Initialize(DeviceManager, ScreenWidth, ScreenHeight);
	  
	  PipelineStateManager PipelineStateManager; 
	  PipelineStateManager.Initialize(DeviceManager);
	  
	  static ShaderGPUData Shader;	  
	  Renderer->InitializeLayouts();

	  static Scene Scene;
	  MeshGPUData Mesh;
	  Mesh.indexCount = 3;
	  
	  InitializeDX11(DeviceManager,
			 RenderTargetManager,
			 PipelineStateManager,
			 Window);
	  
	  CreateCube(DeviceManager, Window, &Shader, &Mesh,
		     VSFileName, PSFileName);
	  
	  MeshGPUData Quad;
	  CreateUnitQuad(DeviceManager,
			 Renderer,
			 Quad,
			 Shader,
			 VSFileName,
			 PSFileName);
	  

	  MeshGPUData Cube;
	  CreateUnitCube(DeviceManager,
			 Renderer,
			 Cube,
			 Shader,
			 VSFileName,
			 PSFileName); 
	  /*
	  std::string filepath = "../Lunora/Assets/Scenes/trial_scene.glb";
	  MeshGPUData TrialScene;
	  if (!LoadGLTF(filepath, TrialScene, Shader, DeviceManager, Renderer))
	    {
	      MessageBoxA(Window, "Could not load scene!", "Error!", MB_ICONWARNING);
	      // return false; 
	    }
	  TrialScene.Transform = TransformSystem::Identity();
	  AddMesh(&Scene.Meshes, &TrialScene);
	  */
	  
	  Mesh.Transform = TransformSystem::Identity();
	  AddMesh(&Scene.Meshes, &Mesh);
	  
	  Quad.Transform = TransformSystem::Identity();
	  AddMesh(&Scene.Meshes, &Quad);

	  Cube.Transform = TransformSystem::Identity();
	  AddMesh(&Scene.Meshes, &Cube);
	  	  
	  WorldMatrix = XMMatrixIdentity();

	  Camera.m_positionX = 0.f;
	  Camera.m_positionY = 0.f;
	  Camera.m_positionZ = -10.f;
	  	  
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
		      for (int i = 0; i < Scene.Meshes.Count; ++i)
			{
			  auto* m = Scene.Meshes.Meshes[i];
			  ReleaseObject(m->VertexBuffer);
			  ReleaseObject(m->IndexBuffer);
			  ReleaseObject(m->MatrixBuffer);
			}
		      
		      ReleaseObject(Mesh.VertexBuffer);
		      ReleaseObject(Mesh.IndexBuffer);
		      ReleaseObject(Mesh.MatrixBuffer);

		      // ReleaseShader(Shader*);
		      
		      DeviceManager.Cleanup();
		      RenderTargetManager.Cleanup();
		      PipelineStateManager.Cleanup();	     
		      
		      Running = false;
		    }
		  
		  TranslateMessage(&Message);
		  DispatchMessageA(&Message);
		}
	      
	      DeviceManager.DeviceContext->ClearRenderTargetView(RenderTargetManager.RenderTargetView, BackgroundColor);
	      DeviceManager.DeviceContext->ClearDepthStencilView(RenderTargetManager.DepthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0);

	      UpdateInput();
	      
	      Camera.Render();
	      Camera.GetViewMatrix(ViewMatrix); 
	      
	      static float rotation = 0.f;
	      const float half_gravity = 5.f;
	      static float height = 0.35f;
	      static float time = 0.0174532925f;

	      height -= (half_gravity * time * time) + (0.0025f);
	      if (height < -0.5f)
		height = 0.5f;

	      rotation -= 0.0174532925f * 2.f;
	      if (rotation < 0.0f)
		rotation += 360.0f;
	      
	      if (Scene.Meshes.Count > 0)
		{
		  auto* animatedMesh = Scene.Meshes.Meshes[0];
   
		  animatedMesh->Transform = TransformSystem::Identity();
		  animatedMesh->Transform.Translation = XMMatrixTranslation(0.0f, height, 0.0f);
		  
		  animatedMesh->Transform.RotationMatrixX = XMMatrixRotationX(rotation);
		  animatedMesh->Transform.RotationMatrixY = XMMatrixRotationY(rotation);
		  animatedMesh->Transform.RotationMatrixZ = XMMatrixRotationZ(rotation);
		  
		  animatedMesh->Transform.RotationMatrix =
		    animatedMesh->Transform.RotationMatrixX *
		    animatedMesh->Transform.RotationMatrixY *
		    animatedMesh->Transform.RotationMatrixZ;
		  
		}
	      
	      RenderScene(DeviceManager,
			  &Shader,
			  &Scene,
			  ViewMatrix,
			  ProjectionMatrix);
	      
	      DeviceManager.SwapChain->Present(1, 0);
	    } 
	}
    }
   
  return 0;
}
