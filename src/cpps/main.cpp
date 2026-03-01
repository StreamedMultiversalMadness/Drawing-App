#include "raylib.h"
#include"iostream"
#include"math.h"
#include"raymath.h"

struct MouseOffset
{
    Vector2 referencePoint;
    Vector2 offset;
};

struct ScreenInfo
{
    Vector2 size;
    Vector2 center;
    Vector2 centeroffset;
    float ratio;
    Color color;
    MouseOffset* mouseOffset;
};

class Pensel
{
    public:
        int arraySize = 8000;
        Vector2 points[8000];
        int lastAddedIndex = 0;
        ScreenInfo* screenInfo;

        void AddPoint(Vector2 point)
        {
            Vector2 offset = screenInfo->mouseOffset->offset;

            if(lastAddedIndex == arraySize-1)
            {
                lastAddedIndex = 0;
            }

            points[lastAddedIndex] = point + offset;;

            lastAddedIndex++;
        }



        void DrawPoints()
        {
            Vector2 offset = screenInfo->mouseOffset->offset;

            for(int i = 1; i < arraySize; i++)
            {
                Vector2 from = points[i-1] - offset;
                Vector2 to = points[i] - offset;
                // if(from == Vector2Zero() || to == Vector2Zero())continue;
                DrawLineEx(from, to, 2.0f, WHITE);
            }
        
        }

        void StopDrawing()
        {
            Vector2 offset = screenInfo->mouseOffset->offset;

            if(lastAddedIndex == arraySize-1)
            {
                lastAddedIndex = 0;
            }

            points[lastAddedIndex] = Vector2Zero();

            lastAddedIndex++;
        }
};

struct AppContext
{
    Pensel* pensel;
    ScreenInfo* screenInfo;
};

ScreenInfo GetScreenInfo(Color screenColor)
{
    float x = GetScreenWidth();
    float y = GetScreenHeight();

    ScreenInfo info;
    info.size = Vector2{x, y};
    info.center = Vector2{x/2, y/2};
    info.ratio = x / y;
    info.color = screenColor;

    return info;
}

float GetHypotenuse(float x, float y)
{
    return sqrt(x*x + y*y);
}

void ButtonPressedEvents(AppContext* context) // When a button is pressed these function inside only fires once
{
    if(IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))
    {
        context->screenInfo->mouseOffset->referencePoint = GetMousePosition() + context->screenInfo->mouseOffset->offset;
    }
}

void ButtonHoldEvents(AppContext* context) // These functions inside fire as long as you hold down a button
{
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        context->pensel->AddPoint(GetMousePosition());
    }
    else if(IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
    {
        context->screenInfo->mouseOffset->offset = context->screenInfo->mouseOffset->referencePoint - GetMousePosition() ; // Relative to the point you recently pressed middle mouse
    }
}

void ButtonReleaseEvents(AppContext* context)
{
    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        // context->pensel->StopDrawing();
    }    
}


void HandleInput(AppContext* context)
{
    ButtonPressedEvents(context);
    ButtonHoldEvents(context);
    ButtonReleaseEvents(context);
}


int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "Rita");
    
    
    Color screenColor = Color{18, 18, 18, 255};

    
    ScreenInfo screenInfo = GetScreenInfo(screenColor);
    MouseOffset mouseOffest = {};
    screenInfo.mouseOffset = &mouseOffest;
    // scrre->offset = Vector2Zero();
    // screenInfo.mouseOffset->referencePoint = Vector2{0,0};

    Pensel pensel;
    pensel.screenInfo = &screenInfo;
    AppContext context = {0};
    context.pensel = &pensel;
    context.screenInfo = &screenInfo;

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();

            HandleInput(&context);

            ClearBackground(screenInfo.color);

            pensel.DrawPoints();
            
        EndDrawing();
    }



    CloseWindow();

    return 0;
}