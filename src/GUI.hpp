#pragma once
#include <imgui.h>
#include <imgui-sfml.h>
#include "AlgoModels.hpp"

enum class Pages { NONE, START, MAIN, EDIT, SIMULATE };

struct Page
{
    Page() { }
    virtual ~Page() = default;
    virtual void init() {}
};

struct Start : public Page
{
    Pages& currentPage;
    AppState& appState;
    AlgoModels& algoModel;
    Start(Pages& p_CurrentPage, AppState& p_appState, AlgoModels& p_algoModel) :
        appState(p_appState),
        algoModel(p_algoModel),
        currentPage(p_CurrentPage)
    { 
        currentPage = p_CurrentPage;
    }
    void init() 
    {
        ImVec2 windowSize = ImVec2(400, 100);
        ImVec2 windowPos = ImGui::GetMainViewport()->Pos;
        windowPos.x = (ImGui::GetMainViewport()->Size.x - windowSize.x) / 2;
        windowPos.y = (ImGui::GetMainViewport()->Size.y - windowSize.y) / 2;

        ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
        ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
        ImGuiWindowFlags flags = 0; flags = (ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
        ImGui::Begin("Start", NULL, flags);
        ImGui::SetWindowFontScale(2.0f);
        ImVec2 buttonSize = ImGui::GetWindowSize();
        ImVec2 buttonPos(0,0);
        ImGui::SetCursorPos(buttonPos);
        if(ImGui::Button("Start", buttonSize))
        {
            currentPage = Pages::MAIN;
        }
        ImGui::End();
    }
};

struct Main : public Page
{
    AppState& appState;
    Pages& currentPage;
    AlgoModels& algoModel;

    Main(Pages& p_CurrentPage, AppState& p_appState, AlgoModels& p_algoModel) :
        appState(p_appState),
        algoModel(p_algoModel),
        currentPage(p_CurrentPage)
    { 

    }

    void init()
    {
        ImVec2 windowSize = ImGui::GetMainViewport()->Size;     windowSize.x *= (.4); windowSize.y *= (0.9);
        ImVec2 windowPos = ImVec2((ImGui::GetMainViewport()->Size.x - windowSize.x) / 2, (ImGui::GetMainViewport()->Size.y - windowSize.y) / 2);
        ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
        ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
        ImGuiWindowFlags flags = 0; flags = (ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
        ImGui::Begin("Main", NULL, flags);
        ImGui::SetWindowFontScale(2.0f);
        float verticalPadding = 45.0f;
        ImVec2 centeredTitle =ImVec2(ImGui::GetWindowSize().x / 2 - (ImGui::CalcTextSize("Main Menu", NULL, false).x * 0.475), verticalPadding);
        ImGui::SetCursorPos(centeredTitle);
        ImGui::Text("Main Menu");
        ImGui::NewLine();
        centeredTitle.x *= (.9);
        centeredTitle.y += (2.5 * verticalPadding);
        ImGui::SetCursorPos(centeredTitle);
        ImGui::Text("Choose a Mode:");
        ImGui::NewLine();
        centeredTitle.y += (1.5 * verticalPadding);
        ImGui::SetCursorPos(centeredTitle);
        static int chosen = 1;
        if(ImGui::RadioButton("Grid", &chosen, 1))
        {
            algoModel.currentType = ModelType::GRID;
        }
        ImGui::SameLine();
        if(ImGui::RadioButton("Graph", &chosen, 2))
        {
            algoModel.currentType = ModelType::GRAPH;
        }
        ImGui::NewLine();

        ImVec2 buttonSize = ImVec2(ImGui::GetWindowSize().x * 0.6, 100);
        float buttonPadding = buttonSize.y + verticalPadding;
        auto getCenteredButtonAtYLevel = [&](int yLevel)
        {
            return ImVec2((ImGui::GetWindowSize().x - buttonSize.x) / 2, yLevel);
        };
        int startingYCoord = 300;
        ImVec2 buttonPos = getCenteredButtonAtYLevel(startingYCoord);
        ImGui::SetCursorPos(buttonPos);
        if(ImGui::Button("Edit", buttonSize))
        {
            algoModel.allowDisplay();
            appState.current_state = OpState::Edit;
            currentPage = Pages::EDIT;
        }
        ImGui::SetCursorPos(getCenteredButtonAtYLevel(startingYCoord + buttonPadding));
        if(ImGui::Button("Simulate", buttonSize))
        {
            algoModel.allowDisplay();
            appState.current_state = OpState::Simulate;
            currentPage = Pages::SIMULATE;
        }

        ImGui::End();
    }
};

struct EditGrid : public Page
{
    AppState& appState;
    Pages& currentPage;
    AlgoModels& algoModel;
    EditGrid(Pages& p_currentPage, AppState& p_appState, AlgoModels& p_algoModel) :
        appState(p_appState),
        algoModel(p_algoModel),
        currentPage(p_currentPage)
    {

    }

