#pragma once
#include "Node.hpp"

class Graph
{
    public:
        Graph(AppState& p_appState, sf::Vector2f displayAreaPosition, sf::Vector2f displayAreaSize);
        void draw(sf::RenderWindow& window);
        void handleLeftClick(int x, int y);
        bool isVisible;
    private:
        std::vector<Node> m_Nodes;
        AppState m_appState;
        sf::Vector2f m_ContainerPosition;
        sf::Vector2f m_ContainerSize;
};

Graph::Graph(AppState& p_appState, sf::Vector2f displayAreaPosition, sf::Vector2f displayAreaSize) :
    m_appState(p_appState)
{
    m_ContainerPosition = displayAreaPosition;
    m_ContainerSize = displayAreaSize;
    isVisible = false;


    // Create Nodes
    
    Node first(m_ContainerPosition, 50.0f);
    m_Nodes.push_back(std::move(first));

}

void Graph::draw(sf::RenderWindow& window)
{
    if(!isVisible) return;

    for(auto& node : m_Nodes)
        node.draw(window);
}

void Graph::handleLeftClick(int x, int y)
{
    for(auto& node : m_Nodes)
    {
        if(node.isIntersecting(x, y))
        {
            std::cout << "clicked!\n";
        }
    }
}
