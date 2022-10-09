#pragma once
#include "AppState.hpp"
#include "Tile.hpp"
#include <vector>
#include <stack>
#include <queue>
#include <limits>
#include <memory>

enum class Algos {None, DFS, BFS};
enum class Editing {None, Spawn, Wall, Goal};

class Grid
{
    public:
        Grid(int p_gridWidth, AppState& p_appState, sf::Vector2f displayAreaOrigin, sf::Vector2f displayAreaSize);
        void draw(sf::RenderWindow &window);
        void removeVertex(int p_vertex);
        void addVertex(int p_vertex);
        void update();
        void executeAlgorithm();
        bool handleLeftClick(int x, int y);
        void handleRightClick(int x, int y);
        void printAdjList();
        void clearGrid();
        void resetSimulation();
        void setAppState(OpState new_state);
        bool isIntersecting(int x, int y);
        Tile* getIntersectedTile(int x, int y);
        void createAdjList();
        void updateGridValues();
        void setGridWidth(int p_newGridWidth);
        int  getGridWidth();
        void Simulate();
        void resetAndSimulate();
        void clearGridOfWalls();
        void playOrPauseSimulate();
        void decreaseGridSize();
        void increaseGridSize();
        void decreaseGridSpawnChanger();
        void setAlgorithm(Algos newAlgo);
        void handleKeyPress(const sf::Event& event);
        void changeAlgo(Algos newAlgo);
        std::string slowdown();
        std::string speedup();
        std::string getStepString();

        std::pair<std::vector<int>, std::vector<int>> bfs();
        std::pair<std::vector<int>, std::vector<int>> dfs();
        int removedVertices = 0;
        Editing editState;      
        bool isVisible;
        int direction = 1;
        int speed = 1;
        
    private:
        Algos m_currentAlgo;

        AppState& m_appState;

        float m_tileWidth;
        int m_gridWidth;

        int m_prevStartPosition;
        int m_prevGoalPosition;
        int m_startPosition;
        int m_goalPosition;
        std::vector<int> m_goalPositions; // allow for multiple goal compatibility eventuallt

        bool m_wallsHaveChanged;
        bool m_justSimulated;

        sf::Vector2f m_Origin;
        sf::Vector2f m_Container;

        std::vector<std::vector<int>> m_AdjList;
        std::vector<std::vector<int>> m_defaultAdjList; // keep a copy so no need to re-compute base adj list
        std::vector<Tile> m_Tiles;

        std::vector<int> m_algo_result_path;    //Single path
        std::vector<int> m_algo_result_total;   //Full exploration tree


        int m_SimulationIteration;
        int m_SimulationIteration_factor;
        int m_SimulationIteration_limit;
        int m_SimulationIndex;

        int m_gridWidthLowerBound;
        int m_gridWidthUpperBound;
};

Grid::Grid(int p_gridWidth, AppState& p_appState, sf::Vector2f displayAreaOrigin, sf::Vector2f displayAreaSize) : 
    m_appState(p_appState)
{
    isVisible = false;
    editState = Editing::Wall;

    m_gridWidthLowerBound = 3;
    m_gridWidthUpperBound = 36;

    m_wallsHaveChanged = true; // done to trigger first iteration of OpState::ShowPath 
    m_justSimulated = true;

    m_currentAlgo = Algos::BFS;

    m_gridWidth = p_gridWidth;

    m_goalPosition = m_gridWidth * m_gridWidth - 1;
    m_prevGoalPosition = m_goalPosition;

    m_startPosition = 0;
    m_prevStartPosition = m_startPosition;
    
    m_Container = displayAreaSize;
    m_tileWidth = displayAreaSize.x / p_gridWidth; // x or y doesnt matter since grid is a square

    m_Origin.x = displayAreaOrigin.x;
    m_Origin.y = displayAreaOrigin.y;

    m_SimulationIndex = 0;
    m_SimulationIteration = 0;
    m_SimulationIteration_factor = 50;
    m_SimulationIteration_limit = 500;
        
    createAdjList();
    m_defaultAdjList = m_AdjList;
    std::cout << '\n';
}

