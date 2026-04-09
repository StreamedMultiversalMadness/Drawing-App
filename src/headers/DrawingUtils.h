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
        ScreenInfo* screenInfo;

        void Erase(Pensel* p);
        void Render();
        void SetPosition(Vector2 mousePos);
};

class Lasso
{
    private:
        int length;
        Vector2 pointArray[1000];
    public:
        bool enabled = false;
        Vector2 GetPoint(int index);
        int GetMaxArraySize();
        void AddPoint(Vector2 point); // Adds a point at a certain index
        int GetLength(); // Returns the value of length (how many points that are placed)
        void LetGo();

        void Render();
};