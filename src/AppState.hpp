#pragma once

/*
    This class will represent different states in the application.
    This allows for a centralized way for different parts of the application
    to communicate with each other while also being in different translation units
*/

enum class OpState { Idle, ShowPath, Edit, Simulate, INIT }; // Operation State

class AppState
{
    public:
        AppState(OpState p_initial_state);
        OpState current_state;
        OpState default_state;
};

AppState::AppState(OpState p_initial_state)
{
    current_state = std::move(p_initial_state);
    default_state = p_initial_state;
}
