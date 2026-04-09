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
    Vector2 pos = this->screenInfo->GetDrawSpacePosition(this->position);
    float scaledRadius = this->radius;
    for(int i = 0; i < p->arraySize; i++)
    {
        Vector2 point = p->points[i];
        float distance = Vector2Distance(pos, point);
        if(distance < scaledRadius)
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
    DrawCircleLinesV(this->position, this->radius * this->screenInfo->zoom.value.Get(), WHITE);
}

int Lasso::GetLength()
{
    return this->length;
}

int Lasso::GetMaxArraySize()
{
    return sizeof(this->pointArray) / sizeof(Vector2);
}

void Lasso::AddPoint(Vector2 point)
{
    if(!this->enabled)return;
    // [TO DO]If length from the point before to the current point is above some threshold then we draw instead ()

    

    int maxSize = this->GetMaxArraySize();
    int len = this->length;
    if(len == maxSize)return; // Too big

    if(len == 0)
    {
        pointArray[len] = point;    
    }

    

    
    length++;
}

void Lasso::LetGo()
{
    // Let go
}

Vector2 Lasso::GetPoint(int index)
{
    int maxSize = this->GetMaxArraySize();
    if(index >= maxSize)return Vector2Zero(); // Index out of bounds

    return this->pointArray[index];
}

void DrawLineDotted(Vector2 from, Vector2 to, Color color) // Not Wokring
{
    Vector2 dir = to-from;
    Vector2 nDir = Vector2Normalize(dir);
    float vLen = Vector2Length(dir);
    
    // First line segement
    Vector2 f = from;
    Vector2 t = from + nDir * vLen/8;
    DrawLineV(f, t, color);

    // Second line segment
    f = from + nDir * vLen/4;
    t = from + nDir * vLen/2;
    DrawLineV(f, t, color);

    // Third line segment
    f = to - nDir * vLen/8;
    t = to;
    DrawLineV(f, t, color);
}

float prevT = 0;
void Lasso::Render()
{
    if(!this->enabled)return;

    // int maxSize = this->GetMaxArraySize();
    int len = this->length;

    float travelDistance = 0.0f;
    bool theSwitch = false;

    for(int i = 0; i < len; i++)
    {
        int nextIndex = (i+1)%len; // Wraps around if len == maxSize

        Vector2 from = this->pointArray[i];
        Vector2 to = this->pointArray[nextIndex];
        float dis = Vector2Length(to - from);

        
        travelDistance += dis;
        if(travelDistance > 5.0f)
        {
            travelDistance = 0;
            theSwitch = !theSwitch;
        }

        if(theSwitch)continue;
        DrawLineV(from, to, WHITE);
    }
}