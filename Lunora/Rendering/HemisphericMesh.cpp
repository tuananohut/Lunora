#include "HemisphericMesh.h"

bool HemisphericMeshLoadFromFile(HemisphericMesh* Buffer)
{
  assert(Buffer && Buffer->filename);

  FILE* file = fopen(Buffer->filename, "r");
  if (!file)
    return false;

  XMFLOAT3 positions[14700];
  XMFLOAT3 normals[14700];
  XMFLOAT2 texcoords[14700];
  
  int posCount = 0;
  int normalCount = 0;
  int texCount = 0;

  char line[15000];

  while (fgets(line, sizeof(line), file))
    {
      if (line[0] == 'v' && line[1] == ' ')
	{
	  float x, y, z;
	  float tx, ty; 
	  float nx, ny, nz;
	  int read = sscanf(line, "v %f %f %f %f %f %f %f %f",
			    &x, &y, &z, &tx, &ty, &nx, &ny, &nz);
	  
	  if (read == 8)
	    {
	      positions[posCount++] = XMFLOAT3(x, y, z);	    
	      normals[normalCount++] = XMFLOAT3(nx, ny, nz); 
	      texcoords[texCount++] = XMFLOAT2(tx, ty); 	    
	    }
	}
    }

  fclose(file);

  if (posCount == 0 || posCount != normalCount)
    return false;

  Buffer->vertexCount = posCount;
  Buffer->indexCount  = Buffer->vertexCount;

  Buffer->vertices = new HemisphericVertex[Buffer->vertexCount];
  Buffer->indices  = new uint32_t[Buffer->indexCount];

  for (int i = 0; i < posCount; ++i)
    {
      Buffer->vertices[i].position = positions[i];
      Buffer->vertices[i].normal   = normals[i];
      Buffer->vertices[i].texture  = texcoords[i];

      Buffer->indices[i] = i;
    }

  return true;
}
 
