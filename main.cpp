#include "Controller.hpp"

const int windowX = 1000, windowY = 960;

namespace Paths {
    using namespace std;
    using namespace sf;
    struct App{
        public:
            void run(std::string title);
            AppState app_state;
    };
    void App::run(std::string title){
        RenderWindow window(VideoMode(windowX, windowY), title);
        window.setPosition(sf::Vector2i(0,50));
        GUI gui;
        Grid g(16);
        Model m(g, window, gui, app_state);
        Controller c(m);
        while(window.isOpen()){
            sf::Clock clock;
            Event event;
            while(window.pollEvent(event)){
                if(event.type == Event::Closed){
                    window.close();
                }
                if(event.type == Event::Resized){
                    FloatRect visibleArea(0,0,event.size.width, event.size.height);
                    window.setView(View(visibleArea));
                }
                c.updateEvent(event);
            }
            Time t1 = clock.getElapsedTime();
            this->app_state.update(t1);
            c.update(window,this->app_state);
            window.clear();
            m.draw();
            window.display();
            Time t2 = clock.restart();
        }
    }
}

int main(){
    Paths::App application;
    application.run("Paths!");
    return 0;
}
