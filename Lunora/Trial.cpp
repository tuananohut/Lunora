#define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "../external/tiny_gltf.h"

tinygltf::Model model;
tinygltf::TinyGLTF loader;
std::string err, warn;

bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, "scene.gltf");

if (!warn.empty())
  {
    std::cout << "Warning: " << warn << std::endl;
  }

if (!err.empty())
  {
    std::cerr << "Error: " << err << std::endl;
  }

if (!ret)
  {
    std::cerr << "Failed to load glTF file!" << std::endl;
    return;
  }

std::cout << "Loaded glTF successfully!" << std::endl;
