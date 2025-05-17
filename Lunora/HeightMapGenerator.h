#ifndef HEIGHTMAPGENERATOR_H
#define HEIGHTMAPGENERATOR_H

#include <vector>

struct HeightMapGenerator
{
  static std::vector<float> Generate(int width, int height, float scale);  
};

#endif
