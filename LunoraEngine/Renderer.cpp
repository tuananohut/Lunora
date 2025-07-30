#include "Renderer.h"

ID3D11Buffer* Renderer::CreateVertexBuffer(UINT size,
					   bool dynamic,
					   bool streamout,
					   D3D11_SUBRESOURCE_DATA* pData)
{
  D3D11_BUFFER_DESC desc;
  dedc.ByteWidth = size;
  desc.MiscFlags = 0;
  desc.StructureByteStride = 0;

  // Selet the appropriate binding locations based on the passed in flags
  if (streamout)
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
  else
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

  // Select the appropriate usage and CPU access flags based on the passed
  // in flags
  if (dynamic)
    {
      desc.Usage = D3D11_USAGE_DYNAMIC;
      desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; 
    }
  else
    {
      desc.Usage = D3D11_USAGE_IMMUTABLE;
      desc.CPUAccessFlags = 0;
    }

  ID3D11Buffer *pBuffer = 0; 
  HRESULT result = m_Device->CreateBuffer(&desc, pData, &pBuffer);

  if (FAILED(result))
    {
      // Error handling
      return ( 0 );
    }

  return ( pBuffer );
}

ID3D11Buffer* Renderer::CreateIndexBuffer(UINT size,
					  bool dynamic,
					  D3D11_SUBRESOURCE_DATA* pData)
{
  D3D11_BUFFER_DESC desc;
  desc.ByteWidth = size;
  desc.MiscFlags = 0;
  desc.StructureByteStride = 0;
  desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

  if (dynamic)
    {
      desc.Usage = D3D11_USAGE_DYNAMIC;
      desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; 
    }
  else
    {
      desc.Usage = D3D11_USAGE_IMMUTABLE;
      desc.CPUAccessFlags = 0;
    }

  ID3D11Buffer *pBuffer = 0; 
  HRESULT result = m_Device->CreateBuffer(&desc, pData, &pBuffer);

  if (FAILED(result))
    {
      // Error handling
      return ( 0 );
    }

  return ( pBuffer );
}

ID3D11Buffer* Renderer::CreateConstantBuffer(UINT size,
					     bool dynamic,
					     bool CPUupdates,
					     D3D11_SUBRESOURCE_DATA *pData)
{
  D3D11_BUFFER_DESC desc;
  desc.ByteWidth = size;
  desc.MiscFlags = 0;
  desc.StructureByteStride = 0;
  desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

  if (dynamic && CPUupdates)
    {
      desc.Usage = D3D11_USAGE_DYNAMIC;
      desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; 
    }
  else if (dynamic && !CPUupdates)
    {
      desc.Usage = D3D11_USAGE_DEFAULT;
      desc.CPUAccessFlags = 0;
    }
  else
    {
      desc.Usage = D3D11_USAGE_IMMUTABLE;
      desc.CPUAccessFlags = 0;
    }

  ID3D11Buffer *pBuffer = 0; 
  HRESULT result = m_Device->CreateBuffer(&desc, pData, &pBuffer);

  if (FAILED(result))
    {
      // Error handling
      return ( 0 );
    }

  return ( pBuffer );

}

ID3D11Buffer* Renderer::CreateStructuredBuffer(UINT count,
					       UINT structsize,
					       bool CPUWritable,
					       bool GPUWritable,
					       D3D11_SUBRESOURCE_DATA* pData)
{
  D3D11_BUFFER_DESC desc;
  desc.ByteWidth = count * structsize;
  desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
  desc.StructureByteStride = structsize;

  if (!CPUWritable && !GPUWritable)
    {
      desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
      desc.Usage = D3D11_USAGE_IMMUTABLE;
      desc.CPUAccessFlags = 0; 
    }

  else if (CPUWritable &&  !GPUWritable)
    {
      desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
      desc.Usage = D3D11_USAGE_DYNAMIC;
      desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; 
    }

  else if (!CPUWritable && GPUWriteable)
    {
      desc.BindFlags = D3D11_BIND_SHADER_RESOURCE |
	               D3D11_BIND_UNORDERED_ACCESS;
      desc.Usage = D3D11_USAGE_DEFAULT;
      desc.CPUAccessFlags = 0;
    }
  else if (CPUWritable && GPUWritable)
    {
      // Handle the error here...
      // Resources can't be writable by both CPU and GPU simultaneously!
    }

  ID3D11Buffer* pBuffer = 0;
  HRESULT result = m_Device->CreateBuffer(&desc, pData, &pBuffer);

  if ( FAILED(result) )
    {
      // Handle the error here...
      return ( 0 );
    }

  return ( pBuffer );
}
