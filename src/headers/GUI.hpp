#pragma once
#include <raylib.h>
#include <list>
#include <string>


enum UIElementProperties // These are used as Flags
{
    NoBackground = 1,
    Text = 2, 
    Picture = 4,
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
    
        
        
        
        
        unsigned int properties; // Uses UIElementProperties
        

        std::string text;
        Color textColor;

        bool visible = true;

        UIElement(unsigned int properties, Color color, Vector2 position, Vector2 size);
        // ~UIElement(); // Will be presnent if I implement runtime instantiations

        void Push(); // When you press the ui
        void Focus(); // For when the cursor is on the ui element (The reason for name focus is if you use a gamepad and don't have cursor)
        void UnFocus(); // When the mouse leaves the area of the ui
        void Render(); // Renders the UI (for the frame you call it on)
        void IsFocus(); 

        Coordinates* GetCoordinates();
        Vector2 GetElementScreenSize();
        void SetBackgroundColor(Color bgColor);
        void SetTextColor(Color txtColor);
        void UpdateScreenCoordinates(int screenSizeX, int screenSizeY);
        void SetUnitPosition(Vector2 pos);
        void SetUnitSize(Vector2 size);

    private:
        bool isFocus;
        Color backgroundColor;
        Color baseColor;
        Color focusColor;
        Color baseTextColor;
        
        Coordinates coordinates;
};
