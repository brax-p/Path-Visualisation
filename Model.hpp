#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include "Grid.hpp"


class Model {
    public:
        Model(Grid& g) : grid(g){}
        void update();
        void draw(sf::RenderWindow &window);
        void toggleTileState(int x, int y);
        std::vector<std::pair<std::string, bool>> states;
        Grid& grid;
    private:
};

void Model::update(){
   
}

void Model::draw(sf::RenderWindow &window){
    this->grid.draw(window);
}

void Model::toggleTileState(int x, int y){
   for(int i = 0; i < grid.tiles.size(); i++){
       sf::RectangleShape t = grid.tiles[i].tile;
       if(x > t.getPosition().x && x < t.getPosition().x+grid.tileLength){
            if(y > t.getPosition().y && y < t.getPosition().y + grid.tileLength){
                if(t.getFillColor() == (sf::Color::White)){
                    grid.tiles[i].tile.setFillColor(sf::Color::Blue);
                } 
                else{
                    grid.tiles[i].tile.setFillColor(sf::Color::White);
                }
            }
       }
   }
}
