#include "Tile.hpp"
#include <vector>
class Grid {
    public:
        Grid(int grid_length);
        std::vector<Tile> tiles;
        std::vector<bool> adjMatrix;
    private:
        int length;
};

Grid::Grid(int grid_length){
    this->length = grid_length;
    for(int i = 0; i < length; i++){
        for(int k = 0; k < length; k++){

        }
    }
}
