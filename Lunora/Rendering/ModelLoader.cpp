#include "ModelLoader.h"
#include <cstdio>

bool LoadHemisphericModel(const char* filename,
			  std::vector<VertexHemispheric>& outVertices,
			  std::vector<uint32_t>& outIndices)
{
  FILE* file = fopen(filename, "r");
  if (!file)
    return false;

  char line[1024];

  while (fgets(line, sizeof(line), file))
    {
      if (line[0] == 'v' && line[1] == ' ')
        {
	  VertexHemispheric v{};
	  int read = sscanf(
			    line,
			    "v %f %f %f %f %f %f %f %f",
			    &v.position.x, &v.position.y, &v.position.z,
			    &v.uv.x, &v.uv.y,
			    &v.normal.x, &v.normal.y, &v.normal.z);

	  if (read == 8)
            {
	      outIndices.push_back((uint32_t)outVertices.size());
	      outVertices.push_back(v);
            }
        }
    }

  fclose(file);
  return !outVertices.empty();
}