void Grid::draw(sf::RenderWindow &window)
{   
    if(!isVisible) return;

    for(auto &tile: m_Tiles)
    {
        tile.draw(window);
    }
}

void Grid::removeVertex(int p_vertex)
{
    auto removeItself = [&](int neighborVertex)
    {
        for(int i = 0; i < m_AdjList[neighborVertex].size(); i++)
        {
            if(m_AdjList[neighborVertex][i] == p_vertex)
            {
                std::swap(m_AdjList[neighborVertex][i], m_AdjList[neighborVertex][m_AdjList[neighborVertex].size() - 1]);
                m_AdjList[neighborVertex].pop_back();
            }
        }
        return;
    };

    if(p_vertex / m_gridWidth > 0)                  removeItself(p_vertex - m_gridWidth);
    if(p_vertex / m_gridWidth < m_gridWidth - 1)    removeItself(p_vertex + m_gridWidth);
    if(p_vertex % m_gridWidth > 0)                  removeItself(p_vertex - 1);
    if(p_vertex % m_gridWidth < m_gridWidth - 1)    removeItself(p_vertex + 1);

}

void Grid::addVertex(int p_vertex)
{
    //get left, right, up, and down.
    //then add current vertex to each valid space -- that means add to any tile that isnt a wall
    //it is guaranteed that walls have no neighbors and no non-walls have walls as a neighbor

    std::vector<int> neighbors;

    int row = p_vertex / m_gridWidth;
    int col = p_vertex % m_gridWidth;

    if(col > 0)                neighbors.push_back(p_vertex - 1); // left neighbor exists
    if(col < m_gridWidth - 1) neighbors.push_back(p_vertex + 1); //right neighbor exists
    if(row > 0)                neighbors.push_back(p_vertex - m_gridWidth); // up neighbor exists
    if(row < m_gridWidth - 1) neighbors.push_back(p_vertex + m_gridWidth); // down neighbor exists

    //from current possible neighbors, check if can add to each neighbor then add
    for(int i = 0; i < neighbors.size(); ++i)
    {
        //If the tile that maps to the current neighbor isnt a wall, then add. Else, do nothing.
        //This ensures that no wall ever gets added to the adjacency list. This allows for simple approach
        //in remove_vertex().
        if(m_Tiles[neighbors[i]].tileType != Type::Wall)
        {
            //add neighbor to current vertex, then add current vertex to neighbor in adj list

            m_AdjList[p_vertex].push_back(neighbors[i]);

            m_AdjList[neighbors[i]].push_back(p_vertex);
        }
    }
}

void Grid::update()
{
    switch(m_appState.current_state)
    {   
        case OpState::Idle:
        {
            break;
        }

        case OpState::ShowPath:
        {
            //this is easier than onclick functions. For Edit and Simulate however, onclicks are fine
            if((m_prevStartPosition != m_startPosition) || (m_prevGoalPosition != m_goalPosition) || (m_wallsHaveChanged) || (m_justSimulated))
            {
                //With the new configurtation, get the new result of the path searching algorithm
                executeAlgorithm();

                // need to update each previous tile that was part of path to a type of blank.
                // "reset" the configuration to prep for path assignment
                for(auto &tile : m_Tiles)
                {
                    tile.part_of_path = false;
                }

                //then get single path
                int current = m_algo_result_path[m_goalPosition];
                while(current != m_startPosition)
                {   
                    m_Tiles[current].part_of_path = true;
                    current = m_algo_result_path[current];
                }

                m_wallsHaveChanged = false;
                m_prevGoalPosition = m_goalPosition;
                m_prevStartPosition = m_startPosition;
                m_justSimulated = false;

            }
            else
            {
                // no need to update. Configuration hasnt changed
            }
            break;
        }
        
        case OpState::Edit:
        {
            break;
        }

        case OpState::Simulate:
        {   
            if(direction == 1)
            {
                if(m_SimulationIndex < m_algo_result_total.size()) // Dont include discovery of goal node
                {
                    int tile_index = m_algo_result_total[m_SimulationIndex];
                    m_Tiles[tile_index].part_of_path = true;
                    m_SimulationIteration += (speed*m_SimulationIteration_factor);
                    if(m_SimulationIteration > m_SimulationIteration_limit)
                    {
                        ++m_SimulationIndex;
                        m_SimulationIteration = 0;
                    }
                }
                else
                {
                    // simulation has finished
                    m_appState.current_state = OpState::Idle;
                }
            }
            break;
        }
    }
    for(auto &tile : m_Tiles)
    {
        tile.update();
    }
}

