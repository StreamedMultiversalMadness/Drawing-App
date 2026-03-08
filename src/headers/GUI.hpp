#pragma once
#include <raylib.h>
#include <list>


enum UIElementType
{
    Button,
    Text, 
    ImageType,
};

enum UIElementShape
{
    Square,
    Circle, 
};

struct Scales
{
    Vector2 position;
    Vector2 size;
};

struct Coordinates
{
    Scales unit;
    Scales screen;
};

class UIElement 
{
    

    public:
        static void Loop(Vector2 cursorPos, int screenSizeX, int screenSizeY);// Checks if the cursor is over the region of a ui
    
        
        
        
        
        UIElementType type;
        UIElementShape shape;
        Color baseColor;
        Color color;
        Color focusColor;

        UIElement(UIElementType type, UIElementShape shape, Color color, Vector2 position, Vector2 size);
        // ~UIElement(); // Will be presnent if I implement runtime instantiations

        void Push(); // When you press the ui
        void Focus(); // For when the cursor is on the ui element (The reason for name focus is if you use a gamepad and don't have cursor)
        void UnFocus(); // When the mouse leaves the area of the ui
        void Render(); // Renders the UI (for the frame you call it on)
        void IsFocus(); 
        Coordinates* GetCoordinates();
        Vector2 GetElementScreenSize();
        void UpdateScreenCoordinates(int screenSizeX, int screenSizeY);
        void SetUnitPosition(Vector2 pos);
        void SetUnitSize(Vector2 size);

    private:
        bool isFocus;
        Coordinates coordinates;
};
