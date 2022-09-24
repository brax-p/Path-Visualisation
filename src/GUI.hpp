#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

enum class ButtonType {};

struct Button {
    Button(sf::Font& font, std::string button_text="Default Text", sf::Vector2f element_position = sf::Vector2f(0.0f,0.0f), bool isVisible=true) : font(font) 
    {
        is_visible = isVisible;
        this->button_text = button_text;
        text.setFont(this->font);
        text.setString(this->button_text);
        text.setCharacterSize(button_text_char_size);
        text.setFillColor(text_color);
        text.setStyle(sf::Text::Bold);
        text.setPosition(element_position);
        
        sf::FloatRect bounds = text.getGlobalBounds();
        element_position.x = bounds.left;
        element_position.y = bounds.top;
        element_size = sf::Vector2f(bounds.width, bounds.height);
        element.setSize(element_size);
        element.setPosition(element_position);
        element.setOutlineColor(outline_color);
        element.setOutlineThickness(element_outline_thickness);
        element.setFillColor(sf::Color::Transparent);
    }

    sf::Text text;
    sf::Font& font;
    sf::RectangleShape element;
    sf::Vector2f element_position;
    sf::Vector2f element_size;
    sf::Color text_color = sf::Color::Black;
    sf::Color outline_color = sf::Color::Blue;
    sf::Color clicked_color = sf::Color::Yellow;
    sf::Color hover_color = sf::Color::Green;
    float element_outline_thickness = 2.0;
    int button_text_char_size = 24; //24 is default value
    std::string button_text;
    bool clicked = false;
    bool is_visible = false;

    void draw(sf::RenderWindow& window) 
    { 
        if(is_visible){
            window.draw(element);
            window.draw(text);
        }
    }

    void update(int mouseX, int mouseY)
    {
        int x = element.getPosition().x, y = element.getPosition().y;
        int size_x = element.getSize().x, size_y = element.getSize().y;
        bool flag = false;
        if(mouseX > x && mouseX < x + size_x){
            if(mouseY > y && mouseY < y + size_y){
                flag = true;
            }
        }
        sf::Color result;
        if(clicked == false){
            if(!flag)
                result = sf::Color::Transparent;
            else
                result = hover_color;
            element.setFillColor(result);
        }
        else if(clicked == true){
            element.setFillColor(clicked_color);
        }
    }
};

class GUI 
{
    public:
        GUI();
        sf::Font font;
        std::vector<Button> buttons;
        sf::RectangleShape container;
        void draw(sf::RenderWindow& window);
};

GUI::GUI()
{

    if(!font.loadFromFile("./fonts/Akshar.ttf")){
        std::cout << "Error loading font\n";
    }
    else{
        std::cout << "Loaded font successfully!\n";
    }

    float x_distance = 50.f; //distance between two buttons horizontally
    float y_distance = 50.0f; //distance betwee two buttons vertically

    container.setSize(sf::Vector2f(400, 750));
    container.setPosition(sf::Vector2f(825, 25));
    container.setFillColor(sf::Color::White);
    container.setOutlineColor(sf::Color::Red);
    container.setOutlineThickness(2);

    Button activate(font, "Start Simulation", sf::Vector2f(container.getPosition().x + 25, container.getPosition().y + 25), true);
    buttons.push_back(std::move(activate));
}

void GUI::draw(sf::RenderWindow& window)
{
    window.draw(container);
    for(auto& button : buttons)
        button.draw(window);
}