    void init()
    {
        ImVec2 containerSize(750, algoModel.displaySize.y);       
        ImVec2 containerPos(ImGui::GetMainViewport()->Size.x - 100 - containerSize.x, ImGui::GetMainViewport()->Size.y - 50 - containerSize.y);
        ImGui::SetNextWindowSize(containerSize);
        ImGui::SetNextWindowPos(containerPos);
        ImGuiWindowFlags flags = 0; flags = (ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);

        ImGui::Begin("Edit Page", NULL, flags);
        ImGui::SetWindowFontScale(2.0f);
        float verticalPadding = 45.0f;
        ImVec2 centeredTitle =ImVec2(ImGui::GetWindowSize().x / 2 - (ImGui::CalcTextSize("Edit", NULL, false).x * 0.475), verticalPadding);
        ImGui::SetCursorPos(centeredTitle);
        ImGui::Text("Edit");
        centeredTitle.y += (2 * verticalPadding);
        centeredTitle.x *= (.70);
        ImGui::SetCursorPos(centeredTitle);
        ImGui::Text("Choose what to edit:");
        centeredTitle.y += (1.5 * verticalPadding);
        centeredTitle.x *= (.90);
        ImGui::SetCursorPos(centeredTitle);
        static int selectedEditMode = 1;
        if(ImGui::RadioButton("Walls", &selectedEditMode, 1))
        {
            algoModel.grid.editState = Editing::Wall;
        }
        ImGui::SameLine();
        if(ImGui::RadioButton("Spawn", &selectedEditMode, 2))
        {
            algoModel.grid.editState = Editing::Spawn;
        }
        ImGui::SameLine();
        if(ImGui::RadioButton("Goal", &selectedEditMode, 3))
        {
            algoModel.grid.editState = Editing::Goal;
        }

        float bottomBtnPadding = 30.0f;
        ImVec2 buttonSize((containerSize.x - (3 * bottomBtnPadding)) / 2, 100);
        ImGui::SetCursorPos(ImVec2(0 + bottomBtnPadding, containerSize.y - bottomBtnPadding - buttonSize.y));
        if(ImGui::Button("Main Menu", buttonSize))
        {
            currentPage = Pages::MAIN;
            algoModel.disallowDisplay();
        }
        ImGui::SetCursorPos(ImVec2((2 * bottomBtnPadding) + buttonSize.x, containerSize.y - bottomBtnPadding - buttonSize.y));
        if(ImGui::Button("Simulate", buttonSize))
        {
            currentPage = Pages::SIMULATE;
        }
        ImVec2 adjBtnSize(buttonSize.x/2 - bottomBtnPadding, 100);
        ImGui::SetCursorPos(ImVec2(bottomBtnPadding, containerSize.y - 2*bottomBtnPadding - buttonSize.y - adjBtnSize.y));
        if(ImGui::Button("-", adjBtnSize))
        {
            algoModel.grid.decreaseGridSize();
        }
        ImGui::SetCursorPos(ImVec2(0 + 3*bottomBtnPadding + adjBtnSize.x, containerSize.y - 2*bottomBtnPadding - buttonSize.y - adjBtnSize.y));
        if(ImGui::Button("+", adjBtnSize))
        {
            algoModel.grid.increaseGridSize();
        }
        ImGui::SetCursorPos(ImVec2(0 + 1*bottomBtnPadding, containerSize.y - 3.5*bottomBtnPadding - buttonSize.y - adjBtnSize.y));
        ImGui::Text("Decrease/Increase Grid Size");
        
        ImGui::End();
    }
};

struct Simulate : public Page 
{
    AppState& appState;
    Pages& currentPage;
    AlgoModels& algoModel;
    std::string speedupText;
    std::string stepText;
    Simulate(Pages& p_currentPage, AppState& p_appState, AlgoModels& p_algoModel) :
        appState(p_appState),
        algoModel(p_algoModel),
        currentPage(p_currentPage)
    {
        speedupText = "Simulation Speed: 1x";
        stepText = "Step 0";
    }

