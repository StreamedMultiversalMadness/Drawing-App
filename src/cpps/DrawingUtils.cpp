#include"DrawingUtils.h"
#include<raylib.h>
#include<raymath.h>

Pensel::Pensel()
{
    color = WHITE;
}

void Pensel::CreateBreak()
{
    if(lastAddedIndex == arraySize-1)
    {
        lastAddedIndex = 0; // Reset of you've drawn too much, This will change to add more slots instead
    }

    points[lastAddedIndex] = Vector2Zero();

    lastAddedIndex++;
}

void Pensel::DrawPoints()
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

void Pensel::AddPoint(Vector2 point)
{
    if(!this->enabled)return;
    if(point == Vector2Zero())return;
    

    if(lastAddedIndex == arraySize-1)
    {
        lastAddedIndex = 0;
    }

 
    points[lastAddedIndex] = this->screenInfo->GetDrawSpacePosition(point);
  

    lastAddedIndex++;
}


void Eraser::Erase(Pensel* p)
{
    if(!this->enabled)return;
    Vector2 pos = p->screenInfo->GetDrawSpacePosition(this->position);
    for(int i = 0; i < p->arraySize; i++)
    {
        Vector2 point = p->points[i];
        float distance = Vector2Distance(pos, point);
        if(distance < this->radius)
        {
            // Erase
            p->points[i] = Vector2Zero();
        }
    }
}

void Eraser::SetPosition(Vector2 mousePos)
{
    this->position = mousePos;
}

void Eraser::Render()
{
    if(!this->enabled)return;
    DrawCircleLinesV(this->position, this->radius, WHITE);
}