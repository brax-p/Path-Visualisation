#include "Controller.hpp"

namespace Paths {
    using namespace std;
    using namespace sf;
    struct App{
        public:
            void run(std::string title);
            AppState app_state;
    };
    void App::run(std::string title){
        RenderWindow window(VideoMode(900,960), title);
        GUI gui;
        Grid g(15);
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


//  TODO:
//
//  PLAN THE WORK, THEN WORK THE PLAN
//
//  [COMPLETE]*Modify the Button structs to take optional arguments for things like size, position, color, etc.
//  Let the default values be the values already used in the implementation
//  This is so creating differing buttons can happen in one line, during initialization,
//  rather than initialization -> changing of size, pos, etc inside the struct
//
//
//  [COMPLETE]*Make the following buttons:
//  Start Simulation
//  Reset Grid
//  ----
//  Buttons for the different algorithms to be used on the Grid
//  let only one button be active at a time -- probably should use the Toggle_Button here
//  In Toggle_Button, implement the toggle feature, that is create a state that is updated when 
//  left clicked
// 
//  *Implement the stepwise simulation -- saving the step wise results and then display
//  step wise
//
//  Implement new algorithms and then add corresponding button,
//  if algo button list becomes too long, think of another solutions?
//  Maybe a new element in GUI?
//
//  --------------------------------------------------------------------------------
//
//  Undesired Behavior:
//  Random green tiles when wall or spawn or goal updates
//  maybe some inprecision within the add/remove vertex? maybe when leftClickDown or rightClickDown?
//
