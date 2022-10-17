#pragma once
#include "Node.hpp"

struct Edge
{
    std::pair<Node&, Node&> nodes;
    sf::Vertex line[2];
    Edge(Node& node1, Node& node2) : nodes(node1, node2) {}
    void update()
    {
        /*
        std::cout << "First: " << nodes.first.getNodePosition().x << "," << nodes.first.getNodePosition().y << "\n";
        std::cout << "Second: " << nodes.second.getNodePosition().x << "," << nodes.second.getNodePosition().y << "\n";
        */
        line[0] = sf::Vertex(nodes.first.getNodePosition());
        line[1] = sf::Vertex(nodes.second.getNodePosition());

    }
    void draw(sf::RenderWindow& window)
    {
        window.draw(line, 2, sf::Lines);
    }
};


class Graph
{
    enum class Algos { NONE };
    enum class Editing { NONE };
    public:
        Graph(AppState& p_appState, sf::Vector2f displayAreaPosition, sf::Vector2f displayAreaSize);
        void draw(sf::RenderWindow& window);
        void update(sf::Vector2i p_mousePosition);
        void handleLeftClick(int x, int y);
        void handleLeftReleased();
        bool isVisible;
    private:
        std::vector<Node> m_Nodes;
        std::vector<Edge> m_Edges;
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
    sf::Vector2f firstPos = m_ContainerPosition;
    float nodeRadius = 25.0f;
    
    Node first(firstPos, nodeRadius);
    m_Nodes.push_back(std::move(first));

    sf::Vector2f secondPos = firstPos;
    secondPos.x+=200;
    secondPos.y+=200;
    Node second(secondPos, nodeRadius);
    m_Nodes.push_back(std::move(second));

    Edge first_second(m_Nodes[0], m_Nodes[1]);
    m_Edges.push_back(std::move(first_second));
}

void Graph::draw(sf::RenderWindow& window)
{
    if(!isVisible) return;

    for(auto& node : m_Nodes)
        node.draw(window);

    for(auto& edge : m_Edges)
        edge.draw(window);
}

void Graph::update(sf::Vector2i p_mousePosition)
{
    for(auto& node : m_Nodes)
    {
        if(node.isClicked)
        {
            node.setNodePosition(sf::Vector2f(p_mousePosition.x, p_mousePosition.y));
        }
    }

    for(auto& edge : m_Edges)
        edge.update();
}

void Graph::handleLeftClick(int x, int y)
{
    for(auto& node : m_Nodes)
    {
        if(node.isIntersecting(x, y))
        {
            node.isClicked = true;
        }
    }
}

void Graph::handleLeftReleased()
{
    for(auto& node : m_Nodes)
    {
        node.isClicked = false;
    }
}