void Grid::executeAlgorithm()
{
    // Use of switch over terniary
    switch(m_currentAlgo)
    {
        case Algos::BFS:
        {   
            std::pair<std::vector<int>, std::vector<int>> bfs_output = bfs();
            m_algo_result_total = bfs_output.first;
            m_algo_result_path = bfs_output.second;
            break;
        }

        case Algos::DFS:
        {
            std::pair<std::vector<int>, std::vector<int>> dfsOutput = dfs();
            m_algo_result_total = dfsOutput.first;
            m_algo_result_path = dfsOutput.second;
            break;
        }

    }
}

std::pair<std::vector<int>, std::vector<int>> Grid::bfs()
{
    int size = m_gridWidth * m_gridWidth;
    std::queue<int> queue;
    queue.push(m_startPosition);
    std::vector<bool> visited(size, false);
    visited[m_startPosition] = true;
    std::vector<int> previous(size);            // contains at each index, the vertex it was "discovered" from. Allows for single path representation
    bool found = false;
    std::vector<int> result_path;
    while(!queue.empty() && !found)
    {
        int u = queue.front();
        queue.pop();
        for(int i = 0; i < m_AdjList[u].size(); ++i)
        {
            int v = m_AdjList[u][i];
            if(!visited[v])
            {
                visited[v] = true;
                previous[v] = u;   //the current vertex that is a neighbor of v, now has v as its parent -- or source -- node
                queue.push(v);
                if(v == m_goalPosition)
                {
                    found = true;
                    break;
                }
                result_path.push_back(v);
            }
        }
    }
    std::pair<std::vector<int>, std::vector<int>> pair_result(result_path, previous);
    return pair_result;
}

std::pair<std::vector<int>, std::vector<int>> Grid::dfs()
{
    int size = m_gridWidth * m_gridWidth;
    std::stack<int> stk;
    std::vector<bool> visited(size, false);
    stk.push(m_startPosition);
    std::vector<int> previous(size);
    bool found = false;
    std::vector<int> resultPath;
    int idx = 0;
    while(!stk.empty() && !found)
    {
        int u = stk.top();
        stk.pop();
        if(!visited[u])
        {
            visited[u] = true;
            if(u == m_goalPosition)
            {
                found = true;
                break;
            }
            else if(idx > 0)
            {
                resultPath.push_back(u);
            }
            for(int i = 0; i < m_AdjList[u].size(); i++)
            {
                int v = m_AdjList[u][i];
                previous[v] = u;
                stk.push(v);
            }
        }
        idx++;
    }
    std::pair<std::vector<int>, std::vector<int>> pairResult(resultPath, previous);
    return pairResult;
}

