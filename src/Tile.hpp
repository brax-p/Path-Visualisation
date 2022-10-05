#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

enum class Type {Blank, Spawn, Goal, Wall, Path};

class Tile 
{
    public:
        Tile(const uint16_t& p_vertex, const float& p_tile_length, const sf::Vector2f& p_tile_position, Type p_tileType);
        void draw(sf::RenderWindow &window);
        void update();
        void assign_type();
        bool is_intersecting(int x, int y);
        void set_type(Type type);
        sf::Vector2f getSize();
        Type get_type();
        Type tileType;
        bool part_of_path;
        uint16_t vertex;

    private:
        sf::Vector2f m_size;
        sf::Vector2f m_position;
        sf::RectangleShape m_tile;
};

Tile::Tile(const uint16_t& p_vertex, const float& p_tile_length, const sf::Vector2f& p_tile_position, Type p_tileType=Type::Blank)
{
    vertex = p_vertex;

    part_of_path = false;

    m_size.x = p_tile_length; 
    m_size.y = p_tile_length;

    m_position = p_tile_position;

    tileType = p_tileType;

    m_tile.setSize(m_size);
    m_tile.setPosition(m_position);
    sf::Color tile_color;
    switch(tileType)
    {
        case Type::Blank:
          tile_color = sf::Color::White;
          break;
        case Type::Goal:
            tile_color = sf::Color::Red;
            break;
        case Type::Spawn:
            tile_color = sf::Color::Yellow;
            break;
        case Type::Wall:
            tile_color = sf::Color::Blue;
            break;
        case Type::Path:
            tile_color = sf::Color::Green;
            break;
    }
    m_tile.setFillColor(tile_color);
    m_tile.setOutlineColor(sf::Color::Blue);
    m_tile.setOutlineThickness(1);
}


void Tile::draw(sf::RenderWindow &window)
{
    window.draw(m_tile);
}

void Tile::update()
{
    if(part_of_path)
    {
        tileType = Type::Path;
    }
    else
    {
        if(tileType == Type::Path)
        {
            tileType = Type::Blank;
        }
    }

    switch(tileType)
    {
        case Type::Path:
            m_tile.setFillColor(sf::Color::Green);
            break;
        
        case Type::Wall:
            m_tile.setFillColor(sf::Color::Magenta);
            break;

        case Type::Blank:
            m_tile.setFillColor(sf::Color::White);
            break;
        
        case Type::Spawn:
            m_tile.setFillColor(sf::Color::Yellow);
            break;
        
        case Type::Goal:
            m_tile.setFillColor(sf::Color::Red);
            break;
        
        default:
            break;
    }
}

bool Tile::is_intersecting(int x, int y)
{
    sf::Vector2f pos = m_tile.getPosition();
    sf::Vector2f size = m_tile.getSize();

    if(x > pos.x && x < pos.x + size.x)
    {
        if(y > pos.y && y < pos.y + size.y)
        {
            return true;
        }
    }
    return false;
}

void Tile::set_type(Type type)
{
    tileType = type;
}

Type Tile::get_type()
{
    return tileType;
}

sf::Vector2f Tile::getSize()
{
    return m_size;
}
