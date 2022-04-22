#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

class Button {
    public:
        Button(sf::Vector2f position, sf::Vector2f size, sf::Color color, std::string text);
        sf::RectangleShape button;
        sf::Font font;
        sf::Text value;
        bool state = 0;

        void draw(sf::RenderWindow &window);
        void update();
};

Button::Button(sf::Vector2f position, sf::Vector2f size, sf::Color color, std::string text){
    
    if(!font.loadFromFile("./fonts/Akshar.ttf")){
        std::cout << "error, cannot load font" << "\n";
    }
    else{
        std::cout << "succesful loading of the font" << "\n";
    }

    int buffer = 50;

    value.setFont(font);
    value.setString(text);
    value.setFillColor(sf::Color::White);
    value.setStyle(sf::Text::Bold | sf::Text::Underlined);
    value.setPosition(position);

    sf::FloatRect bounds = value.getGlobalBounds();

    sf::Vector2f buttonPos(bounds.left-(buffer/2), bounds.top-(buffer/2));
    sf::Vector2f buttonSize(bounds.width+buffer, bounds.height+buffer+4);

    button.setSize(buttonSize);
    button.setPosition(buttonPos);
    button.setFillColor(sf::Color::Transparent);
    button.setOutlineColor(sf::Color::Green);
    button.setOutlineThickness(2);
}

void Button::draw(sf::RenderWindow &window){
    window.draw(this->button);
    window.draw(this->value);
}

void Button::update(){
    
}
