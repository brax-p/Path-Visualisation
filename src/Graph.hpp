#pragma once
#include "Node.hpp"

struct Edge
{
    std::pair<Node&, Node&> nodes;
    sf::Vertex line[2];
    Edge(Node& node1, Node& node2) : nodes(node1, node2) {}
    void update()
    {
        std::cout << nodes.first.getNodePosition().x << "," << nodes.first.getNodePosition().y << "\n";
        std::cout << nodes.second.getNodePosition().x << "," << nodes.second.getNodePosition().y << "\n";
        
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
    public:
        enum class Algos { NONE };
        enum class Editing { NONE, ADD_NODE, REMOVE_NODE, ADD_EDGE, REMOVE_EDGE, MOVE_NODE};
        Graph(AppState& p_appState, sf::Vector2f displayAreaPosition, sf::Vector2f displayAreaSize);
        void draw(sf::RenderWindow& window);
        void update(sf::Vector2i p_mousePosition);
        void handleLeftClick(int x, int y);
        void handleLeftReleased();
        bool coordsInsideContainer(sf::Vector2i coords, float buffer);
        bool isVisible;
        Editing currentEditState;
        float nodeRadius;
    private:
        std::vector<Node> m_Nodes;
        std::vector<Edge> m_Edges;
        AppState m_appState;
        sf::Vector2f m_ContainerPosition;
        sf::Vector2f m_ContainerSize;
        sf::RectangleShape m_Container;
        

};

Graph::Graph(AppState& p_appState, sf::Vector2f displayAreaPosition, sf::Vector2f displayAreaSize) :
    m_appState(p_appState)
{
    currentEditState = Editing::MOVE_NODE;
    isVisible = false;

    m_ContainerPosition = displayAreaPosition;
    m_ContainerSize = displayAreaSize;

    // Create Nodes
    sf::Vector2f firstPos = m_ContainerPosition;
    nodeRadius = 25.0f;
    firstPos.x+=nodeRadius;
    firstPos.y+=nodeRadius;
    
    Node first(firstPos, nodeRadius);
    m_Nodes.push_back(std::move(first));

    sf::Vector2f secondPos = firstPos;
    secondPos.x+=200;
    secondPos.y+=200;
    Node second(secondPos, nodeRadius);
    m_Nodes.push_back(std::move(second));

    Edge first_second(m_Nodes[0], m_Nodes[1]);
    m_Edges.push_back(std::move(first_second));

    m_Container.setPosition(m_ContainerPosition);
    m_Container.setSize(m_ContainerSize);
    m_Container.setOutlineThickness(2);
    m_Container.setOutlineColor(sf::Color::Blue);
    m_Container.setFillColor(sf::Color::Transparent);
}

void Graph::draw(sf::RenderWindow& window)
{
    if(!isVisible) return;

    for(auto& node : m_Nodes)
        node.draw(window);

    for(auto& edge : m_Edges)
        edge.draw(window);

    window.draw(m_Container);
}

void Graph::update(sf::Vector2i p_mousePosition)
{
    for(auto& edge : m_Edges)
        edge.update();

    if(currentEditState != Editing::MOVE_NODE) return;

    for(auto& node : m_Nodes)
    {
        if(node.isClicked)
        {
            if(coordsInsideContainer(p_mousePosition, node.getNodeRadius()))
                node.setNodePosition(sf::Vector2f(p_mousePosition.x, p_mousePosition.y));
        }
    }

}

void Graph::handleLeftClick(int x, int y)
{
    switch(currentEditState)
    {
        case Editing::MOVE_NODE:
            {
                for(auto& node : m_Nodes)
                    if(node.isIntersecting(x, y))
                        node.isClicked = true;
                break;
            }
        case Editing::ADD_NODE:
            {
                if(coordsInsideContainer(sf::Vector2i(x,y), 0))
                {
                    Node newNode(sf::Vector2f(x,y), nodeRadius);
                    //m_Nodes.push_back(std::move(newNode));
                }
                break;
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

bool Graph::coordsInsideContainer(sf::Vector2i coords, float buffer)
{
    if(coords.x > m_ContainerPosition.x+buffer && coords.x < m_ContainerPosition.x + m_ContainerSize.x-buffer)
        if(coords.y > m_ContainerPosition.y+buffer && coords.y < m_ContainerPosition.y + m_ContainerSize.y-buffer)
            return true;
    return false;
}
