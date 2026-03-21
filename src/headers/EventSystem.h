#pragma once
// #include<list>

namespace EventSystem
{
    

    class Event
    {
        private:
            // std::list<void> connections; // Holds all the connections  [CRASH WARNING]
        public: 
            void Connect(void (*function)()); // Connecting a function to the event so that the function is called when the event is triggered
            void Disconnect(void (*function)()); // Disconnects the specified function from being called when event is triggered

        friend void TriggerEvent(Event &event);
    };

    void TriggerEvent(Event &event); // For triggering an event
}
