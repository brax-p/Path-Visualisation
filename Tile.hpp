#include <SFML/Graphics.hpp>

enum class Type {Tile, Spawn, Goal, Wall};

class Tile {
    public:
        Tile(int v, int tL, sf::Vector2f tilePosition);
        virtual ~Tile() = default;

        int vertex; // Corresponds to the index within the adjList, AKA
                    // vertex = index;
        
        bool part_of_path = false;
        int tileLength;
        sf::RectangleShape tile;
        
        virtual void update() {

            if(this->part_of_path)
                tile.setFillColor(sf::Color::Green);
            else if(this->type() == Type::Tile){
                tile.setFillColor(sf::Color::White);
            }
        }

        virtual Type type() const {
            return Type::Tile;
        }

        void draw(sf::RenderWindow& window) {
            window.draw(this->tile);
        }

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


class Spawn : public Tile {
    public:
        Spawn(int v, int tL, sf::Vector2f tile_position);
        Type type() const override {
            return Type::Spawn;
        }
};

Spawn::Spawn(int v, int tL, sf::Vector2f tile_position) : Tile(v, tL, tile_position){
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


class Goal : public Tile {
    public:
        Goal(int v, int tL, sf::Vector2f tile_position);
        Type type() const override {
            return Type::Goal;
        }
};

Goal::Goal(int v, int tL, sf::Vector2f tile_position) : Tile(v, tL, tile_position) {
    tile.setFillColor(sf::Color::Red);
}


class Wall : public Tile {
    public:
        Wall(int v, int tL, sf::Vector2f tile_position);
        Type type() const override {
            return Type::Wall;
        }
};

Wall::Wall(int v, int tL, sf::Vector2f tile_position) : Tile(v, tL, tile_position){
        tile.setFillColor(sf::Color::Blue);
}

