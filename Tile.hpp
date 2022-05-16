#include <SFML/Graphics.hpp>

class Tile {
    public:
        Tile();
        Tile(int v, int tL, sf::Vector2f tilePosition);
        int vertex; // Corresponds to the index within the adjList, AKA
                    // vertex = index for simplicity
        
        int tileLength;
        sf::RectangleShape tile;
    protected:
        sf::Vector2f size;
        sf::Vector2f position;
};

Tile::Tile(int v, int tL, sf::Vector2f tilePosition){
    this->vertex = v;
    this->tileLength = tL;
    size.x = tileLength; size.y = tileLength;
    position = tilePosition;

    //Create Tile
    tile.setSize(size);
    tile.setPosition(position);
    tile.setFillColor(sf::Color::White);
    tile.setOutlineColor(sf::Color::Blue);
    tile.setOutlineThickness(2);
}


class Spawn : Tile {
    public:
        Spawn(int v, int tL, sf::Vector2f tile_position);
};

Spawn::Spawn(int v, int tL, sf::Vector2f tile_position) {
    /*vertex = v;   
    tileLength = tL;

    size.x = tileLength;
    size.y = tileLength;
    
    position = tile_position;

    tile.setSize(size);
    tile.setPosition(position);*/
    tile.setFillColor(sf::Color::Yellow);
    /*tile.setOutlineColor(sf::Color::Blue);
    tile.setOutlineThickness(2);*/
    
}


class Goal : Tile {
    public:
        Goal(int v, int tL, sf::Vector2f tile_position);
};

Goal::Goal(int v, int tL, sf::Vector2f tile_position) {
    tile.setFillColor(sf::Color::Red);
}


class Wall : Tile {
    public:
        Wall(int v, int tL, sf::Vector2f tile_position);
};

Wall::Wall(int v, int tL, sf::Vector2f tile_position){
        
}


//TODO:
// Create derived classes from Tile which implement the 3 types: spawn, goal, and wall --
// This is to hopefully reduce complexity when adding multiple spawns, goals, walls and in the
// future, weights from tile to tile.
