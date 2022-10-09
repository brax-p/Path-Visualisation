#pragma once
#include "Grid.hpp"

enum class ModelType { NONE, GRID, TREE };
class AlgoModels
{
public: 
    Grid grid;
    AppState& appState;
    sf::Vector2f displayOrigin;
    sf::Vector2f displaySize;
    sf::RenderWindow& window;
    ModelType currentType;

    AlgoModels(AppState& p_appState, sf::Vector2f p_displayOrigin, sf::Vector2f p_displaySize, sf::RenderWindow& w) :
                grid(15, p_appState, p_displayOrigin, p_displaySize),
                window(w),
                appState(p_appState)
    {
        displayOrigin = p_displayOrigin;
        displaySize = p_displaySize;
        currentType = ModelType::GRID; // default
    }

    void draw()
    {
        if(appState.current_state == OpState::INIT)
            return;
        
        if(currentType == ModelType::GRID)
        {
            grid.draw(window);
        }
        else if(currentType == ModelType::TREE)
        {
            //tree.draw(window);
        }
    }

    void update()
    {
        if(currentType == ModelType::GRID)
        {
            grid.update();
        }
        else if(currentType == ModelType::TREE)
        {
            //tree.update();
        }
    }

    void allowDisplay()
    {
        if(currentType == ModelType::GRID)
        {
            //tree.isVisible = false;
            grid.isVisible = true;
        }
        else if(currentType == ModelType::TREE)
        {
            grid.isVisible = false;
            //tree.isVisible = false;
        }
    }

    void disallowDisplay()
    {
        if(currentType == ModelType::GRID)
        {
            grid.isVisible = false;
        }
        if(currentType == ModelType::TREE)
        {
            //tree.isVisible = false;
        }
    }

    void handleLeftClick(int x, int y)
    {
        if(currentType == ModelType::GRID)
        {
            grid.handleLeftClick(x,y);
        }
    }

    void handleRightClick(int x, int y)
    {
        grid.handleRightClick(x, y);
    }

    void handleKeyPress(const sf::Event& event)
    {
        if(currentType == ModelType::GRID)
        {
            grid.handleKeyPress(event);
        }
    }
};
