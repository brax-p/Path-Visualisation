#pragma once
#include "Grid.hpp"

enum class ModelType { NONE, GRID, TREE };
class AlgoModel 
{
public: 
    Grid grid;
    AlgoModel() :
                grid(15, appState, algoDisplayPos, algoDisplaySize);
    {

    }
    ModelType currentType;
    void setModelType(ModelType p_newType)
    {
        currentType = p_newType;
    }
    void draw()
    {
        if(currentType == ModelType::GRID)
        {
            //grid draw
        }
        else if(currentType == ModelType::TREE)
        {

        }
    }

    void update()
    {
        if(currentType == ModelType::GRID)
        {

        }
        else if(currentType == ModelType::TREE)
        {

        }
    }

private:

};
