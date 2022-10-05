#pragma once
#include <imgui.h>
#include <imgui-sfml.h>
//#include "AlgoModels.hpp"

enum class Pages { NONE, START, MAIN, EDIT, SIMULATE };

struct Page
{
    Page() { init(); }
    virtual ~Page() = default;
    virtual void init() {}
};
struct Start : public Page
{
    Start(Pages* p_CurrentPage) { init(p_CurrentPage); }
    void init(Pages* p_CurrentPage) 
    {
        ImVec2 windowSize = ImGui::GetMainViewport()->Size;
        ImVec2 windowPos = ImGui::GetMainViewport()->Pos;
        ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
        ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
        ImGuiWindowFlags flags = 0; flags = (ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
        ImGui::Begin("Title", NULL, flags);
        ImVec2 buttonSize(400, 100);
        ImVec2 buttonPos(windowPos.x + windowSize.x/2 - buttonSize.x/2, windowPos.y + windowSize.y/2 - buttonSize.y/2);
        ImGui::SetCursorPos(buttonPos);
        if(ImGui::Button("Start", buttonSize))
        {
            *p_CurrentPage = Pages::MAIN;
        }
        ImGui::End();
    }
};

struct Main : public Page
{
    Main(Pages* p_CurrentPage) { init(p_CurrentPage); }
    void init(Pages* p_CurrentPage)
    {
        
    }
};

class GUI
{
    public:
        GUI(sf::RenderWindow& p_Window);
        ~GUI();
        void init();
        void update(sf::Time epoch);
        void render();
        void doStuff();
        void processEvent(sf::Event& event);
    private:
        sf::RenderWindow& m_Window;
        Pages* m_CurrentPage;
};

GUI::GUI(sf::RenderWindow& p_Window) : m_Window(p_Window)
{
    init();
    m_CurrentPage = new Pages;
    *m_CurrentPage = Pages::START;
}

GUI::~GUI()
{
    ImGui::SFML::Shutdown();
}

void GUI::init()
{
    ImGui::SFML::Init(m_Window);
}

void GUI::update(sf::Time epoch)
{
    ImGui::SFML::Update(m_Window, epoch);
}

void GUI::render()
{
    ImGui::SFML::Render(m_Window);
}

void GUI::doStuff()
{
    switch(*m_CurrentPage)
    {
        case Pages::START:
        {
            Start start(m_CurrentPage);

            break;
        }
    }
}

void GUI::processEvent(sf::Event& event)
{
    ImGui::SFML::ProcessEvent(m_Window, event);
}
