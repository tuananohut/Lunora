#include <Lunora.h>

struct Sandbox : public Lunora::Application
{
  Sandbox() {}
  ~Sandbox() {}
};

Lunora::Application* Lunora::CreateApplication()
{
  return new Sandbox();
}
