#include "Tile.hpp"
#include "AppState.hpp"
#include <vector>
#include <iostream>
#include <stack>
#include <queue>
#include <limits>


// TODO:
// 
// Alter Grid class such that each Tile and its derivates contains a list of its neighbors
// Assign to each Tile or its derivates an index that corresponds to its position in the
// list of Tile/derivates objects

const int INF = std::numeric_limits<int>::max();
class Grid {
    public:
        Grid(int grid_length);

        std::vector<Tile> tiles;
        std::vector<std::vector<int>> adjList;
        std::vector<std::vector<int>> defaultGridList;
        std::vector<std::vector<int>> pathSteps;
        std::vector<std::vector<int>> total_algorithm_steps;
        std::vector<int> node_summation;
        std::vector<int> walls;

        void draw(sf::RenderWindow& window);
        void update(AppState& app_state);

        void dfs();
        void bfs();
        void newBfs();

        void printVector(std::vector<int>& vec);

        void showSinglePath(std::vector<int>& path);
        void simulateAlgorithm(std::vector<std::vector<int>>& all_paths);

        void setStartPos(int startPos){this->startPos = startPos;}
        int getStartPos(){return this->startPos;}

        void setGoalPos(int goalPos) {this->goalPos = goalPos;}
        int getGoalPos(){return this->goalPos;}

        void setDefaultGridState();
        void resetConfigurationState();

        void removeVertex(int tileNumber);
        void addVertex(int tileNumber);

        void incrementStep(sf::Event& event);
        void showPath();


        int iteration_factor = 5000;
        int iteration = 0;
        sf::Time delta_time;
        int display_state = 1;
        bool activePathing = true;
        int tileLength = 50;
        int prevTileLength = 50;
        int step = 0;
    private:
        int length;
        sf::Vector2f leftCornerPosition;
        int startPos = 0;
        int goalPos;
};

void Grid::removeVertex(int tileNumber){
    std::vector<int> neighbors = adjList[tileNumber];
    walls.push_back(tileNumber);
    for(int i = 0; i < neighbors.size(); i++){
        std::vector<int> current = adjList[neighbors[i]];
        for(int k = 0; k < current.size(); k++){
            if(current[k] == tileNumber){
                adjList[neighbors[i]].erase(adjList[neighbors[i]].begin()+k);
            } 
        }
    }
}

void Grid::addVertex(int tileNumber){
    int rowSize = this->length;
    int row = tileNumber / rowSize;
    bool leftEdge = true, rightEdge = true;
    
    int topTile = tileNumber - rowSize;
    int leftTile = tileNumber - 1;
    int rightTile = tileNumber + 1;
    int bottomTile = tileNumber + rowSize;

    (tileNumber % rowSize == 0) ? leftEdge = true: leftEdge = false;
    (tileNumber % rowSize == rowSize-1) ? rightEdge = true: rightEdge = false;

    if(leftEdge){
        adjList[tileNumber].push_back(rightTile);
        if(row == 0){
            adjList[tileNumber].push_back(bottomTile);
        }
        else if(row == rowSize - 1){
            adjList[tileNumber].push_back(topTile);
        }
        else{
            adjList[tileNumber].push_back(bottomTile);
            adjList[tileNumber].push_back(topTile);
        }
    }
    else if(rightEdge){
        adjList[tileNumber].push_back(leftTile);
        if(row == 0){
            adjList[tileNumber].push_back(bottomTile);
        }
        else if(row == rowSize - 1){
            adjList[tileNumber].push_back(topTile);
        }
        else{
            adjList[tileNumber].push_back(bottomTile);
            adjList[tileNumber].push_back(topTile);
        }
    }
    else{
        adjList[tileNumber].push_back(leftTile);
        adjList[tileNumber].push_back(rightTile);
        if(row == 0){
            adjList[tileNumber].push_back(bottomTile);
        }
        else if(row == rowSize - 1){
            adjList[tileNumber].push_back(topTile);
        }
        else{
            adjList[tileNumber].push_back(bottomTile);
            adjList[tileNumber].push_back(topTile);
        }
    }
    for(auto vertex: adjList[tileNumber]){
        adjList[vertex].push_back(tileNumber);     
    }
}

void Grid::printVector(std::vector<int>& vec){
    for(auto v: vec){
        std::cout << v << " ";
    }
    std::cout << std::endl;
}