    void init()
    {
        ImVec2 containerSize(750, algoModel.displaySize.y);       
        ImVec2 containerPos(ImGui::GetMainViewport()->Size.x - 100 - containerSize.x, ImGui::GetMainViewport()->Size.y - 50 - containerSize.y);
        ImGui::SetNextWindowSize(containerSize);
        ImGui::SetNextWindowPos(containerPos);
        ImGuiWindowFlags flags = 0; flags = (ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);

        ImGui::Begin("Simulate Page", NULL, flags);
        ImGui::SetWindowFontScale(2.0f);
        float verticalPadding = 45.0f;
        ImVec2 centeredTitle =ImVec2(ImGui::GetWindowSize().x / 2 - (ImGui::CalcTextSize("Simulate", NULL, false).x * 0.475), verticalPadding);
        ImGui::SetCursorPos(centeredTitle);
        ImGui::Text("Simulate");

        ImVec2 choosePrompt = ImVec2(ImGui::GetWindowSize().x / 2 - (ImGui::CalcTextSize("Choose an Algorithm:", NULL, false).x * 0.475), 2*verticalPadding + centeredTitle.y);
        ImGui::SetCursorPos(choosePrompt);
        ImGui::Text("Choose an Algorithm:");
        
        ImVec2 algosRadios = ImVec2(ImGui::GetWindowSize().x / 2 - (ImGui::CalcTextSize("Choose an Algorithm:", NULL, false).x * 0.275), verticalPadding + choosePrompt.y);
        ImGui::SetCursorPos(algosRadios);
        static int algos = 0;
        bool algoHasChanged = false;
        if(ImGui::RadioButton("BFS", &algos, 0))
        {
            algoModel.grid.changeAlgo(Algos::BFS);
            algoHasChanged = true;
        }
        ImGui::SameLine();
        if(ImGui::RadioButton("DFS", &algos, 1))
        {
            algoModel.grid.changeAlgo(Algos::DFS);
            algoHasChanged = true;
        }
        if(algoHasChanged)
        {
            algoModel.grid.clearGrid();
            algoModel.grid.resetSimulation();
        }
        
        float bottomBtnPadding = 30.0f;
        ImVec2 buttonSize((containerSize.x - (3 * bottomBtnPadding)) / 2, 100);
        ImGui::SetCursorPos(ImVec2(0 + bottomBtnPadding, containerSize.y - bottomBtnPadding - buttonSize.y));
        if(ImGui::Button("Main Menu", buttonSize))
        {
            currentPage = Pages::MAIN;
            algoModel.disallowDisplay();
        }
        ImGui::SetCursorPos(ImVec2((2 * bottomBtnPadding) + buttonSize.x, containerSize.y - bottomBtnPadding - buttonSize.y));
        if(ImGui::Button("Edit", buttonSize))
        {
            appState.current_state = OpState::Edit;
            currentPage = Pages::EDIT;
            algoModel.grid.resetSimulation();
        }
        ImVec2 activationButtonSize((ImGui::GetWindowSize().x - 4*bottomBtnPadding) / 3, 100);
        ImGui::SetCursorPos(ImVec2(bottomBtnPadding, 550));
        if(ImGui::Button("<<", activationButtonSize))
        {
            speedupText = "Simulation Speed: " + algoModel.grid.slowdown();
        }
        ImGui::SetCursorPos(ImVec2(2*bottomBtnPadding+activationButtonSize.x, 550));
        if(ImGui::Button("Play", activationButtonSize))
        {
            algoModel.grid.Simulate();
        }
        ImGui::SetCursorPos(ImVec2(3*bottomBtnPadding+2*activationButtonSize.x, 550));
        if(ImGui::Button(">>", activationButtonSize))
        {
            speedupText = "Simulation Speed: " + algoModel.grid.speedup();
        }
        ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x / 2 - ImGui::CalcTextSize(speedupText.c_str()).x / 2, 450));
        ImGui::Text(speedupText.c_str());
        stepText = algoModel.grid.getStepString();
        ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x / 2 - ImGui::CalcTextSize(stepText.c_str()).x / 2, 350));
        ImGui::Text(stepText.c_str());
        ImGui::End();
    }
     
};

class GUI
{
    public:
        GUI(sf::RenderWindow& p_Window, AlgoModels& p_algoModels, AppState& p_appState);
        ~GUI();
        void init();
        void update(sf::Time epoch);
        void render();
        void doStuff();
        void processEvent(sf::Event& event);

    private:
        sf::RenderWindow& m_Window;
        Pages m_CurrentPage;
        AlgoModels& m_algoModels;
        AppState& m_appState;
        Start start;
        Main main;
        EditGrid editGrid;
        Simulate simulate;
};

GUI::GUI(sf::RenderWindow& p_Window, AlgoModels& p_algoModels, AppState& p_appState) : 
    m_Window(p_Window),
    m_algoModels(p_algoModels),
    m_appState(p_appState),
    editGrid(m_CurrentPage, m_appState, m_algoModels),
    simulate(m_CurrentPage, m_appState, m_algoModels),
    main(m_CurrentPage, m_appState, m_algoModels),
    start(m_CurrentPage, m_appState, m_algoModels)
{
    this->init();
    m_CurrentPage = Pages::START;
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
    switch(m_CurrentPage)
    {
        case Pages::START:
        {
            start.init();
            break;
        }
        
        case Pages::MAIN:
        {
            main.init();
            break;
        }

        case Pages::EDIT:
        {
            if(m_algoModels.currentType == ModelType::GRID)
            {
                editGrid.init();
            }
            break;
        }

        case Pages::SIMULATE:
        {
            simulate.init();
            break;
        }
    }
}

void GUI::processEvent(sf::Event& event)
{
    ImGui::SFML::ProcessEvent(m_Window, event);
}
