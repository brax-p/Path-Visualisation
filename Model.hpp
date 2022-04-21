#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include "Grid.hpp"


class Model {
    public:
        Model(Grid& g) : grid(g){
		    states = {"nill","start", "goal", "wall"};
	    }
        void update(sf::RenderWindow& window);
        void draw(sf::RenderWindow &window);
        void handleLeftClick(int x, int y);
        void handleRightClick(int x, int y);
        int onATile(int x, int y);

        void setState(std::string state);
	    std::vector<std::string> states;
        
        sf::Color white = sf::Color::White;
        sf::Color blue = sf::Color::Blue;
        sf::Color red = sf::Color::Red;
        sf::Color green = sf::Color::Green;
        sf::Color yellow = sf::Color::Yellow;

        bool leftClickDown = false;
        bool rightClickDown = false;
        Grid& grid;
};

void Model::setState(std::string state){
    if(states[0] != state){
        for(int k = 1; k < states.size(); k++){
            if(states[k] == state){
                std::swap(states[0], states[k]);
            }
        }
    }
}

void Model::update(sf::RenderWindow &window){
    if(leftClickDown){
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        handleLeftClick(pos.x, pos.y);
    }
    grid.update(); 
}

void Model::draw(sf::RenderWindow &window){
    this->grid.draw(window);
}

int Model::onATile(int x, int y){
   for(int i = 0; i < grid.tiles.size(); i++){
       sf::RectangleShape t = grid.tiles[i].tile;
       if(x > t.getPosition().x && x < t.getPosition().x+grid.tileLength){
            if(y > t.getPosition().y && y < t.getPosition().y + grid.tileLength){
                return i;
            }
       }
   }
   return -1;
}

void Model::handleLeftClick(int x, int y){
    int tileNumber = onATile(x,y);
    if(tileNumber != -1){ //if the current left click is ontop of a tile
        if(states[0] == "nill"){
            return;
        }
        else if(states[0] == "start"){
            if(grid.tiles[tileNumber].tile.getFillColor() == white){
                grid.tiles[grid.getStartPos()].tile.setFillColor(white);
                grid.tiles[tileNumber].tile.setFillColor(yellow);
                grid.setStartPos(tileNumber);
            }
        }
        else if(states[0] == "goal"){
            grid.tiles[grid.getGoalPos()].tile.setFillColor(white);
            grid.tiles[tileNumber].tile.setFillColor(red);
            grid.setGoalPos(tileNumber);
        }
        else if(states[0] == "wall"){
            grid.tiles[tileNumber].tile.setFillColor(blue);
            //Handle adjacency list changes needed;
            grid.removeVertex(tileNumber);
        }
    }
}

void Model::handleRightClick(int x, int y){
    int tileNumber = onATile(x,y);
    if(tileNumber != -1){
        if(grid.tiles[tileNumber].tile.getFillColor() == blue){
            grid.tiles[tileNumber].tile.setFillColor(white);
            grid.addVertex(tileNumber);
        }
    }
}