Grid::Grid(int grid_length){
    this->length = grid_length;
    std::vector<std::vector<int>> vec(length*length);
    pathSteps = vec;
    this->goalPos = length*length-1;
    leftCornerPosition.x = 100.0;
    leftCornerPosition.y = 100.0;

    //Create a Grid of Tile(s);
    for(int i = 0; i < length; i++){
        for(int k = 0; k < length; k++){
            sf::Vector2f tilePosition;
            tilePosition.x = k*tileLength + leftCornerPosition.x;
            tilePosition.y = i*tileLength + leftCornerPosition.y;
            Tile t((i*length+k), tileLength, tilePosition); // kth element of the ith row in form (i*length + k)
            tiles.push_back(t);

            //Create adjacency Matrix that models the grid
        }
    }

    //setting default start and goal tiles' color
    tiles[0].tile.setFillColor(sf::Color::Yellow);
    tiles[length*length-1].tile.setFillColor(sf::Color::Red);

    for(int i = 0; i < length; i++){
        for(int k = 0; k < length; k++){
            std::vector<int> currentList;
            //right ==> (i*length) + k + 1;
            //left ==> (i*length) + k - 1;
            //up ==> (i-1)*length + k;
            //down ==> (i+1) * length + k;
            int right = i*length + k + 1;
            int left = i*length + k - 1;
            int up = (i-1)*length + k;
            int down = (i+1)*length + k;
            if(i == 0){
               if(k == 0){
                   //only right and down
                    currentList.push_back(right);
                    currentList.push_back(down);
               }
               else if(k == length - 1){
                   currentList.push_back(left);
                   currentList.push_back(down);
               }
               else{
                    currentList.push_back(right);
                    currentList.push_back(down);
                    currentList.push_back(left);
               }
            }
            else if(i == length - 1){
                if(k == 0){
                    //only right and down
                    currentList.push_back(right);
                    currentList.push_back(up);
                }
                else if(k == length - 1){
                    currentList.push_back(left);
                    currentList.push_back(up);
                }
                else {
                    currentList.push_back(right);
                    currentList.push_back(left);
                    currentList.push_back(up);
               } 
            }
            else{
                if(k == 0){
                    //only right and down
                    currentList.push_back(right);
                    currentList.push_back(down);
                    currentList.push_back(up);
                }
                else if(k == length - 1){
                    currentList.push_back(left);
                    currentList.push_back(down);
                    currentList.push_back(up);
                }
                else{
                    currentList.push_back(right);
                    currentList.push_back(down);
                    currentList.push_back(left);
                    currentList.push_back(up);
               }
            }
            sort(currentList.begin(), currentList.end());
            adjList.push_back(currentList);
        }
    }
    defaultGridList = adjList;
}

void Grid::draw(sf::RenderWindow &window){
    for(auto k: tiles)
        window.draw(k.tile);
}

void Grid::setDefaultGridState(){
    adjList = defaultGridList;
    tiles[startPos].tile.setFillColor(sf::Color::Yellow);
    tiles[goalPos].tile.setFillColor(sf::Color::Red);
    for(int i = 0; i < tiles.size(); i++){
        if(i != startPos && i != goalPos && tiles[i].tile.getFillColor() != sf::Color::Blue)
            tiles[i].tile.setFillColor(sf::Color::White);
    }
}

void Grid::resetConfigurationState(){
    for(int i = 0; i < tiles.size(); i++){
        if(tiles[i].tile.getFillColor() == sf::Color::Green){
            tiles[i].tile.setFillColor(sf::Color::White);
        }
    }
}

