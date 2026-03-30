#pragma once
#include<raylib.h>

namespace AnimationSystem // Namespace
{
    // enum InterpolationType // [W.I.P]
    // {
    //     Linear,
    // };

    // class Interpolation
    // {
    //     private:
            
    //     public:
    //         float startValue; // The starting value, where the animation starts from
    //         float endValue; // The final value, when the animation is done

    //         // InterpolationType type; // [W.I.P]

    //         void Interpolate(); // Runs the animation one step (Should be called once every frame)
            
        
    //         Interpolation(Event changeEvent, void(*setter)(float val)); // Constructor
    //     // ~Interpolation(); // Destructor
    // };

    class AnimatableValue
    {
        private:
            float currentValue;
            float goal;
            void Interpolate();

        public:
            void Set(float goal); // Sets the goal for which to interpolate towards
            float Get(); // Returns the current value 

        AnimatableValue(); // Constructor
        ~AnimatableValue(); // Destructor

        friend void Process(); // Friend function
    };

    class AnimatableVector2
    {
        private:
            AnimatableValue x;
            AnimatableValue y;
        public:
            void Set(float x, float y);
            void Set(Vector2 pos);
            Vector2 Get();

        friend void Process(); // Friend function
    };


    
    void Process(); // Run all the current animations in interpList (Should be called once every frame)

}
