#include <SFML/Graphics.hpp>

class Tile {
    public:
        Tile(int v, int tL, sf::Vector2f tilePosition);
        int vertex;
        int tileLength;
        sf::RectangleShape tile;
    private:
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