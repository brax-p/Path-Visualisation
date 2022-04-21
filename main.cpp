#include "Controller.hpp"
#include "Button.hpp"
namespace Paths {
    using namespace std;
    using namespace sf;
    struct App{
        public:
            void run(std::string title);
    };
    void App::run(std::string title){
        RenderWindow window(VideoMode(1400,960), title);
        Grid g(10);
        Model m(g);
        Controller c(m);

        sf::Vector2f size(300,150);
        sf::Vector2f pos(0,0);
        Button b(pos, size, sf::Color::Magenta, "Hello World!");

        while(window.isOpen()){
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
            c.update(window);
            window.clear();
            m.draw(window);
            b.draw(window);
            window.display();
        }
    }
}

int main(){
    Paths::App application;
    application.run("Paths!");
    return 0;
}
