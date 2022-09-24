#pragma once
#include "Model.hpp"
#include <iostream>


class Controller{

    public:
        Controller(Model& m);
        void update_event(const sf::Event &event);
        void update(sf::RenderWindow &window);
        
        int delta_time;
    private:
        Model& model;

};

Controller::Controller(Model &m) : model(m) { }

void Controller::update(sf::RenderWindow &window){
    sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
    int x = mouse_position.x;
    int y = mouse_position.y;
    this->model.update(window, x, y);
}

void Controller::update_event(const sf::Event &event){
    int x = event.mouseButton.x, y = event.mouseButton.y;
    switch(event.type)
    {
        case sf::Event::MouseButtonPressed:
        {   
            if(event.mouseButton.button == sf::Mouse::Left)
            {
                model.leftClickDown = true;
            }
            break;
        }

        case sf::Event::MouseButtonReleased:
        {
            if(event.mouseButton.button == sf::Mouse::Left)
            {
                model.leftClickDown = false;
            }
            break;
        }

        case sf::Event::KeyPressed:
        {
            model.handleKeyPress(event);
            break;
        }
    }
}
