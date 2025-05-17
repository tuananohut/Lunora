#include <cmath>
#include <cstdlib>

#include "HeightMapGenerator.h"

float ValueNoise(int x, int y)
{
  int n = x + y * 57;
  n = (n << 13) ^ n;

  return (1.f - (( n * ( n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.f);
}

float SmoothNoise(int x, int y)
{
  float corners = (ValueNoise(x-1, y-1) + ValueNoise(x+1, y-1) + ValueNoise(x-1, y+1) + ValueNoise(x+1, y+1)) / 16.f;
  float sides = (ValueNoise(x-1, y) + ValueNoise(x+1, y) + ValueNoise(x, y-1) + ValueNoise(x, y+1)) / 8.f;
  float center = ValueNoise(x, y) / 4.f;

  return corners + sides + center; 
}

float Interpolate(float a, float b, float t)
{
  float ft = t * 3.1415927f;
  float f = (1 - cosf(ft)) * 0.5f;

  return a * (1 - f) + b * f;
}

float InterpolatedNoise(float x, float y)
{
  int intX = int(x);
  int intY = int(y);
  float fracX = x - intX;
  float fracY = y - intY;

  float v1 = SmoothNoise(intX, intY);
  float v2 = SmoothNoise(intX + 1, intY);
  float v3 = SmoothNoise(intX, intY + 1);
  float v4 = SmoothNoise(intX + 1, intY + 1);

  float i1 = Interpolate(v1, v2, fracX);
  float i2 = Interpolate(v3, v4, fracX);

  return Interpolate(i1, i2, fracY);
}

std::vector<float> HeightMapGenerator::Generate(int width,
						int height,
						float scale)
{
  std::vector<float> heightMap(width * height);

  for (int z = 0; z < height; ++z)
    {
      for (int x = 0; x < width; ++x)
	{
	  float nx = x / scale;
	  float nz = z / scale;
	  float noise = InterpolatedNoise(nx, nz);
	  heightMap[z * width + x] = noise;
	}
    }

  return heightMap; 
}