bool Grid::handleLeftClick(int x, int y)
{
    // return true if interacting with the grid. 
    // return false if not. This helps not computing other elements in the model like gui buttons since
    // only 1 object can be clicked/interacted with at any time. -- by design

    if((!isVisible) || (m_appState.current_state != OpState::Edit))
        return false;
    Tile* selectedTile = getIntersectedTile(x, y); // no need to iterator over every tile in m_Tiles -> simply calculate which tile would be clicked given some coords, if any.
    if(selectedTile != nullptr && (selectedTile->tileType == Type::Blank || selectedTile->tileType == Type::Path))
    {   

        clearGrid();
        resetSimulation();
        setAppState(OpState::Edit);

        switch(editState)
        {
            case Editing::Wall:
            {
                removeVertex(selectedTile->vertex);
                removedVertices++;
                selectedTile->set_type(Type::Wall);
                m_wallsHaveChanged = true;
                break;
            }
            case Editing::Spawn:
            {
                addVertex(m_startPosition);
                m_Tiles[m_startPosition].tileType = Type::Blank;
                removeVertex(selectedTile->vertex);
                m_startPosition = selectedTile->vertex;
                selectedTile->tileType = Type::Spawn;
                break;
            }

            case Editing::Goal:
            {   
                //addVertex(m_goalPosition); // not needed since valid tiles are only path / blank so already a part of adj list??
                m_Tiles[m_goalPosition].tileType = Type::Blank;
                m_goalPosition = selectedTile->vertex;
                selectedTile->tileType = Type::Goal;
                break;
            }
        }
        return true;
    }
    return false;
}

void Grid::handleRightClick(int mouseX, int mouseY)
{
    Tile* selectedTile = getIntersectedTile(mouseX, mouseY);
    if(selectedTile != nullptr)
    {
        clearGrid();
        resetSimulation();
        setAppState(OpState::Edit);
        if(m_appState.current_state == OpState::Edit)
        {
        
            if(selectedTile->tileType == Type::Wall)   
            {
                addVertex(selectedTile->vertex);
                removedVertices--;
                selectedTile->set_type(Type::Blank);
            }
        }
    }
}

void Grid::printAdjList()
{
    for(int i = 0; i < m_AdjList.size(); ++i)
    {
        std::cout << i << ": ";
        for(int j = 0; j < m_AdjList[i].size(); ++j)
        {
            std::cout << m_AdjList[i][j] << " ";
        }
        std::cout << "\n";
    }
}

void Grid::setAppState(OpState new_state)
{
    m_appState.current_state = new_state;
}

void Grid::clearGrid()
{
    for(auto &tile : m_Tiles)
    {
        if(tile.part_of_path)
        {
            tile.tileType = Type::Blank;
            tile.part_of_path = false;
        }
    }
}

void Grid::resetSimulation()
{
    clearGrid();
    m_SimulationIndex = 0;
    m_SimulationIteration = 0;
}

void Grid::playOrPauseSimulate()
{
    if(m_SimulationIndex == 0)
    {
        Simulate();
    }
    if(m_appState.current_state == OpState::Simulate)
        m_appState = OpState::Idle;
    else if(m_appState.current_state == OpState::Idle || m_appState.current_state == OpState::Edit)
        m_appState = OpState::Simulate;
}

void Grid::Simulate()
{
    setAppState(OpState::Simulate);
    clearGrid();
    executeAlgorithm();
    resetSimulation();
}

void Grid::resetAndSimulate()
{
    clearGrid();
    executeAlgorithm();
    setAppState(OpState::Simulate);
    resetSimulation();
}

bool Grid::isIntersecting(int x, int y)
{

    float sizeX = m_gridWidth * m_tileWidth;
    float sizeY = m_gridWidth * m_tileWidth;
    if(x > m_Origin.x && x < m_Origin.x + sizeX)
    {
        if(y > m_Origin.y && y < m_Origin.y + sizeY)
        {
            return true;
        }
    }

    return false;
}

Tile* Grid::getIntersectedTile(int x, int y)
{

    float size = m_gridWidth * m_tileWidth;
    Tile* result;
    if(isIntersecting(x, y))
    {
        sf::Vector2f adjustedCoords(x - m_Origin.x, y - m_Origin.y);
        adjustedCoords.x /= m_tileWidth;
        adjustedCoords.y /= m_tileWidth;
        int col = adjustedCoords.x;
        int row = adjustedCoords.y;
        int idx = row * m_gridWidth + col;
        result = &m_Tiles[idx];
    }
    else
    {
        result = nullptr;
    }
    return result;
}

