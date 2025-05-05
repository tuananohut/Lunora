#define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "../external/tiny_gltf.h"

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
      return false; 
    }

  if (!warn.empty())
    {
      return false; 
    }

  for (const auto& mesh: model.meshes)
    {
      for (const auto& primitive: mesh.primitives)
	{
	  VertexBufferType vertices[1000] = {};
	  unsigned long indices[1000] = {};
	  size_t vertexCount = 0;
	  size_t indexCount = 0; 
	  
 	  const auto& positionAccessor = model.accessors[primitive.attributes.find("POSITION")->second];
	  const auto& positionBufferView = model.bufferViews[positionAccessor.bufferView];
	  const auto& positionBuffer = model.buffers[positionBufferView.buffer];
	  const float* positions = reinterpret_cast<const float*>(&positionBuffer.data[positionBufferView.byteOffset]);

	  for (size_t i = 0; i < positionAccessor.count; i++)
	    {
	      VertexBufferType vertex;
	      vertex.position = XMFLOAT3(positions[i * 3], positions[i * 3 + 1], positions[i * 3 + 2]);
	      vertices[vertexCount++] = vertex; 
	    }
	  
	  const auto& normalAccessor = model.accessors[primitive.attributes.find("NORMAL")->second];
	  const auto& normalBufferView = model.bufferViews[normalAccessor.bufferView];
	  const auto& normalBuffer = model.buffers[normalBufferView.buffer];
	  const float* normals = reinterpret_cast<const float*>(&normalBuffer.data[normalBufferView.byteOffset]);

	  size_t normalIndex = 0;
	  for (size_t i = 0; i < normalAccessor.count; i++)
            {
	      vertices[normalIndex].normal = XMFLOAT3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]);
	      normalIndex++;
            }

	  const auto& indexAccessor = model.accessors[primitive.indices];
	  const auto& indexBufferView = model.bufferViews[indexAccessor.bufferView];
	  const auto& indexBuffer = model.buffers[indexBufferView.buffer];
	  const unsigned short* idx = reinterpret_cast<const unsigned short*>(&indexBuffer.data[indexBufferView.byteOffset]);

	  for (size_t i = 0; i < indexAccessor.count; i++)
	    {
	      indices[indexCount++] = static_cast<unsigned long>(idx[i]);
	    }

	  BuildMesh(deviceManager, renderer, meshData, shaderData, vertices, vertexCount, indices, indexCount, VSFileName, PSFileName); 
	}
    }

  return true; 
}
