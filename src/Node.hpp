#pragma once

class Node 
{
    public:
        Node(sf::Vector2f p_Position, float p_Radius, std::string p_ID);
        void draw(sf::RenderWindow& window);
        bool isIntersecting(int x, int y);

    private:
        //std::vector<std::unique_ptr<Node>> m_Neighbors;
        sf::CircleShape m_Node;
        std::string m_ID;
};

Node::Node(sf::Vector2f p_Position, float p_Radius, std::string p_ID="default")
{
    m_Node.setPosition(p_Position);
    m_Node.setRadius(p_Radius);
    m_Node.setOutlineThickness(2);
    m_Node.setOutlineColor(sf::Color::Black);
    m_Node.setFillColor(sf::Color::White);

    m_ID = p_ID;
}

void Node::draw(sf::RenderWindow& window)
{
    window.draw(m_Node);
}

bool Node::isIntersecting(int x, int y)
{
    //circle intersection

    float deltaX = x - m_Node.getPosition().x;
    float deltaY = y - m_Node.getPosition().y;

    if(((deltaX * deltaX) + (deltaY * deltaY)) <= (m_Node.getRadius() * m_Node.getRadius()))
        return true;
    return false;
}
