#include "ModelLoader.h"

bool LoadModelFromFile(const char* filename,
		       SimpleVertexCombined** outVertices,
		       int* outVertexCount)
{
  FILE* file = fopen(filename, "r");
  if (!file)
    {
      return false; 
    }

  XMFLOAT3 positions[1024];
  XMFLOAT4 colors[1024];
  int posCount = 0, colCount = 0;

  char type[2];
  while(fscanf(file, "%1s", type) == 1)
    {
      if (strcmp(type, "v") == 0)
	{
	  float x, y, z;
	  fscanf(file, "%f %f %f", &x, &y, &z);
	  positions[posCount++] = XMFLOAT3(x, y, z); 
	}
      else if (strcmp(type, "c") == 0)
	{
	  float r, g, b, a;
	  fscanf(file, "%f %f %f %f", &r, &g, &b, &a);
	  colors[colCount++] = XMFLOAT4(r, g, b, a);
	}
    }

  fclose(file);

  int count = (posCount < colCount) ? posCount : colCount;
  *outVertexCount = count;

  *outVertices = (SimpleVertexCombined*)malloc(sizeof(SimpleVertexCombined) * count);
  if (!*outVertices)
    {
      return false; 
    }

  for (int i = 0; i < count; ++i)
    {
      (*outVertices)[i].position = positions[i];
      (*outVertices)[i].color = colors[i];
    }

  return true; 
}
