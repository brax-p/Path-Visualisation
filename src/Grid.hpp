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
        Grid(int p_grid_width, AppState *p_app_state);
        void draw(sf::RenderWindow &window);
        void remove_vertex(int p_vertex);
        void add_vertex(int p_vertex);
        void update();
        void execute_algorithm();
        bool handle_leftclick(int x, int y);
        void handle_rightclick();
        void print_adj_list();
        void clear_grid();
        void reset_simulation();
        void set_app_state(OpState new_state);
        std::pair<std::vector<int>, std::vector<int>> bfs();
        std::vector<int> dfs();
        Editing edit_state;      
        
    private:
        Algos m_current_algo;

        AppState* m_app_state;

        float m_tile_length;
        int m_grid_width;

        int m_prev_start_position;
        int m_prev_goal_position;
        int m_start_position;
        int m_goal_position;
        std::vector<int> m_goal_positions; // allow for multiple goal compatibility

        bool m_walls_have_changed;
        bool m_just_simulated;

        sf::Vector2f m_origin;

        std::vector<std::vector<int>> m_adj_list;
        std::vector<std::vector<int>> m_default_adj_list; // keep a copy so no need to re-compute base adj list
        std::vector<Tile> m_tiles;

        std::vector<int> m_algo_result_path;    //Single path
        std::vector<int> m_algo_result_total;   //Full exploration tree


        int m_simulation_iteration;
        int m_simulation_iteration_factor;
        int m_simulation_iteration_limit;
        int m_simulation_index;


};

Grid::Grid(int p_grid_width, AppState *p_app_state) : m_app_state(p_app_state)
{

    edit_state = Editing::None;

    m_walls_have_changed = true; // done to trigger first iteration of OpState::ShowPath 
    m_just_simulated = true;

    m_current_algo = Algos::BFS;

    m_grid_width = p_grid_width;

    m_goal_position = m_grid_width * m_grid_width - 1;
    m_prev_goal_position = m_goal_position;

    m_start_position = 0;
    m_prev_start_position = m_start_position;

    m_tile_length = 50.f;

    m_origin.x = 25.f;
    m_origin.y = 25.f;

    m_simulation_index = 0;
    m_simulation_iteration = 0;
    m_simulation_iteration_factor = 50;
    m_simulation_iteration_limit = 500;

    //Create the adjacency list representation and the list of Tiles that map to each vertex in the adjacency list
    for(int i = 0; i < m_grid_width; ++i)
    {
        for(int j = 0; j < m_grid_width; ++j)
        {
            int current_vertex = i * m_grid_width + j;
            sf::Vector2f tile_position((float)j*m_tile_length+m_origin.x, (float)i*m_tile_length+m_origin.y);
            Type tile_type;
            if(current_vertex == m_start_position)
            {
                tile_type = Type::Spawn;
            }
            else if(current_vertex == m_goal_position)
            {
                tile_type = Type::Goal;
            }
            else
            {
                tile_type = Type::Blank;
            }
            Tile temp(current_vertex, m_tile_length, tile_position, tile_type);
            m_tiles.push_back(std::move(temp));

            int right = i * m_grid_width + j + 1;
            int left = i * m_grid_width + j - 1;
            int up = (i - 1) * m_grid_width + j;
            int down = (i + 1) * m_grid_width + j;

            std::vector<int> current_list;

            if(j > 0) current_list.push_back(left);
            if(j < m_grid_width - 1) current_list.push_back(right);

            if(i > 0) current_list.push_back(up);
            if(i < m_grid_width - 1) current_list.push_back(down);

            m_adj_list.push_back(std::move(current_list));
        }
    }
    m_default_adj_list = m_adj_list;
    std::cout << '\n';
}

void Grid::draw(sf::RenderWindow &window)
{   
    for(auto &tile: m_tiles)
    {
        tile.draw(window);
    }
}

void Grid::remove_vertex(int p_vertex)
{
   for(int i = 0; i < m_adj_list[p_vertex].size(); ++i)
   {
        int neighbor = m_adj_list[p_vertex][i];
        for(int j = 0; j < m_adj_list[neighbor].size(); ++j)
        {
            if(m_adj_list[neighbor][j] == p_vertex)
            {
                m_adj_list[neighbor].erase(m_adj_list[neighbor].begin() + j);
            }
        }
   }
}

void Grid::add_vertex(int p_vertex)
{
    //get left, right, up, and down.
    //then add current vertex to each valid space -- that means add to any tile that isnt a wall
    //it is guaranteed that walls have no neighbors and no non-walls have walls as a neighbor

    std::vector<int> neighbors;

    int row = p_vertex / m_grid_width;
    int col = p_vertex % m_grid_width;

    if(col > 0)                neighbors.push_back(p_vertex - 1); // left neighbor exists
    if(col < m_grid_width - 1) neighbors.push_back(p_vertex + 1); //right neighbor exists
    if(row > 0)                neighbors.push_back(p_vertex - m_grid_width); // up neighbor exists
    if(row < m_grid_width - 1) neighbors.push_back(p_vertex + m_grid_width); // down neighbor exists

    //from current possible neighbors, check if can add to each neighbor then add
    for(int i = 0; i < neighbors.size(); ++i)
    {
        //If the tile that maps to the current neighbor isnt a wall, then add. Else, do nothing.
        //This ensures that no wall ever gets added to the adjacency list. This allows for simple approach
        //in remove_vertex().
        if(m_tiles[neighbors[i]].tile_type != Type::Wall)
        {
            //add neighbor to current vertex, then add current vertex to neighbor in adj list

            m_adj_list[p_vertex].push_back(neighbors[i]);

            m_adj_list[neighbors[i]].push_back(p_vertex);
        }
    }
}

