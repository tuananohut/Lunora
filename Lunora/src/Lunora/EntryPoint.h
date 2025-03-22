#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#ifdef LNR_BUILD_DLL

extern Lunora::Application* Lunora::CreateApplication();

int main(int argc, char** argv)
{
  auto app = Lunora::CreateApplication();
  app->Run();
  delete app;
}

#endif

#endif
