#include<GUI.hpp>
#include<iostream>
#include <string>


std::list<UIElement*> elementList; // A list that holds all the elements

void UIElement::SetBackgroundColor(Color bgColor)
{
    this->baseColor = bgColor;
    this->backgroundColor = bgColor; // [BUG?] This might cause some bugs
}

void UIElement::UpdateScreenCoordinates(int screenSizeX, int screenSizeY)
{
    Vector2 unitPos = this->coordinates.unit.position;
    Vector2 unitSize = this->coordinates.unit.size;

    // Vector2 offset = Vector2{screenSizeX / 2, screenSizeY / 2};
    

    Vector2 screenPos = Vector2{unitPos.x * screenSizeX * unitSize.x, unitPos.y * screenSizeY * (1-unitSize.y)}; // [Note to self: Remove the offset]
    Vector2 screenSize = Vector2{unitSize.x * screenSizeX, unitSize.y * screenSizeY};

    this->coordinates.screen.position = screenPos;
    this->coordinates.screen.size = screenSize; 
}

void UIElement::SetUnitPosition(Vector2 pos)
{
    this->coordinates.unit.position = pos;
    this->UpdateScreenCoordinates(GetScreenWidth(), GetScreenHeight());
}

void UIElement::SetUnitSize(Vector2 size)
{
    this->coordinates.unit.size = size;
    this->UpdateScreenCoordinates(GetScreenWidth(), GetScreenHeight());
}

Coordinates* UIElement::GetCoordinates()
{
    return &this->coordinates;
}


UIElement::UIElement(unsigned int properties, Color color, Vector2 position, Vector2 size)
{
    this->properties = properties;
    this->baseColor = color;
    this->backgroundColor = color;
    this->focusColor = Color{color.r, color.g, color.b, 50};
    this->SetUnitPosition(position);
    this->SetUnitSize(size);
    

    elementList.push_back(this);
}



void UIElement::Focus()
{
    this->isFocus = true;
    this->backgroundColor = this->focusColor;
}

void UIElement::UnFocus()
{
    this->isFocus = true;
    this->backgroundColor = this->baseColor;
}

void UIElement::Render()
{
    if(!this->visible)return;
    unsigned int properties = this->properties;
    bool noBackground = properties & UIElementProperties::NoBackground;
    bool text = properties & UIElementProperties::Text;

    Coordinates* coords = this->GetCoordinates();
    Vector2 pos = coords->screen.position;
    Vector2 size = coords->screen.size;
   
    if(!noBackground)
    {
        // Square
        DrawRectangle(pos.x, pos.y, size.x, size.y, this->backgroundColor);
    }
    if(text)
    {
        // Text
        DrawText(this->text.c_str(), pos.x, pos.y, size.y, this->textColor);
    }
        
     
    
}



void Hover(UIElement* e, Vector2 cursorPos)
{
    float cursor_x = cursorPos.x;
    float cursor_y = cursorPos.y;

    Coordinates* coords = e->GetCoordinates();

    Vector2 e_pos = coords->screen.position;
    Vector2 e_size = coords->screen.size;

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
