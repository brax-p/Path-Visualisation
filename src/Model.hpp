#pragma once
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include "Grid.hpp"
#include "GUI.hpp"
#include "AppState.hpp"
#include "AlgoModels.hpp"

class Model {
    public:
        Model(sf::RenderWindow& w, GUI& gui_param, AppState& p_appState, AlgoModels& p_algoModel);
        void update(sf::RenderWindow& window, int mouseX, int mouseY);
        void draw();
        void handleLeftClick(int mouseX, int mouseY);
        void handleSingleLeftClick(int mouseX, int mouseY);
        void handleLeftReleased();
        void handleRightClick(int x, int y);
        void handleKeyPress(const sf::Event& event);

        bool leftClickDown = false;
        bool rightClickDown = false;
        bool printGrid = true;
        bool debug = false;

        GUI& gui;
        sf::RenderWindow& window;
        AppState& appState;
        OpState default_state = OpState::ShowPath;
        AlgoModels& algoModels;
};

Model::Model(sf::RenderWindow& w, GUI& gui_param, AppState& p_appState, AlgoModels& p_algoModels) : 
    window(w),
    gui(gui_param),
    appState(p_appState),
    algoModels(p_algoModels)
{

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
    algoModels.update();
}

void Model::draw()
{
        algoModels.draw();
}

void Model::handleLeftClick(int mouseX, int mouseY)
{  
    algoModels.handleLeftClick(mouseX, mouseY);
}

void Model::handleLeftReleased()
{

}

void Model::handleRightClick(int x, int y)
{
    algoModels.handleRightClick(x, y);
}

void Model::handleKeyPress(const sf::Event &event)
{
    algoModels.handleKeyPress(event);
}

void Model::handleSingleLeftClick(int mouseX, int mouseY)
{

}

