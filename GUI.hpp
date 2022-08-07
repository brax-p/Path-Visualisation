#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

enum class ButtonType {};

struct Button {
    Button(sf::Font& font, std::string button_text="Default Text", sf::Vector2f element_position = sf::Vector2f(0.0,0.0), bool isVisible=true) : font(font) {
        is_visible = isVisible;
        this->button_text = button_text;
        text.setFont(this->font);
        text.setString(this->button_text);
        text.setCharacterSize(button_text_char_size);
        text.setFillColor(text_color);
        text.setStyle(sf::Text::Bold);
        text.setPosition(element_position);
        
        sf::FloatRect bounds = text.getGlobalBounds();
        this->element_position.x = bounds.left;
        this->element_position.y = bounds.top;
        this->element_size = sf::Vector2f(bounds.width, bounds.height);
        element.setSize(element_size);
        element.setPosition(this->element_position);
        element.setOutlineColor(this->outline_color);
        element.setOutlineThickness(this->element_outline_thickness);
        element.setFillColor(sf::Color::Transparent);
    }

    sf::Text text;
    sf::Font& font;
    sf::RectangleShape element;
    sf::Vector2f element_position;
    sf::Vector2f element_size;
    sf::Color text_color = sf::Color::White;
    sf::Color outline_color = sf::Color::Red;
    sf::Color clicked_color = sf::Color::Yellow;
    sf::Color hover_color = sf::Color::Cyan;
    float element_outline_thickness = 2.0;
    int button_text_char_size = 24; //24 is default value
    std::string button_text;
    bool clicked = false;
    bool is_visible = false;

    void draw(sf::RenderWindow& window) { 
        if(is_visible){
            window.draw(this->element);
            window.draw(this->text);
        }
    }

    void update(int mouseX, int mouseY){
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
                result = this->hover_color;
            this->element.setFillColor(result);
        }
        else if(clicked == true){
            this->element.setFillColor(this->clicked_color);
        }
    }
};

class GUI {
    public:
        GUI();
        sf::Font font;
        std::vector<Button> buttons;
        void draw(sf::RenderWindow& window);
};

GUI::GUI() {

    if(!this->font.loadFromFile("./fonts/Akshar.ttf")){
        std::cout << "Error loading font\n";
    }
    else{
        std::cout << "Loaded font successfully!\n";
    }

    sf::Vector2f start_sim_pos(100.0, 50.0);
    Button start_simulation(this->font, "Start Simulation", start_sim_pos);
    buttons.push_back(start_simulation);

    sf::Vector2f reset_sim_pos = start_sim_pos;
    reset_sim_pos.x += (50+buttons[0].element.getSize().x);
    Button reset_simulation(this->font, "Reset Simulation", reset_sim_pos, false);
    buttons.push_back(reset_simulation);


    sf::Vector2f confirm_sim_pos = reset_sim_pos;
    confirm_sim_pos.x += (50+buttons[1].element.getSize().x);
    Button confirm_simulation(this->font, "Confirm", confirm_sim_pos, false);
    buttons.push_back(confirm_simulation);


    sf::Vector2f cancel_sim_pos = confirm_sim_pos;
    cancel_sim_pos.x += (50+buttons[2].element.getSize().x);
    Button cancel_simulation(this->font, "Cancel", cancel_sim_pos, false);
    buttons.push_back(cancel_simulation);

    sf::Vector2f bfs_button_pos = cancel_sim_pos;
    bfs_button_pos.x += (50+buttons[3].element.getSize().x);
    Button bfs_button(this->font, "BFS", bfs_button_pos, false);
    buttons.push_back(bfs_button);

    sf::Vector2f dfs_button_pos = bfs_button_pos;
    dfs_button_pos.x += (50+buttons[4].element.getSize().x);
    Button dfs_button(this->font, "DFS", dfs_button_pos, false);
    buttons.push_back(dfs_button);
    for(int i = 1; i < buttons.size(); i++){
        buttons[i].element.setOutlineColor(sf::Color::Blue);
    }

}

void GUI::draw(sf::RenderWindow& window){
    for(auto& button : buttons)
        button.draw(window);
}
