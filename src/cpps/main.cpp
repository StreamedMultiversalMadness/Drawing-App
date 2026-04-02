#include "raylib.h"
#include"iostream"
#include"math.h"
#include"raymath.h"
#include"GUI.hpp"
#include <string>
#include<AnimationSystem.hpp>

using namespace AnimationSystem;

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

        ScreenInfo()
        {
            float x = GetScreenWidth();
            float y = GetScreenHeight();

            size = Vector2{x, y};
            center = Vector2{x/2, y/2};
            ratio = x / y;
        }

        void SetBackgroundColor(Color c)
        {
            color = c;
        }
};

class Pensel
{
    public:
        int arraySize = 80000;
        Vector2 points[80000];
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
            
            Vector2 offset = screenInfo->mouseOffset.value.Get();

            if(lastAddedIndex == arraySize-1)
            {
                lastAddedIndex = 0;
            }

            point = point / screenInfo->zoom.value.Get(); // Zoom scaling
            points[lastAddedIndex] = (point + offset);

            lastAddedIndex++;
        }



        void DrawPoints()
        {
            Vector2 offset = screenInfo->mouseOffset.value.Get();

            for(int i = 1; i < arraySize; i++)
            {

                Vector2 from = points[i-1];
                Vector2 to = points[i];
                if(from == Vector2Zero() || to == Vector2Zero())continue;
                from = (from - offset) * screenInfo->zoom.value.Get();
                to = (to - offset) * screenInfo->zoom.value.Get();
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

float lerp(float start, float end, float time)
{
    return start * (1- time) + end * time;
}

Vector2 lerp(Vector2 start, Vector2 end, float time)
{
    return start * (1 - time) + end * time;
}

void ButtonPressedEvents(AppContext* context) // When a button is pressed these function inside only fires once
{
    if(IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))
    {
        
        MouseOffset mOffset = context->screenInfo->mouseOffset;
        context->screenInfo->mouseOffset.referencePoint = GetMousePosition() + mOffset.value.Get(); // This is the problem, I don't know what's wrong
    }
    if(GetMouseWheelMove() > 0.0f || GetMouseWheelMove() < 0.0f) // Mouse Scroll
    {
        
        float zoomAdd = 0.2f;

        if(GetMouseWheelMove() < 0.0f) // Scroll out (Zoom out)
        {
            zoomAdd = -0.2f;
        }

        Zoom zoom = context->screenInfo->zoom;
        float newVal = Clamp(context->screenInfo->zoom.value.Get() + zoomAdd, context->screenInfo->zoom.min, context->screenInfo->zoom.max);

        context->screenInfo->zoom.value.Set(newVal);
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
        MouseOffset* mouseOffset = &context->screenInfo->mouseOffset;
        mouseOffset->value.Set(mouseOffset->referencePoint - GetMousePosition()); // Relative to the point you recently pressed middle mouse
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

    Color buttonColor = Color{50, 50, 50, 255};

    Vector2 restUnitPos_resume = Vector2{0.5, 0.4};
    Vector2 restUnitPos_quit= Vector2{0.5, 0.7};

    UIElement resume_Button = {restUnitPos_resume, UIElementProperties::Text | UIElementProperties::Square};
    resume_Button.SetUnitSize(Vector2{0.3, 0.1});
    resume_Button.SetButtonColor(buttonColor);
    resume_Button.SetPivot(Vector2{0.5, 0.5});
    resume_Button.textColor = WHITE;
    resume_Button.text = "Resume";
    resume_Button.visible = false;

    UIElement quit_Button = {restUnitPos_quit, UIElementProperties::Text | UIElementProperties::Square,};
    quit_Button.SetUnitSize(Vector2{0.3, 0.1});
    quit_Button.SetButtonColor(buttonColor);
    quit_Button.SetPivot(Vector2{0.5, 0.5});
    quit_Button.textColor = WHITE;
    quit_Button.text = "Quit";
    quit_Button.visible = false;

    UIElement drawButton = {restUnitPos_quit, UIElementProperties::Circle | UIElementProperties::ImageLabel};
    drawButton.SetImageTexture("../images/selectionwheel/erase.png");
    drawButton.SetButtonColor(buttonColor);
    drawButton.SetUnitSize(0.02f);

    UIElement drawButton2 = {restUnitPos_quit, UIElementProperties::Circle | UIElementProperties::ImageLabel};
    drawButton2.SetImageTexture("../images/selectionwheel/lasso.png");
    drawButton2.SetButtonColor(buttonColor);
    drawButton2.SetUnitSize(0.02f);
 

    UIElement drawButton3 = {restUnitPos_quit, UIElementProperties::Circle | UIElementProperties::ImageLabel};
    drawButton3.SetImageTexture("../images/selectionwheel/pen.png");
    drawButton3.SetButtonColor(buttonColor);
    drawButton3.SetUnitSize(0.02f);
 
    SelectionWheel wheel = {0.4f};
    wheel.Add(&drawButton);
    wheel.Add(&drawButton2);
    wheel.Add(&drawButton3);

    

    Pensel pensel;
    pensel.screenInfo = &screenInfo;

    AppContext context = {0};
    context.pensel = &pensel;
    context.screenInfo = &screenInfo;

    SetTargetFPS(60);
    while (!WindowShouldClose() || context.quit)
    {
        BeginDrawing();

            HandleInput(&context);
            if(IsKeyPressed(KEY_ESCAPE)) // Being executed once per press
            {
                context.paused = !context.paused;
                resume_Button.visible = context.paused;
                quit_Button.visible = context.paused; 
                if(context.paused)
                {
                    resume_Button.SetUnitPosition(Vector2{0.5f, 0.5f});
                    quit_Button.SetUnitPosition(Vector2{0.5f, 0.65f});
                }else
                {
                    resume_Button.SetUnitPosition(restUnitPos_resume);
                    quit_Button.SetUnitPosition(restUnitPos_quit);
                }
            }

            if(IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))
            {
                wheel.OpenAt(ScreenToUnitCords(GetMousePosition()));
            }
            else if(IsMouseButtonReleased(MOUSE_BUTTON_MIDDLE))
            {
                wheel.Close();
            }

            
            
            
            ClearBackground(screenColor);
            
            pensel.DrawPoints();
            UIElement::Loop(GetMousePosition(), GetScreenWidth(), GetScreenHeight());
            AnimationSystem::Process();
            
        EndDrawing();
    }



    CloseWindow();

    return 0;
}