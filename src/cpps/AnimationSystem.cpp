#include<AnimationSystem.hpp>
#include<list>
#include<iostream>
#include<EventSystem.h>
#include<raylib.h>
#include"raymath.h"

using namespace AnimationSystem;
using EventSystem::Event;

std::list<AnimatableValue*> list_AnimatableValue;  // A list that holds all the interpolation objects
std::list<AnimatableVector2*> list_AnimatableVector2;

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


AnimatableVector2::AnimatableVector2()
{
    list_AnimatableVector2.push_front(this);
}

AnimatableVector2::~AnimatableVector2() // Not sure if this part is necessary
{
    list_AnimatableVector2.remove(this);
}

Vector2 AnimatableVector2::Get()
{
    return this->current;
}

void AnimatableVector2::Set(Vector2 goal)
{
    this->goal = goal;
}

void AnimatableVector2::Interpolate()
{
    if(Vector2Equals(current, goal)) return; // Animation is finished
    float time = 0.1f; // [W.I.P]
    
    // (*value) = startValue * (1.0f - time) + endValue * time; // Sets the value of the thing that should animate
    current = current * (1.0f - time) + goal * time;
}




void AnimationSystem::Process()
{
    for (AnimatableValue* v : list_AnimatableValue) // Runs through every interpolation in interpList
    {
        v->Interpolate();
    }

    for (AnimatableVector2* v2 : list_AnimatableVector2) // Runs through every interpolation in interpList
    {
        v2->Interpolate();
    }
     
}