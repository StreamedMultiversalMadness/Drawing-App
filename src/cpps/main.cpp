#include "raylib.h"
#include"iostream"
#include"math.h"
#include"raymath.h"
#include"GUI.hpp"
#include <string>
#include"AnimationSystem.hpp"
#include"Screen.h"
#include"DrawingUtils.h"

using namespace AnimationSystem;
using namespace Screen;





struct AppContext
{
    Pensel* pensel;
    Eraser* eraser;
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
        context->eraser->Erase(context->pensel);
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
    drawButton.imageScale = 0.2f;
    drawButton.SetUnitSize(0.02f);

    UIElement drawButton2 = {restUnitPos_quit, UIElementProperties::Circle | UIElementProperties::ImageLabel};
    drawButton2.SetImageTexture("../images/selectionwheel/lasso.png");
    drawButton2.SetButtonColor(buttonColor);
    drawButton2.imageScale = 0.2f;
    drawButton2.SetUnitSize(0.02f);
 

    UIElement drawButton3 = {restUnitPos_quit, UIElementProperties::Circle | UIElementProperties::ImageLabel};
    drawButton3.SetImageTexture("../images/selectionwheel/pen.png");
    drawButton3.SetButtonColor(buttonColor);
    drawButton3.imageScale = 0.2f;
    drawButton3.SetUnitSize(0.02f);
 
    SelectionWheel wheel = {0.4f};
    wheel.Add(&drawButton);
    wheel.Add(&drawButton2);
    wheel.Add(&drawButton3);


    Pensel pensel;
    pensel.screenInfo = &screenInfo;
    

    Eraser eraser; 
    eraser.screenInfo = &screenInfo;

    

    AppContext context = {0};
    context.pensel = &pensel;
    context.eraser = &eraser;
    context.screenInfo = &screenInfo;

    SetTargetFPS(60);
    while (!WindowShouldClose() || context.quit)
    {
        BeginDrawing();

            Vector2 mousePos = GetMousePosition();

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

            if(IsKeyPressed(KEY_SPACE))
            {
                wheel.OpenAt(ScreenToUnitCords(mousePos));
                pensel.enabled = !pensel.enabled;
                eraser.enabled = !eraser.enabled;  
            }
            else if(IsKeyReleased(KEY_SPACE))
            {
                wheel.Close();
            }

            eraser.SetPosition(mousePos);
            eraser.Render();
            
            
            ClearBackground(screenColor);
            
            pensel.DrawPoints();
            UIElement::Loop(GetMousePosition(), GetScreenWidth(), GetScreenHeight());
            AnimationSystem::Process();
            
        EndDrawing();
    }



    CloseWindow();

    return 0;
}