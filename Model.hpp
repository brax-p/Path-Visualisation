#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include "Grid.hpp"
#include "GUI.hpp"

class Model {
    public:
        Model(Grid& g, sf::RenderWindow& w, GUI& gui_param) : grid(g), window(w), gui(gui_param) {
		    states = {"nill","start", "goal", "wall"};
	    }
        void update(sf::RenderWindow& window, int mouseX, int mouseY, AppState& app_state);
        void draw();
        void handleLeftClick(int x, int y);
        void handleLeftReleased();
        void handleRightClick(int x, int y);
        void printGridTiles();
        int onATile(int x, int y);
        int onAButton(int x, int y);

        void setState(std::string state);
	    std::vector<std::string> states;
        
        sf::Color white = sf::Color::White;
        sf::Color blue = sf::Color::Blue;
        sf::Color red = sf::Color::Red;
        sf::Color green = sf::Color::Green;
        sf::Color yellow = sf::Color::Yellow;

        sf::Time delta_time;

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
    this->delta_time = delta_time;
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

int Model::onATile(int x, int y){
    for(auto& tile: grid.tiles_){
        if(x > tile->tile.getPosition().x && x < tile->tile.getPosition().x + tile->tile.getSize().x){
            if(y > tile->tile.getPosition().y && y < tile->tile.getPosition().y + tile->tile.getSize().y){
                return tile->vertex;
            }
        }
    }
   return -1;
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
    for(auto& tile: grid.tiles_){
        std::string tile_type = "";
        Type type = tile->type();
        if(type == Type::Tile){
            if(tile->part_of_path)
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

    int tileNumber = onATile(x,y);



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
                sf::Vector2f tile_position = grid.tiles_[v]->tile.getPosition();
                grid.tiles_.emplace_back(new Wall(v,tL, tile_position));
                int idx = grid.tiles_.size()-1;
                std::swap(grid.tiles_[v], grid.tiles_[idx]);
                grid.tiles_.erase(grid.tiles_.begin()+idx+1);
            }
            //Handle adjacency list changes needed;
            grid.removeVertex(tileNumber);
        }
    }
}

void Model::handleLeftReleased() {

}
void Model::handleRightClick(int x, int y){
    int tileNumber = onATile(x,y);
    if(tileNumber != -1){
        Type type = grid.tiles_[tileNumber]->type();
        if(type == Type::Wall){
            int v = tileNumber;
            int tile_length = grid.tiles_[v]->tileLength;
            sf::Vector2f tile_position = grid.tiles_[v]->tile.getPosition();
            grid.tiles_.emplace_back(new Tile(v,tile_length, tile_position));
            int size = grid.tiles_.size();
            std::swap(grid.tiles_[v], grid.tiles_[size-1]);
            grid.tiles_.pop_back();
            grid.addVertex(tileNumber);
        }
    }
}

