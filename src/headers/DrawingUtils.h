#pragma once
#include<raylib.h>
#include"Screen.h"

using namespace Screen;

class Pensel
{
    public:
        int arraySize = 80000;
        Vector2 points[80000];
        int lastAddedIndex = 0;
        ScreenInfo* screenInfo;
        Color color;
        bool enabled = true;

        Pensel();

        void AddPoint(Vector2 point);



        void DrawPoints();

        void CreateBreak();
};

class Eraser
{
    private:
        Vector2 position;
        float radius = 20.0f;

    public:
        bool enabled = false;

        void Erase(Pensel* p);
        void Render();
        void SetPosition(Vector2 mousePos);
};