void Grid::createAdjList()
{
    m_AdjList = {};
    m_Tiles = {};
    //Create the adjacency list representation and the list of Tiles that map to each vertex in the adjacency list
    for(int i = 0; i < m_gridWidth; ++i)
    {
        for(int j = 0; j < m_gridWidth; ++j)
        {
            int currentVertex = i * m_gridWidth + j;
            sf::Vector2f tilePosition(static_cast<float>(j*m_tileWidth+m_Origin.x), static_cast<float>(i*m_tileWidth+m_Origin.y));
            Type tileType;
            if(currentVertex == m_startPosition)
            {
                tileType = Type::Spawn;
            }
            else if(currentVertex == m_goalPosition)
            {
                tileType = Type::Goal;
            }
            else
            {
                tileType = Type::Blank;
            }
            Tile temp(currentVertex, m_tileWidth, tilePosition, tileType);
            m_Tiles.push_back(std::move(temp));

            int right   = i * m_gridWidth + j + 1;
            int left    = i * m_gridWidth + j - 1;
            int up      = (i - 1) * m_gridWidth + j;
            int down    = (i + 1) * m_gridWidth + j;

            std::vector<int> currentList;

            if(j > 0)               currentList.push_back(left);
            if(j < m_gridWidth - 1) currentList.push_back(right);

            if(i > 0)               currentList.push_back(up);
            if(i < m_gridWidth - 1) currentList.push_back(down);

            m_AdjList.push_back(std::move(currentList));
        }
    }
}
void Grid::updateGridValues()
{
    m_tileWidth = m_Container.x / m_gridWidth;
    m_goalPosition = m_gridWidth * m_gridWidth - 1;
    createAdjList();
}

void Grid::setGridWidth(int p_newGridWidth)
{
    m_gridWidth = p_newGridWidth;
}

int Grid::getGridWidth()
{
    return m_gridWidth;
}

void Grid::clearGridOfWalls()
{
    for(auto &tile : m_Tiles)
    {
        if(tile.tileType == Type::Wall)
        {
            addVertex(tile.vertex);
            tile.tileType = Type::Blank;
        }
    }
}

void Grid::decreaseGridSpawnChanger()
{
    addVertex(m_startPosition);
    m_Tiles[m_startPosition].tileType = Type::Blank;
    removeVertex(0);
    m_startPosition = 0;
    m_Tiles[0].tileType = Type::Spawn;
}

void Grid::setAlgorithm(Algos newAlgo)
{
    m_currentAlgo = newAlgo;
}

void Grid::handleKeyPress(const sf::Event& event)
{

}

void Grid::decreaseGridSize()
{
    int currentSize = m_gridWidth;
    if(currentSize > m_gridWidthLowerBound)
    {
        clearGridOfWalls();
        decreaseGridSpawnChanger();
        m_gridWidth = currentSize - 1;
        updateGridValues();
    }
}

void Grid::increaseGridSize()
{
    int currentSize = m_gridWidth;
    if(currentSize < m_gridWidthUpperBound)
    {
        clearGridOfWalls();
        m_gridWidth = currentSize + 1;
        updateGridValues();
    }
}

std::string Grid::slowdown()
{
    std::string result = "";
    if(speed > 1)
        result = std::to_string(--speed);
    else
        result = std::to_string(speed);
    result += "x";
    return result;
}

std::string Grid::speedup()
{
    std::string result = "";
    if(speed < 8)
        result = std::to_string(++speed);
    else
        result = std::to_string(speed);
    result += "x";
    return result;
}

void Grid::changeAlgo(Algos newAlgo)
{
    m_currentAlgo = newAlgo;
    executeAlgorithm();
}

std::string Grid::getStepString()
{
    return "Step " + std::to_string(m_SimulationIndex+1);
}
