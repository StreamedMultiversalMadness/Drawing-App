#include "raylib.h"
#include"iostream"
#include"math.h"
#include"raymath.h"
#include"GUI.hpp"
#include <string>

struct MouseOffset
{
    Vector2 referencePoint;
    Vector2 value;
};

struct Zoom
{
    float min = 1.0f;
    float max = 2.0f;
    float value = 1.0f;
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

        ScreenInfo()
        {
            float x = GetScreenWidth();
            float y = GetScreenHeight();
            MouseOffset mOffset;
            Zoom zm;

            size = Vector2{x, y};
            center = Vector2{x/2, y/2};
            ratio = x / y;

            zoom = zm;
            mouseOffset = mOffset;
        }

        void SetBackgroundColor(Color c)
        {
            color = c;
        }
};

class Pensel
{
    public:
        int arraySize = 8000;
        Vector2 points[8000];
        int lastAddedIndex = 0;
        ScreenInfo* screenInfo;
        Color color;

        Pensel()
        {
            color = WHITE;
        }

        void AddPoint(Vector2 point)
        {
            // Point is usually Mouse Position
            if(point == Vector2Zero())return;
            
            Vector2 offset = screenInfo->mouseOffset.value;

            if(lastAddedIndex == arraySize-1)
            {
                lastAddedIndex = 0;
            }

            point = point / screenInfo->zoom.value; // Zoom scaling
            points[lastAddedIndex] = (point + offset);

            lastAddedIndex++;
        }



        void DrawPoints()
        {
            Vector2 offset = screenInfo->mouseOffset.value;

            for(int i = 1; i < arraySize; i++)
            {

                Vector2 from = points[i-1];
                Vector2 to = points[i];
                if(from == Vector2Zero() || to == Vector2Zero())continue;
                from = (from - offset) * screenInfo->zoom.value;
                to = (to - offset) * screenInfo->zoom.value;
                DrawLineEx(from, to, 2.0f, color);
            }
        
        }

        void CreateBreak()
        {
            if(lastAddedIndex == arraySize-1)
            {
                lastAddedIndex = 0; // Reset of you've drawn too much, This will change to add more slots instead
            }

            points[lastAddedIndex] = Vector2Zero();

            lastAddedIndex++;
        }
};

struct AppContext
{
    Pensel* pensel;
    ScreenInfo* screenInfo;
    bool paused = false;
    bool quit = false;
};

float GetHypotenuse(float x, float y)
{
    return sqrt(x*x + y*y);
}

void ButtonPressedEvents(AppContext* context) // When a button is pressed these function inside only fires once
{
    if(IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))
    {
        
        MouseOffset mOffset = context->screenInfo->mouseOffset;
        context->screenInfo->mouseOffset.referencePoint = GetMousePosition() + mOffset.value; // This is the problem, I don't know what's wrong
    }
    if(GetMouseWheelMove() > 0.0f)
    {
        Zoom zoom = context->screenInfo->zoom;
        context->screenInfo->zoom.value += 0.2f;
        context->screenInfo->zoom.value = Clamp(context->screenInfo->zoom.value, context->screenInfo->zoom.min, context->screenInfo->zoom.max);
    }
    else if (GetMouseWheelMove() < 0.0f)
    {
        Zoom zoom = context->screenInfo->zoom;
        context->screenInfo->zoom.value -= 0.2f;
        context->screenInfo->zoom.value = Clamp(context->screenInfo->zoom.value, context->screenInfo->zoom.min, context->screenInfo->zoom.max);
    }
}

void ButtonHoldEvents(AppContext* context) // These functions inside fire as long as you hold down a button
{
    if(context->paused)return; // [CHANGE] Change this here maybe

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        context->pensel->AddPoint(GetMousePosition());
    }
    else if(IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
    {
        context->screenInfo->mouseOffset.value = context->screenInfo->mouseOffset.referencePoint - GetMousePosition() ; // Relative to the point you recently pressed middle mouse
    }
}

void ButtonReleaseEvents(AppContext* context)
{
    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        context->pensel->CreateBreak();
    }    
}


void HandleInput(AppContext* context)
{
    ButtonPressedEvents(context);
    ButtonHoldEvents(context);
    ButtonReleaseEvents(context);
}

void PauseAction()
{
    
}


int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "Rita");
    SetExitKey(0); // Otherwise Esc will be the default

    
    Color screenColor = Color{18, 18, 18, 255};

    
    ScreenInfo screenInfo;
    screenInfo.zoom.max = 4.0f;

    // scrre->offset = Vector2Zero();
    // screenInfo->mouseOffset.referencePoint = Vector2{0,0};

    UIElement resume_Button = {UIElementProperties::Text, BLACK, Vector2{0.5, 0.5}, Vector2{0.5, 0.1}};
    resume_Button.textColor = WHITE;
    resume_Button.text = "Resume";
    resume_Button.visible = false;

    UIElement quit_Button = {UIElementProperties::Text, BLACK, Vector2{0.5, 0.65}, Vector2{0.5, 0.1}};
    quit_Button.textColor = WHITE;
    quit_Button.text = "Quit";
    quit_Button.visible = false;

    Pensel pensel;
    pensel.screenInfo = &screenInfo;

    AppContext context = {0};
    context.pensel = &pensel;
    context.screenInfo = &screenInfo;

    SetTargetFPS(60);
    while (!WindowShouldClose() && !context.quit)
    {
        BeginDrawing();

            HandleInput(&context);
            if(IsKeyPressed(KEY_ESCAPE))
            {
                context.paused = !context.paused;
                resume_Button.visible = context.paused;
                quit_Button.visible = context.paused; 
            }

            
            
            
            ClearBackground(screenColor);
            
            pensel.DrawPoints();
            UIElement::Loop(GetMousePosition(), GetScreenWidth(), GetScreenHeight());
            
        EndDrawing();
    }



    CloseWindow();

    return 0;
}