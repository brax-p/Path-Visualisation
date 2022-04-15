#include "Model.hpp"
#include <iostream>


class Controller{

    public:
        Controller(Model& m) : model(m){}
        void updateEvent(sf::Event &event);
        void update(sf::RenderWindow &window);
    
    private:
        Model& model;

};

void Controller::update(sf::RenderWindow &window){
    this->model.update();
}

void Controller::updateEvent(sf::Event &event){
    if(event.type == sf::Event::MouseButtonPressed){
        if(event.mouseButton.button == sf::Mouse::Right){
            model.toggleTileState(event.mouseButton.x, event.mouseButton.y);
        }
    }       
    else if(event.type == sf::Event::KeyPressed){
        if(event.key.code == sf::Keyboard::B){
            this->model.grid.bfs();
        }
    }
}
