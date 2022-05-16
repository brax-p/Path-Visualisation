#include "Model.hpp"
#include <iostream>


class Controller{

    public:
        Controller(Model& m) : model(m){
        }
        
        void updateEvent(sf::Event &event);
        void update(sf::RenderWindow &window, AppState& app_state);

        sf::Time delta_time;
    
    private:
        Model& model;

};

void Controller::update(sf::RenderWindow &window, AppState& app_state){
    this->delta_time = delta_time;
    sf::Vector2i mouse_position = sf::Mouse::getPosition();
    int x = mouse_position.x;
    int y = mouse_position.y;
    this->model.update(window, x, y,app_state);
}

void Controller::updateEvent(sf::Event &event){
    int x = event.mouseButton.x, y = event.mouseButton.y;
    if(event.type == sf::Event::MouseButtonPressed){
        if(event.mouseButton.button == sf::Mouse::Left){
            this->model.leftClickDown = true;
            this->model.handleLeftClick(x,y);
        }
        else if(event.mouseButton.button == sf::Mouse::Right){
            this->model.handleRightClick(x,y);
        }
    }
    else if(event.type == sf::Event::MouseButtonReleased){
        if(event.mouseButton.button == sf::Mouse::Left){
            this->model.leftClickDown = false; 
        }
    }
    else if(event.type == sf::Event::KeyPressed){
        if(event.key.code == sf::Keyboard::B){
            //this->model.grid.bfs();
        }
        else if(event.key.code == sf::Keyboard::S){
            this->model.setState("start");  
        }
        else if(event.key.code == sf::Keyboard::G){
            this->model.setState("goal");
        }
        else if(event.key.code == sf::Keyboard::W){
            this->model.setState("wall");
        }
        else if(event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::Left){
            this->model.grid.incrementStep(event);  
        }
    }
    else if(event.type == sf::Event::MouseWheelScrolled){
        if(event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel){
            //pos is up scroll
            //negative is down scroll
            if(event.mouseWheelScroll.delta == 1){
                this->model.grid.tileLength+=1;
            }
            else if(event.mouseWheelScroll.delta == -1){
                this->model.grid.tileLength-=1;
            }
        }
    }
}
