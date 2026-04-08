#pragma once
#include<raylib.h>
#include<raymath.h>
#include"AnimationSystem.hpp"

using namespace AnimationSystem;

namespace Screen
{
    struct MouseOffset
    {
        Vector2 referencePoint;
        AnimatableVector2 value;
    };

    struct Zoom
    {
        float min = 0.5f;
        float max = 2.0f;
        AnimatableValue value;
    };

    class ScreenInfo
    {
        private:
            Color color;

        public:
            Vector2 size;
            Vector2 center;
            Vector2 centeroffset;
            float ratio;
            
            Zoom zoom;
            MouseOffset mouseOffset;

            ScreenInfo();

            void SetBackgroundColor(Color c);
            Vector2 GetDrawSpacePosition(Vector2 mouseScreenPos); // Gets the corresponding position in the world you are drawing in by using mouseOffset (and zooming) and mouse position
    };

}
