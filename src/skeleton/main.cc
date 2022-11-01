#include "skeleton/scene/scene.hh"

int main() {
  auto delta = Delta::CreateInstance();

  Delta::WindowInfo window_info;
  window_info.title = "Skeleton";
  auto window = delta->CreateWindow(window_info);

  Skeleton::Scene scene(delta, window);

  while (window->IsOpen()) {
    window->PollEvents();
    scene.Render();
  }
}
