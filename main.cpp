#include "Controller.hpp"

namespace Algos {
    using namespace std;
    using namespace sf;
    struct App{
        public:
            void run(std::string title);
    };
    void App::run(std::string title){
        RenderWindow window(VideoMode(1400,960), title);
        Model m;
        Controller c(m);
        
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
            window.display();
        }
    }
}

int main(){
    Algos::App application;
    application.run("Paths!");
    return 0;
}
