#pragma once
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include "Grid.hpp"
#include "GUI.hpp"
#include "AppState.hpp"

class Model {
    public:
        Model(Grid& g, sf::RenderWindow& w, GUI& gui_param, AppState* p_app_state);
        void update(sf::RenderWindow& window, int mouseX, int mouseY);
        void draw();
        void handleLeftClick(int mouseX, int mouseY);
        void handleLeftReleased();
        void handleRightClick(int x, int y);
        void handleKeyPress(const sf::Event& event);

     
	    std::vector<std::string> states;
        
        sf::Color white = sf::Color::White;
        sf::Color blue = sf::Color::Blue;
        sf::Color red = sf::Color::Red;
        sf::Color green = sf::Color::Green;
        sf::Color yellow = sf::Color::Yellow;

        sf::Time delta_time;

        std::string current_state = "";

        bool leftClickDown = false;
        bool rightClickDown = false;
        bool printGrid = true;
        bool debug = false;

        //display_state refers to the current mode of the playback for the algorithms on 
        //the grid. The current states supported are:
        //  0 -- Default state, move the spawn and goal nodes/create walls on the grid
        //  1 -- choose a start/end node then playback the algorithm in real-time, started
        //       by some start button
        
        int display_state = 1;
        int current_clicked_button;

        GUI& gui;
        Grid& grid;
        sf::RenderWindow& window;
        AppState* app_state;
        OpState default_state = OpState::ShowPath;
};

Model::Model(Grid& g, sf::RenderWindow& w, GUI& gui_param, AppState* p_app_state) : grid(g), window(w), gui(gui_param), app_state(p_app_state)
{
    current_clicked_button = 0;
}


void Model::update(sf::RenderWindow &window, int mouseX, int mouseY){
    if(leftClickDown)
    {
        handleLeftClick(mouseX, mouseY);
    }
    grid.update(); 
    //std::cout << (1/app_state.delta_time) << "Frames per second\n";
    for(auto& button: gui.buttons){
        button.update(mouseX, mouseY);
    }
}

void Model::draw(){
    grid.draw(this->window);
    gui.draw(this->window);
}

void Model::handleLeftClick(int mouseX, int mouseY)
{  
    if(app_state->current_state == OpState::Simulate)
    {
        if(grid.edit_state == Editing::Wall || grid.edit_state == Editing::Goal || grid.edit_state == Editing::Spawn)
        {
            // if the user wants to "modify" the grid in any capacity
            grid.set_app_state(OpState::Edit);
            grid.reset_simulation();
            grid.clear_grid();
            grid.handle_leftclick(mouseX, mouseY);
        }
    }
    else if(app_state->current_state == OpState::Edit || app_state->current_state == OpState::ShowPath)
    {
        // modify of grid is ok during edit and showPath since showPath is just edit + active path finding!
        grid.handle_leftclick(mouseX, mouseY);
    }
    else 
    { }
}

void Model::handleLeftReleased()
{
    gui.buttons[current_clicked_button].clicked = false;
}

void Model::handleRightClick(int x, int y){
}

void Model::handleKeyPress(const sf::Event &event)
{
    switch(event.key.code)
    {
        case sf::Keyboard::P:
        {
            grid.clear_grid();
            grid.execute_algorithm();
            grid.set_app_state(OpState::Simulate);
            grid.reset_simulation();
            break;
        }

        case sf::Keyboard::G:
        {
            grid.edit_state = Editing::Goal;
            break;
        }

        case sf::Keyboard::S:
        {
            grid.edit_state = Editing::Spawn;
            break;
        }

        case sf::Keyboard::W:
        {
            grid.edit_state = Editing::Wall;
            break;
        }

        case sf::Keyboard::Escape:
        {
            grid.edit_state = Editing::None;
            break;
        }
    }
}