void Grid::update(AppState& app_state){
    
    int current_state_idx = app_state.current_interaction_state;
    std::string current_state = app_state.interaction_states[current_state_idx];
    if(current_state == "Playground"){
        resetConfigurationState();
        bfs();
        if(!activePathing)
            showPath();

    }
    else if(current_state == "Simulate"){

    }
    /*
        //run algorithm
        // pseudo logic:
        //
        //      if currently running simulation,
        //          every nth factor (when count % n == 0)
        //          display next sequence of node discovery
        //          
        //          assuming the Data Structure is a vector of vectors where
        //          each vector in the vector of vectors contains
        //          a list of nodes being discovered that step + all other
        //          nodes already discovered, there number of iterations needed is
        //          vector.size() * n where vector.size() is the size of the vector
        //          of vectors
        //
        //          In this context, since this->update() is executed every
        //          program count, then at each iteration, perform the following:
        //          vector current = vector sum_discovered_nodes
        //          vector current += vector nth_step_discovered_nodes
        //
        //          this way, the current vector being used contains the previous
        //          list of all discoverd nodes as well as the nodes discovered
        //          at the current step. This allows for easy drawing each iteration 
        //          and understanding about which nodes in the grid are supposed
        //          to be discovered or not -- albeit not walls or spawn or goal
        //          or just undiscovered

    }*/



    //adjusting grid size if resized by the scroll up or scroll down action
    if(tileLength > prevTileLength){
        int diff = tileLength - prevTileLength;
        for(int i = 0; i < length; i++){
            for(int k = 0; k < length; k++){
                sf::Vector2f size(tileLength, tileLength);
                sf::Vector2f pos = tiles[i*length+k].tile.getPosition();
                pos.x+=k*diff;
                pos.y+=i*diff;
                tiles[i*length+k].tile.setPosition(pos);
                tiles[i*length+k].tile.setSize(size);
            } 
        }
    }
    else if(tileLength < prevTileLength){
        int diff = prevTileLength - tileLength;
        for(int i = 0; i < length; i++){
            for(int k = 0; k < length; k++){
                sf::Vector2f size(tileLength, tileLength);
                sf::Vector2f pos = tiles[i*length+k].tile.getPosition();
                pos.x-=k*diff; 
                pos.y-=i*diff;
                tiles[i*length+k].tile.setPosition(pos);
                tiles[i*length+k].tile.setSize(size);
            }
        }
    }
    prevTileLength = tileLength;
}

void Grid::dfs(){
    int listSize = adjList.size();
    std::vector<bool> visited(listSize, false);
    std::vector<int> previous(listSize);

    std::stack<int> stack;
    stack.push(startPos);
    while(stack.size() != 0){
        int u = stack.top();
        stack.pop();
        if(visited[u] == false){
            visited[u] = true;
            for(int i = adjList[u].size() - 1; i >= 0; i--){
                if(visited[adjList[u][i]] == false){
                    stack.push(adjList[u][i]);
                }
            }
        }
    }
}

void Grid::bfs(){   
    int listSize = length*length;
    std::queue<int> queue;
    queue.push(startPos);
    std::vector<bool> visited(listSize, false);
    std::vector<int> previous(listSize);
    visited[startPos] = true;

    bool found = false;
    
    while (!queue.empty() && found == false) {
        int u = queue.front();
        queue.pop();
        for (int i = 0; i < adjList[u].size(); ++i) {
            int v = adjList[u][i];
            if(visited[v] == false){
                visited[v] = true;
                previous[v] = u;
                queue.push(v);
                if(v == goalPos){
                    found = true;
                }
            }
        }
    }
    showSinglePath(previous);
}

void Grid::newBfs() {
    int size = length * length;
    std::queue<int> queue;
    queue.push(startPos);
    std::vector<bool> visited(size, false);
    
    while(!queue.empty()){
        int u = queue.front();
        queue.pop();
        std::vector<int> current_neighbors;
        for(int i = 0; i < adjList[u].size(); i++){
            int v = adjList[u][i];
            if(visited[v] == false){
                visited[v] = true;
                queue.push(v);
                current_neighbors.push_back(v);
            }   
        }
        this->total_algorithm_steps.push_back(current_neighbors);
    }
}

void simulateAlgorithm(std::vector<std::vector<int>>& all_paths){
    
}

void Grid::showSinglePath(std::vector<int>& path){
    std::stack<int> stack;
    int current = path[goalPos];
    std::vector<int> currentPath;
    while(current != startPos){
        stack.push(current);
        current = path[current];
    }
    int index = 0;
    while(!stack.empty()){
        int v = stack.top();
        currentPath.push_back(v);
        pathSteps[index] = currentPath;
        index++;
        if(activePathing)
            tiles[v].tile.setFillColor(sf::Color::Green);
        stack.pop();
    }
}


void Grid::showPath(){
    for(int i = 0; i < step; i++){
        for(int k = 0; k < pathSteps[i].size(); k++){
            tiles[pathSteps[i][k]].tile.setFillColor(sf::Color::Green); 
        }
    }
}

void Grid::incrementStep(sf::Event& event){
    if(event.key.code == sf::Keyboard::Right){
        step++;
    }
    else if(event.key.code == sf::Keyboard::Left){
        step--;
    }
}
