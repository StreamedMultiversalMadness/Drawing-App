#pragma once
#include <raylib.h>
#include <list>
#include<vector>
#include <string>
#include<AnimationSystem.hpp>

using namespace AnimationSystem;

enum UIElementProperties // These are used as Flags
{
    Square = 1,
    Text = 2, 
    Circle = 4,
    Picture = 8,
};

// struct AnimatableScales
// {
//     AnimatableVector2 position;
//     Vector2 size;
// };

// struct Scales
// {
//     Vector2 position;
//     Vector2 size;
// };

struct Coordinates
{
    Vector2 unitPos;
    Vector2 unitSize;
    
    AnimatableVector2 screenPos;
    Vector2 screenSize;
};

class UIElement 
{
    

    public:
        static std::list<UIElement*> elementList; // A list that holds all the elements
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

        Coordinates GetCoordinates();
        Vector2 GetElementScreenSize();
        void SetPivot(Vector2 newPivot);
        void SetBackgroundColor(Color bgColor);
        void SetTextColor(Color txtColor);
        void UpdateScreenCoordinates(float screenSizeX, float screenSizeY);
        void SetUnitPosition(Vector2 pos);
        void SetUnitSize(Vector2 size);

    private:
        bool isFocus;
        Color backgroundColor;
        Color baseColor;
        Color focusColor;
        Color baseTextColor;
        Vector2 pivot;

        void UpdateScreenPos(float screenSizeX, float screenSizeY);
        void UpdateScreenSize(float screenSizeX, float screenSizeY);
        
        Coordinates coordinates;
};

class SelectionWheel // Will have UIElements in it
{
    private:
        float unitRadius; // Percentage of which the radius occupies the screen.
        float elementScaling; // Percentage of scaling for elements if for example the UIElements all need to be scaled down.
        std::vector<UIElement*> elements; 
        static void ShowElement(UIElement* e, bool show);
        void SetElementPositionByOrder(UIElement* e, Vector2 unitPos, int order);

    public:
        void OpenAt(Vector2 pos); // Sets the position of each UIElement and makes them visible.
        void Close(); // The reverse process of Open bascially.
        
        void Add(UIElement* e); // Adds a UI elemnt to the elements list.
        void Remove(UIElement& e); // Removes a UI element from the elements list.

        SelectionWheel(float unitRadius); // Constructor

};