void Grid::update()
{
    //Onclicks are better for Edit and Simulate than for ShowPath.
    //This is due to a result of controller class actions might not resulting in a changing of configuration. But, sometimes it does.
    //So, instead of checking whether or not a change has occured (heavy overhead), result to the "naive" approach.

    //Edit and Simulate can use onclick because:
    //Edit doesn't display a path it only changes the configuration. Think of edit as ShowPath minus active path display
    //Simulate shows algorithmic results over time. The results are already known and the configuration cannot be changed.
    
    switch(m_app_state->current_state)
    {   
        case OpState::Idle:
        {
            break;
        }

        case OpState::ShowPath:
        {
            //this is easier than onclick functions. For Edit and Simulate however, onclicks are fine
            if((m_prev_start_position != m_start_position) || (m_prev_goal_position != m_goal_position) || (m_walls_have_changed) || (m_just_simulated))
            {
                //With the new configurtation, get the new result of the path searching algorithm
                execute_algorithm();

                // need to update each previous tile that was part of path to a type of blank.
                // "reset" the configuration to prep for path assignment
                for(auto &tile : m_tiles)
                {
                    tile.part_of_path = false;
                }

                //then get single path
                int current = m_algo_result_path[m_goal_position];
                while(current != m_start_position)
                {   
                    m_tiles[current].part_of_path = true;
                    current = m_algo_result_path[current];
                }

                m_walls_have_changed = false;
                m_prev_goal_position = m_goal_position;
                m_prev_start_position = m_start_position;
                m_just_simulated = false;

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
            if(m_simulation_index < m_algo_result_total.size()) // Dont include discovery of goal node
            {
                int tile_index = m_algo_result_total[m_simulation_index];
                m_tiles[tile_index].part_of_path = true;
                m_simulation_iteration += m_simulation_iteration_factor;
                if(m_simulation_iteration > m_simulation_iteration_limit)
                {
                    ++m_simulation_index;
                    m_simulation_iteration = 0;
                }
            }
            else
            {
                // simulation has finished
                m_simulation_index = 0;
                m_simulation_iteration = 0;
            }
            break;
        }
    }
    for(auto &tile : m_tiles)
    {
        tile.update();
    }
}

void Grid::execute_algorithm()
{
    // Use of switch over terniary
    switch(m_current_algo)
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
            m_algo_result_total = dfs();
            break;
        }

    }
}

std::pair<std::vector<int>, std::vector<int>> Grid::bfs()
{
    int size = m_grid_width * m_grid_width;
    std::queue<int> queue;
    queue.push(m_start_position);
    std::vector<bool> visited(size, false);
    visited[m_start_position] = true;
    std::vector<int> previous(size);            // contains at each index, the vertex it was "discovered" from. Allows for single path representation
    bool found = false;
    std::vector<int> result_path;
    while(!queue.empty() && !found)
    {
        int u = queue.front();
        queue.pop();
        for(int i = 0; i < m_adj_list[u].size(); ++i)
        {
            int v = m_adj_list[u][i];
            if(!visited[v])
            {
                visited[v] = true;
                previous[v] = u;   //the current vertex that is a neighbor of v, now has v as its parent -- or source -- node
                queue.push(v);
                if(v == m_goal_position)
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

std::vector<int> Grid::dfs()
{
    return {};
}

bool Grid::handle_leftclick(int x, int y)
{
    // return true if interacting with the grid. 
    // return false if not. This helps not computing other elements in the model like gui buttons since
    // only 1 object can be clicked/interacted with at any time.

    // if on a tile, execute action based on Operation State
    for(auto &tile : m_tiles)
    {   
        if(tile.is_intersecting(x,y) && ((tile.tile_type == Type::Blank || tile.tile_type == Type::Path))) //only allow changing to/from "valid" spaces
        {
            
            switch(edit_state)
            {
                case Editing::Wall:
                {
                    remove_vertex(tile.vertex);
                    tile.set_type(Type::Wall);
                    m_walls_have_changed = true;
                    break;
                }
                case Editing::Spawn:
                {
                    add_vertex(m_start_position);
                    m_tiles[m_start_position].tile_type = Type::Blank;
                    remove_vertex(tile.vertex);
                    m_start_position = tile.vertex;
                    tile.tile_type = Type::Spawn;
                    break;
                }

                case Editing::Goal:
                {   
                    add_vertex(m_goal_position);
                    m_tiles[m_goal_position].tile_type = Type::Blank;
                    m_goal_position = tile.vertex;
                    std::cout << "New Goal Pos: " << m_goal_position << '\n';
                    tile.tile_type = Type::Goal;
                    break;
                }
            }
            
            return true;
        }
    }
    return false;
}

void Grid::print_adj_list()
{
    for(int i = 0; i < m_adj_list.size(); ++i)
    {
        std::cout << i << ": ";
        for(int j = 0; j < m_adj_list[i].size(); ++j)
        {
            std::cout << m_adj_list[i][j] << " ";
        }
        std::cout << "\n";
    }
}

void Grid::set_app_state(OpState new_state)
{
    m_app_state->current_state = new_state;
}

void Grid::clear_grid()
{
    for(auto &tile : m_tiles)
    {
        if(tile.part_of_path)
        {
            tile.tile_type = Type::Blank;
            tile.part_of_path = false;
        }
    }
}

void Grid::reset_simulation()
{
    m_simulation_index = 0;
    m_simulation_iteration = 0;
}
