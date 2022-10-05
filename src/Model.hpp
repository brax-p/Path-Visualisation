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
        Model(Grid& g, sf::RenderWindow& w, GUI& gui_param, AppState* p_appState);
        void update(sf::RenderWindow& window, int mouseX, int mouseY);
        void draw();
        void handleLeftClick(int mouseX, int mouseY);
        void handleSingleLeftClick(int mouseX, int mouseY);
        void handleLeftReleased();
        void handleRightClick(int x, int y);
        void handleKeyPress(const sf::Event& event);

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
        int maxGridWidth = 25;

        GUI& gui;
        Grid& grid;
        sf::RenderWindow& window;
        AppState* appState;
        OpState default_state = OpState::ShowPath;
};

Model::Model(Grid& g, sf::RenderWindow& w, GUI& gui_param, AppState* p_appState) : grid(g), window(w), gui(gui_param), appState(p_appState)
{
    current_clicked_button = 0;
}


void Model::update(sf::RenderWindow &window, int mouseX, int mouseY){
    if(leftClickDown && !rightClickDown)
    {
        handleLeftClick(mouseX, mouseY);
    }
    if(rightClickDown && !leftClickDown)
    {
        handleRightClick(mouseX, mouseY);
    }
   grid.update(); 
    //std::cout << (1/appState.delta_time) << "Frames per second\n";
    /*
    for(auto& button: gui.buttons){
        button.update(mouseX, mouseY);
    }
    */
}

void Model::draw(){
    if(appState->current_state != OpState::INIT)
        grid.draw(this->window);
}

void Model::handleLeftClick(int mouseX, int mouseY)
{  
    //perform a search to determine which elements, if any, need to perform handleLeftClick -- optimization?
    //grid.handleLeftClick(mouseX, mouseY);
    grid.handleLeftClick(mouseX, mouseY);
    //gui.handleLeftClick(grid, mouseX, mouseY);
}

void Model::handleLeftReleased()
{
    //gui.buttons[current_clicked_button].clicked = false;
}

void Model::handleRightClick(int x, int y)
{
    grid.handleRightClick(x, y);
}

void Model::handleKeyPress(const sf::Event &event)
{
    switch(event.key.code)
    {
        case sf::Keyboard::P:
        {
            //grid.Simulate();
            //appState->current_state = OpState::Simulate;
            break;
        }

        case sf::Keyboard::G:
        {
            grid.editState = Editing::Goal;
            break;
        }

        case sf::Keyboard::S:
        {
            grid.editState = Editing::Spawn;
            break;
        }

        case sf::Keyboard::W:
        {
            grid.editState = Editing::Wall;
            break;
        }

        case sf::Keyboard::Escape:
        {
            window.close();
            break;
        }

        case sf::Keyboard::U:
        {   
            appState->current_state = OpState::Edit;
            int currentWidth = grid.getGridWidth();
            if(currentWidth < maxGridWidth)
            {
                grid.setGridWidth(grid.getGridWidth() + 1);
                grid.updateGridValues();
            }
            break;
        }

        case sf::Keyboard::J:
        {
            appState->current_state = OpState::Edit;
            int currentWidth = grid.getGridWidth();
            if(currentWidth > 3)
            {
                grid.setGridWidth(grid.getGridWidth() - 1);
                grid.updateGridValues();
            }
            break;
        }
    }
}

void Model::handleSingleLeftClick(int mouseX, int mouseY)
{

}

