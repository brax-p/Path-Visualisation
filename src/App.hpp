#pragma once
#include "Controller.hpp"
#include "GUI.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <imgui.h>
#include <imgui-sfml.h>

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
    SCREEN_WIDTH = 1900;
    SCREEN_HEIGHT = 1000;
}

void App::run()
{

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), title, sf::Style::Titlebar | sf::Style::Close);
    window.setPosition(sf::Vector2i(0,0));
    sf::Vector2f algoDisplayPos(50.0f, 50.0f);
    sf::Vector2f algoDisplaySize(900.0f, 900.0f);
    
    AppState appState(OpState::INIT);
    AlgoModels algoModels(appState, algoDisplayPos, algoDisplaySize, window);
    GUI gui(window, algoModels, appState);
    Model model(window, gui, appState, algoModels);
    Controller controller(model);

    sf::Clock deltaClock;
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            gui.processEvent(event);
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
        gui.update(deltaClock.restart());
        gui.doStuff();
        controller.update(window);
        window.clear();
        model.draw();
        gui.render();
        window.display();
        sf::Time epoch_end = deltaClock.restart();
       // std::cout << "Frames per second: " << 1.0 / epoch_end.asSeconds() << "\n"; // FPS COUNTER IN TERMINAL
    }
}
