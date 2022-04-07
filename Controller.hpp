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
    
}