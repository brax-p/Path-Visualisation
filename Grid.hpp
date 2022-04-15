#include "Tile.hpp"
#include <vector>
#include <iostream>
#include <stack>
#include <queue>
#include <limits>

const int INF = std::numeric_limits<int>::max();
class Grid {
    public:
        Grid(int grid_length);
        void draw(sf::RenderWindow &window);
        void update();
        std::vector<Tile> tiles;
        std::vector<std::vector<int>> adjList;
        void dfs();
        void bfs();
        void printVector(std::vector<int>& vec);
        void listPath(std::vector<int>& path);
        int tileLength = 50;
    private:
        int length;
        sf::Vector2f leftCornerPosition;
        int startPos = 0;
        int goalPos = 15;
};

void Grid::printVector(std::vector<int>& vec){
    for(auto v: vec){
        std::cout << v << " ";
    }
    std::cout << std::endl;
}

Grid::Grid(int grid_length){
    this->length = grid_length;
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
    /*for(int i = 0; i < adjList.size(); i++){
        std::cout << i << ": ";
        for(int k = 0; k < adjList[i].size(); k++){
            std::cout <<  adjList[i][k] << " ";
        }
        std::cout << "\n";
    }*/
}

void Grid::draw(sf::RenderWindow &window){
    for(auto k: tiles)
        window.draw(k.tile);
}

void Grid::update(){

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
            if(u == goalPos){
                //  path to goal node exists
                return;

            }
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
        std::cout << u << " ";
        queue.pop();
        for (int i = 0; i < adjList[u].size(); ++i) {
            int v = adjList[u][i]; // v is a neighbor of u
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
    std::cout << "\n";
    listPath(previous);
}

void Grid::listPath(std::vector<int>& path){
    std::stack<int> stack;
    stack.push(goalPos);
    int current = path[goalPos];
    while(current != startPos){
        stack.push(current);
        current = path[current];
    }
    stack.push(startPos);
    while(!stack.empty()){
        int v = stack.top();
        tiles[v].tile.setFillColor(sf::Color::Green);
        std::cout << v << " ";
        stack.pop();
    }
    std::cout << "\n";
}

