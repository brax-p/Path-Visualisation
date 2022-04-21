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
};

Button::Button(sf::Vector2f position, sf::Vector2f size, sf::Color color, std::string text){
    button.setSize(size);
    button.setPosition(position);
    button.setFillColor(color);
    
    if(!font.loadFromFile("./fonts/Akshar.ttf")){
        std::cout << "error, cannot load font" << "\n";
    }
    else{
        std::cout << "succesful loading of the font" << "\n";
    }
    int numChars = text.size();
    int charSize = size.x / numChars;
    sf::Vector2f textPos = position;
    textPos.x+=size.x/32;
    textPos.y+=size.y/4;

    //TODO:
    //need to fix text size for general cases with buttons;

    value.setFont(font);
    value.setString(text);
    value.setFillColor(sf::Color::White);
    value.setStyle(sf::Text::Bold | sf::Text::Underlined);
    value.setPosition(textPos);
    value.setScale(2,1.5);
}

void Button::draw(sf::RenderWindow &window){
    window.draw(this->button);
    window.draw(this->value);
}
