#define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "../external/tiny_gltf.h"


void ProcessNode(DeviceManager& deviceManager,
		 RenderManager* renderer,
		 MeshGPUData& meshData,
		 ShaderGPUData& shaderData,
		 const tinygltf::Model& model,
                 const tinygltf::Node& node,
                 XMMATRIX parentTransform);

bool LoadGLTF(const std::string& filepath,
	      MeshGPUData& meshData,
	      ShaderGPUData& shaderData,
	      DeviceManager& deviceManager,
	      RenderManager* renderer)
{  
  tinygltf::TinyGLTF loader; 
  tinygltf::Model model;
  
  std::string err, warn;

  bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, filepath);
  if (!ret)
    {
      OutputDebugStringA("Load Binary From File!");
      return false; 
    }

  if (!warn.empty())
    {
      OutputDebugStringA("Warn empty!"); 
      return false; 
    }

  const auto& scene = model.scenes[model.defaultScene];
  for (int nodeIndex: scene.nodes)
    {
      ProcessNode(deviceManager,
		  renderer,
		  meshData,
		  shaderData,
		  model,
		  model.nodes[nodeIndex],
		  XMMatrixIdentity()); 
    }
  
  return true; 
}


void ProcessNode(DeviceManager& deviceManager,
		 RenderManager* renderer,
		 MeshGPUData& meshData,
		 ShaderGPUData& shaderData,
		 const tinygltf::Model& model,
                 const tinygltf::Node& node,
                 XMMATRIX parentTransform)
{
  XMMATRIX localTransform = XMMatrixIdentity();

  if (!node.matrix.empty())
    {
      localTransform = XMMATRIX(
				node.matrix[0], node.matrix[1], node.matrix[2], node.matrix[3],
				node.matrix[4], node.matrix[5], node.matrix[6], node.matrix[7],
				node.matrix[8], node.matrix[9], node.matrix[10], node.matrix[11],
				node.matrix[12], node.matrix[13], node.matrix[14], node.matrix[15]
				);
    }
  else
    {
      XMMATRIX t = XMMatrixTranslation(node.translation[0], node.translation[1], node.translation[2]);
      XMMATRIX r = XMMatrixRotationQuaternion(XMLoadFloat4((XMFLOAT4*)&node.rotation[0]));
      XMMATRIX s = XMMatrixScaling(node.scale[0], node.scale[1], node.scale[2]);
      localTransform = s * r * t;
    }

  XMMATRIX globalTransform = localTransform * parentTransform;

  if (node.mesh >= 0)
    {
      const auto& mesh = model.meshes[node.mesh];
      for (const auto& primitive : mesh.primitives)
        {
	  const auto& posAccessor = model.accessors[primitive.attributes.at("POSITION")];
	  const auto& indexAccessor = model.accessors[primitive.indices];

	  size_t vertexCount = posAccessor.count;
	  size_t indexCount = indexAccessor.count;

	  VertexBufferType* vertices = new VertexBufferType[vertexCount];
	  unsigned long* indices = new unsigned long[indexCount];

	  const auto& posView = model.bufferViews[posAccessor.bufferView];
	  const auto& posBuffer = model.buffers[posView.buffer];
	  const float* positions = reinterpret_cast<const float*>(&posBuffer.data[posView.byteOffset]);
	  
	  VertexBufferType* vertices = new VertexBufferType[vertexCount];

	  for (size_t i = 0; i < vertexCount; i++)
	    {
	      XMFLOAT3 pos = XMFLOAT3(positions[i * 3], positions[i * 3 + 1], positions[i * 3 + 2]);

	      XMVECTOR p = XMLoadFloat3(&pos);
	      p = XMVector3Transform(p, globalTransform);
	      XMStoreFloat3(&vertices[i].position, p);
	    }

	  // Normal verisi varsa oku
	  if (primitive.attributes.find("NORMAL") != primitive.attributes.end())
	    {
	      const auto& normAccessor = model.accessors[primitive.attributes.at("NORMAL")];
	      const auto& normView = model.bufferViews[normAccessor.bufferView];
	      const auto& normBuffer = model.buffers[normView.buffer];
	      const float* normals = reinterpret_cast<const float*>(&normBuffer.data[normView.byteOffset]);

	      for (size_t i = 0; i < normAccessor.count && i < vertexCount; i++)
		{
		  vertices[i].normal = XMFLOAT3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]);
		}
	    }

	  const auto& indexAccessor = model.accessors[primitive.indices];
	  const auto& indexView = model.bufferViews[indexAccessor.bufferView];
	  const auto& indexBuffer = model.buffers[indexView.buffer];

	  const size_t indexCount = indexAccessor.count;

	  if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
	    {
	      const unsigned short* idx = reinterpret_cast<const unsigned short*>(&indexBuffer.data[indexView.byteOffset]);
	      for (size_t i = 0; i < indexCount; i++)
		indices[i] = static_cast<unsigned long>(idx[i]);
	    }
	  else if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
	    {
	      const unsigned int* idx = reinterpret_cast<const unsigned int*>(&indexBuffer.data[indexView.byteOffset]);
	      for (size_t i = 0; i < indexCount; i++)
		indices[i] = static_cast<unsigned long>(idx[i]);
	    }
	  else
	    {
	      OutputDebugStringA("Unsupported index component type.\n");
	      delete[] vertices;
	      continue;
	    }

	  BuildMesh(deviceManager, renderer, meshData, shaderData,
		    vertices, vertexCount,
		    indices, indexCount,
		    VSFileName, PSFileName);

	  delete[] vertices;
	  delete[] indices;
	}
    }

  for (int childIndex : node.children)
    {
      ProcessNode(deviceManager,
		  renderer,
		  meshData,
		  shaderData,
		  model,
		  model.nodes[childIndex],
		  globalTransform);
    }
}
