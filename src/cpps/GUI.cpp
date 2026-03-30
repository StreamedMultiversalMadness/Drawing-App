#include<GUI.hpp>
#include<iostream>
#include <string>
#include<raymath.h>
#include<AnimationSystem.hpp>


std::list<UIElement*> elementList; // A list that holds all the elements

void UIElement::SetBackgroundColor(Color bgColor)
{
    this->baseColor = bgColor;
    this->backgroundColor = bgColor; // [BUG?] This might cause some bugs
}

void UIElement::SetTextColor(Color txtColor)
{
    this->baseTextColor = txtColor;
    this->textColor = txtColor; // [BUG?] This might cause some bugs
}

void UIElement::UpdateScreenPos(float screenSizeX, float screenSizeY)
{
    Vector2 unitPos = coordinates.unitPos;
    Vector2 unitSize = coordinates.unitSize;
    
    Vector2 offset = {pivot.x * screenSizeX, pivot.y * screenSizeY};
    Vector2 newUnitPos = {unitPos.x - pivot.x * unitSize.x, unitPos.y - pivot.y * unitSize.y};
    Vector2 screenPos = Vector2{newUnitPos.x * screenSizeX, newUnitPos.y * screenSizeY}; // [EDIT HERE ]

    std::cout << " Screen position : (" << coordinates.screenPos.Get().x << ", " << coordinates.screenPos.Get().y << ")" << std::endl;
    coordinates.screenPos.Set(screenPos);
}

void UIElement::UpdateScreenSize(float screenSizeX, float screenSizeY)
{
    Vector2 unitSize = coordinates.unitSize;
    Vector2 screenSize = Vector2{unitSize.x * screenSizeX, unitSize.y * screenSizeY}; 

    coordinates.screenSize = screenSize; 
}

void UIElement::UpdateScreenCoordinates(float screenSizeX, float screenSizeY)
{
    UpdateScreenPos(screenSizeX, screenSizeY);
    UpdateScreenSize(screenSizeX, screenSizeY);
}

void UIElement::SetUnitPosition(Vector2 pos)
{
    coordinates.unitPos = pos;
    UpdateScreenPos(GetScreenWidth(), GetScreenHeight());
}

void UIElement::SetUnitSize(Vector2 size)
{
    coordinates.unitSize = size;
    UpdateScreenSize(GetScreenWidth(), GetScreenHeight());
}

Coordinates UIElement::GetCoordinates()
{
    return coordinates;
}

void UIElement::SetPivot(Vector2 newPivot)
{
    pivot = newPivot;
}


UIElement::UIElement(unsigned int properties, Color color, Vector2 position, Vector2 size)
{
    this->properties = properties;
    this->baseColor = color;
    this->backgroundColor = color;
    this->focusColor = Color{color.r, color.g, color.b, 50};
    this->baseTextColor = WHITE;
    this->pivot = Vector2Zero();
    
    this->SetUnitPosition(position);
    this->SetUnitSize(size);
    

    elementList.push_back(this);
}



void UIElement::Focus()
{
    this->isFocus = true;
    this->backgroundColor = this->focusColor;
    this->textColor = this->focusColor;
}

void UIElement::UnFocus()
{
    this->isFocus = true;
    this->backgroundColor = this->baseColor;
    this->textColor = this->baseTextColor;
}

void UIElement::Render()
{
    if(!this->visible)return; // Will not render if visible is false


    unsigned int props = this->properties;
    bool square = props & UIElementProperties::Square;
    bool text = props & UIElementProperties::Text;

    Vector2 pos = coordinates.screenPos.Get();
    Vector2 size = coordinates.screenSize;
    
    if(square)
    {
       
        DrawRectangle(pos.x, pos.y, size.x, size.y, this->backgroundColor);
    }
    if(text)
    {
        // Text
        int fontSize = size.y;
        const char* text = this->text.c_str();
        int textWidth = MeasureText(text, fontSize);
        DrawText(text, pos.x, pos.y, fontSize, this->textColor);
    }
        
     
    
}



void Hover(UIElement* e, Vector2 cursorPos)
{
    float cursor_x = cursorPos.x;
    float cursor_y = cursorPos.y;

    Coordinates coords = e->GetCoordinates();

    Vector2 e_pos = coords.screenPos.Get();
    Vector2 e_size = coords.screenSize;

    float posX = e_pos.x;
    float posY = e_pos.y;

    float endX = posX + e_size.x;
    float endY = posY + e_size.y;


    bool hover = false;

    
    // Square
    bool xVacinity = cursor_x < endX && cursor_x > posX;
    bool yVacinity = cursor_y < endY && cursor_y > posY;
    if(xVacinity && yVacinity) hover = true; // Cursor is inside
      

    
    if(hover)
    {
        e->Focus();
    }
    else
    {
        e->UnFocus();
    }
}

int lastX = 0; // Temp
int lastY = 0; // Temp

void UIElement::Loop(Vector2 cursorPos, int screenSizeX, int screenSizeY) // Checks if the cursor is over the region of a ui
{
    // Here all should be rendered

    if (screenSizeX != lastX || screenSizeY != lastY) // For when the screen size changes
    {
        lastX = screenSizeX;
        lastY = screenSizeY;

        for (UIElement* e : elementList)
        {
            e->UpdateScreenCoordinates(lastX, lastY);
        }
    }


    for (UIElement* e : elementList)
    {
        e->Render();
        Hover(e, cursorPos);
    }
    
}
