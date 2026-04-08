#include"Screen.h"

using namespace Screen;

ScreenInfo::ScreenInfo()
{
    float x = GetScreenWidth();
    float y = GetScreenHeight();

    size = Vector2{x, y};
    center = Vector2{x/2, y/2};
    ratio = x / y;
}

void ScreenInfo::SetBackgroundColor(Color c)
{
    color = c;
}

Vector2 ScreenInfo::GetDrawSpacePosition(Vector2 mouseScreenPos)
{
    Vector2 pos = mouseScreenPos / this->zoom.value.Get(); // Zoom scaling
    pos = pos + this->mouseOffset.value.Get(); // Mouse offset
    return pos;
}

