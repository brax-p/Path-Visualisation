#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include "Grid.hpp"
#include "GUI.hpp"

template <class T>
int onAnElement(int x, int y, std::vector<std::unique_ptr<T>>& list){
    int idx = 0;
    for(auto& component : list){
        sf::Vector2f size = component->element.getSize();
        sf::Vector2f position = component->element.getPosition();
        if(x > position.x && x < position.x + size.x){
            if(y > position.y && y < position.y + size.y){
                return idx;
            }
        }
        idx++;
    }
    return -1;
}

class Model {
    public:
        Model(Grid& g, sf::RenderWindow& w, GUI& gui_param, AppState& app_state_param) : grid(g), window(w), gui(gui_param), app_state(app_state_param) {
		    states = {"nill","start", "goal", "wall"};
            current_clicked_button = 0;
	    }
        void update(sf::RenderWindow& window, int mouseX, int mouseY, AppState& app_state);
        void draw();
        void handleLeftClick(int x, int y);
        void handleLeftReleased();
        void handleRightClick(int x, int y);
        void printGridTiles();
        int onAButton(int x, int y);

        void setState(std::string state);
	    std::vector<std::string> states;
        
        sf::Color white = sf::Color::White;
        sf::Color blue = sf::Color::Blue;
        sf::Color red = sf::Color::Red;
        sf::Color green = sf::Color::Green;
        sf::Color yellow = sf::Color::Yellow;

        sf::Time delta_time;

        std::string current_state = "";

        bool leftClickDown = false;
        bool rightClickDown = false;
        bool printGrid = true;
        bool debug = false;

        //display_state refers to the current mode of the playback for the algorithms on 
        //the grid. The current states supported are:
        //  0 -- Default state, move the spawn and goal nodes/create walls on the grid
        //  1 -- choose a start/end node then playback the algorithm in real-time, started
        //       by some start button
        
        int display_state = 1;
        int current_clicked_button;
        AppState& app_state;
        GUI& gui;
        Grid& grid;
        sf::RenderWindow& window;
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

void Model::update(sf::RenderWindow &window, int mouseX, int mouseY, AppState& app_state){
    this->current_state = app_state.current_interaction_state;
    if(leftClickDown){
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        handleLeftClick(pos.x, pos.y);
        if(printGrid && debug)
            printGridTiles();
    }
    else if(rightClickDown) {
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        handleRightClick(pos.x, pos.y);
        if(printGrid && debug)
            printGridTiles();
    }
    
    if(leftClickDown == false){
        printGrid = true;
    }
    else if(rightClickDown == false){
        printGrid = true;
    }
    //this->gui.update(mouseX, mouseY);
    grid.update(app_state); 
    //std::cout << (1/app_state.delta_time) << "Frames per second\n";
    for(auto& button: gui.buttons){
        button->update(mouseX, mouseY);
    }
}

void Model::draw(){
    this->grid.draw(this->window);
    this->gui.draw(this->window);
}

int Model::onAButton(int x, int y) {
    int idx = 0;
    for(auto& button: gui.buttons) {
        sf::Vector2f size = button->element.getSize();
        sf::Vector2f position = button->element.getPosition();
        if(x > position.x && x < position.x + size.x){
            if(y > position.y && y < position.y + size.y){
                return idx;
            }
        }
        idx++;
    }
    return -1;
}

void Model::printGridTiles() {
    printGrid = false;
    int idx = 0;
    int width = grid.length;
    for(auto& element: grid.tiles_){
        std::string tile_type = "";
        Type type = element->type();
        if(type == Type::Tile){
            if(element->part_of_path)
                tile_type = "-";
            else
                tile_type = "0";
        }
        else if(type == Type::Wall)
            tile_type = "1";
        else if(type == Type::Spawn)
            tile_type = "S";
        else if(type == Type::Goal)
            tile_type = "G";

        std::cout << tile_type << " ";
        if(idx % width == width - 1)
            std::cout << "\n";
        idx++;
    }
}




void Model::handleLeftClick(int x, int y){
    int test_button = onAnElement(x,y, grid.tiles_);
    std::cout << "button pressed is: " << test_button << "\n";
    int tileNumber = onAnElement(x,y, grid.tiles_);
    int buttonNumber = onAButton(x,y);
    bool onAnElement = (tileNumber != -1 || buttonNumber != -1) ? true : false;
    if(onAnElement == true){
        if(tileNumber > -1){ //if the current left click is ontop of a tile
            Type type = grid.tiles_[tileNumber]->type();
            if(states[0] == "nill"){
                return;
            }
            else if(states[0] == "start"){
                if(type == Type::Tile){
                    //swap logic
                    int startPos = grid.getStartPos();
                    int temp_tileNumber = tileNumber;
                    grid.tiles_[tileNumber]->vertex = startPos;
                    grid.tiles_[startPos]->vertex = temp_tileNumber;
                    std::swap(grid.tiles_[tileNumber], grid.tiles_[grid.getStartPos()]);
                    grid.setStartPos(temp_tileNumber);
                }
            }
            else if(states[0] == "goal"){
                if(type == Type::Tile){
                    int goalPos = grid.getGoalPos();
                    int temp_tileNumber = tileNumber;
                    grid.tiles_[tileNumber]->vertex = goalPos;
                    grid.tiles_[goalPos]->vertex = temp_tileNumber;
                    std::swap(grid.tiles_[tileNumber], grid.tiles_[grid.getGoalPos()]);
                    grid.setGoalPos(temp_tileNumber);
                }
            }
            else if(states[0] == "wall"){
                if(type == Type::Tile){
                    int v = tileNumber;
                    int tL = grid.tiles_[v]->tileLength;
                    sf::Vector2f tile_position = grid.tiles_[v]->element.getPosition();
                    grid.tiles_.emplace_back(new Wall(v,tL, tile_position));
                    int idx = grid.tiles_.size()-1;
                    std::swap(grid.tiles_[v], grid.tiles_[idx]);
                    grid.tiles_.erase(grid.tiles_.begin()+idx+1);
                }
                //Handle adjacency list changes needed;
                grid.removeVertex(tileNumber);
            }
        }
        else if(buttonNumber > -1){
            //std::cout << "button number is: " << buttonNumber << "\n";
            gui.buttons[buttonNumber]->clicked = true;   
            current_clicked_button = buttonNumber;
            if(gui.buttons[buttonNumber]->button_text == "Start Simulation") {
                int sim_idx = 0;
                for(auto& state : app_state.interaction_states){
                    if(state=="Simulation"){
                        
                    }
                    sim_idx++;
                }
            }
        }
    }
}

void Model::handleLeftReleased() {
        gui.buttons[current_clicked_button]->clicked = false;
}
void Model::handleRightClick(int x, int y){
    int tileNumber = onAnElement(x,y, grid.tiles_);
    if(tileNumber != -1){
        Type type = grid.tiles_[tileNumber]->type();
        if(type == Type::Wall){
            int v = tileNumber;
            int tile_length = grid.tiles_[v]->tileLength;
            sf::Vector2f tile_position = grid.tiles_[v]->element.getPosition();
            grid.tiles_.emplace_back(new Tile(v,tile_length, tile_position));
            int size = grid.tiles_.size();
            std::swap(grid.tiles_[v], grid.tiles_[size-1]);
            grid.tiles_.pop_back();
            grid.addVertex(tileNumber);
        }
    }
}

