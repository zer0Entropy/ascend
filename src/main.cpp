#include "include/core/app.hpp"

auto main() -> int
{
  Application&            appInstance{Application::GetInstance()};

  do {
    appInstance.Update();
  } while(appInstance.GetStatus() == Application::StatusID::Running);
  return 0;
}
