#pragma once
#include "Controller.hpp"
#include <SFML/Graphics.hpp>
#include <string>

class App
{
    public:
        App();
        void run();
    
    private:
        std::string title;
        uint32_t SCREEN_WIDTH;
        uint32_t SCREEN_HEIGHT;
};

App::App()
{
    title = "Paths!";
    SCREEN_WIDTH = 1275;
    SCREEN_HEIGHT = 800;
}

void App::run()
{

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), title, sf::Style::Titlebar | sf::Style::Close);
    window.setPosition(sf::Vector2i(250, 125));

    AppState *app_state = new AppState(OpState::Edit);
    GUI gui;
    Grid grid(15, app_state);
    Model model(grid, window, gui, app_state);
    Controller controller(model);

    while(window.isOpen())
    {
        sf::Clock clock;
        sf::Event event;
        while(window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                {
                    window.close();
                    break;
                }
                case sf::Event::Resized:
                {
                    sf::FloatRect visible_area(0, 0, event.size.width, event.size.height);
                    window.setView(sf::View(visible_area));
                    break;
                }
            }
            controller.update_event(event);
        }
        sf::Time epoch_begin = clock.getElapsedTime();
        controller.update(window);
        window.clear();
        model.draw();
        window.display();
        sf::Time epoch_end = clock.restart();
        //std::cout << "Frames per second: " << 1.0 / epoch_end.asSeconds() << "\n";
    }
    delete app_state;
    std::cout << "Ending window size: " << window.getSize().x << "," << window.getSize().y << '\n';
}
