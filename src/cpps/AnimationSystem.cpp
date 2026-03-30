#include<AnimationSystem.hpp>
#include<list>
#include<iostream>
#include<EventSystem.h>
#include<raylib.h>
#include"raymath.h"

using namespace AnimationSystem;
using EventSystem::Event;


std::list<AnimatableValue*> list_AnimatableValue;  // A list that holds all the interpolation objects

// Interpolation::Interpolation(Event changeEvent, void(*setter)(float val))
// {
//     interpList.push_front(this); // Adds an element to end of list defined in AnimationManager.h
// }

// void Interpolation::Interpolate()
// {
//     

// }


AnimatableValue::AnimatableValue()
{
    goal = 1;
    currentValue = 1;
    list_AnimatableValue.push_front(this);
}

AnimatableValue::~AnimatableValue() // Not sure if this part is necessary
{
    list_AnimatableValue.remove(this);
}

void AnimatableValue::Set(float newGoal)
{
    this->goal = newGoal;
}

float AnimatableValue::Get()
{
    return this->currentValue;
}

void AnimatableValue::Interpolate()
{
    if(currentValue == goal) return; // Animation is finished
    float time = 0.1f; // [W.I.P]
    
    // (*value) = startValue * (1.0f - time) + endValue * time; // Sets the value of the thing that should animate
    currentValue = currentValue * (1.0f - time) + goal * time;
}


////////////////////////////// ANIMATABLE VECTOR2 START HERE ///////////////////////////////


Vector2 AnimatableVector2::Get()
{
    return Vector2{x.Get(), y.Get()};
}

void AnimatableVector2::Set(float x, float y)
{
    this->x.Set(x);
    this->y.Set(y);
}

void AnimatableVector2::Set(Vector2 pos)
{
    this->x.Set(pos.x);
    this->y.Set(pos.y);
}



void AnimationSystem::Process()
{
    for (AnimatableValue* v : list_AnimatableValue) // Runs through every interpolation in interpList
    {
        v->Interpolate();
    }
     
}