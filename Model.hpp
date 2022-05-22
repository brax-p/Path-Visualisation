#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include "Grid.hpp"
#include "GUI.hpp"

class Model {
    public:
        Model(Grid& g, sf::RenderWindow& w, GUI& gui_param) : grid(g), window(w), gui(gui_param){
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

        //display_state refers to the current mode oof the playback for the algorithms on 
        //the grid. The current states supported are:
        //  0 -- Default state, move the spawn and goal nodes/create walls on the grid
        //  1 -- choose a start/end node then playback the algorithm in real-time, started
        //       by some start button
        
        int display_state = 1;

        Grid& grid;
        GUI& gui;
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
    //Hover Logic

    //this->gui.update(mouseX, mouseY);
    grid.update(app_state); 
    gui.update();
    std::cout << (1/app_state.delta_time) << "Frames per second\n";
}

void Model::draw(){
    this->grid.draw(this->window);
    gui.draw(window);
}

int Model::onATile(int x, int y){
    /*
   for(int i = 0; i < grid.tiles.size(); i++){
       sf::RectangleShape t = grid.tiles[i].tile;
       if(x > t.getPosition().x && x < t.getPosition().x+grid.tileLength){
            if(y > t.getPosition().y && y < t.getPosition().y + grid.tileLength){
                return i;
            }
       }
   }
   */
    
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

    //needs to work with enum of elements in gui
    
    int idx = 0;
    for(auto& button : gui.buttons_){
        sf::Vector2f pos = button->button.getPosition();
        sf::Vector2f size = button->button.getSize();
        if(x > pos.x && x < pos.x + size.x){
            if(y > pos.y && y < pos.y + size.y){
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
    int button_idx = onAButton(x,y);



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
    else if(button_idx > -1){
        auto& button = gui.simulation_options[button_idx];
        if(button.clicked == false){
            button.clicked = true;
        }
    }

}

void Model::handleLeftReleased() {
    for(auto& button : gui.buttons_){
        button->clicked = false;
    }
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
            //grid.tiles_.erase(grid.tiles_.begin()+size);
            grid.addVertex(tileNumber);
        }
    }
}

