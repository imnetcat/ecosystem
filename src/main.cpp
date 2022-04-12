#define _CRT_SECURE_NO_WARNINGS // For wxWidgets lib
#include "ui/controls.h"
#include "ui/world_canvas.h"
#include "logic/world.h"
#include "ui/benchmark.h"

#include <wx/xrc/xmlres.h>
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <iostream>
#include <string>
#include <map>

#include <windows.h>
#include <ShellApi.h>

using namespace ecosystem::logic;
using namespace ecosystem::ui;

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;

class Form;

class MyApp : public wxApp
{
public:
    bool render_loop_on;
    bool OnInit();
    void onIdle(wxIdleEvent& evt);

    Form* form;
public:
    void activateRenderLoop(bool on);
};

wxIMPLEMENT_APP(MyApp);

class Form : public wxFrame
{
public:
    Form();
    ~Form();
    void onIdle(wxIdleEvent& evt);
    void onClose(wxCloseEvent& evt)
    {
        wxGetApp().activateRenderLoop(false);
        evt.Skip(); // don't stop event, we still want window to close
    }
    DECLARE_EVENT_TABLE()
private:
    World* world;
    world_canvas* canvas;
    wxSlider* zoom_slider;
    wxSlider* speed_slider;
};

BEGIN_EVENT_TABLE(Form, wxFrame)
EVT_CLOSE(Form::onClose)
END_EVENT_TABLE()

Form::~Form()
{
    delete world;
}

void MyApp::activateRenderLoop(bool on)
{
    if (on && !render_loop_on)
    {
        Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(MyApp::onIdle));
        render_loop_on = true;
    }
    else if (!on && render_loop_on)
    {
        Disconnect(wxEVT_IDLE, wxIdleEventHandler(MyApp::onIdle));
        render_loop_on = false;
    }
}

void MyApp::onIdle(wxIdleEvent& evt)
{
    form->onIdle(evt);
    evt.RequestMore(); // render continuously, not only once on idle
}

#define customWXId(ID) (wxID_HIGHEST + (int)ID)

enum class MenuItemID
{
    Wiki = 1,
    Repository,
    Author,

    Run,
    Pause,
    ByTick,
    Hybernate,

    Terrain,
    Energy,
    Organic,
    Species,
    Age,
    Ration,
    Success,
    Generation,
};

bool MyApp::OnInit()
{
    unsigned __int64 GENOME_SEED = random::Generate();
    Genome::Init(GENOME_SEED);
    
    /*
    wxXmlResource::Get()->InitAllHandlers();
    wxXmlResource::Get()->Load("src/layout.xrc");
    wxXmlResource::Get()->LoadAllFiles("rc");


    wxDialog dlg;
    if (wxXmlResource::Get()->LoadDialog(&dlg, NULL, "SimpleDialog"))
        dlg.ShowModal();
        */

    form = new Form();
   
    form->Show();

    activateRenderLoop(true);
    return true;
}

void Form::onIdle(wxIdleEvent& evt)
{
    MYBENCHMARK(world->Update(), "World update max time");

    // Recreate world if all entities die
    if (world->Entities().size() == 0)
    {
        world = new World(
            WORLD_WIDTH,
            WORLD_HEIGHT,
            LIGHT_POWER,
            MAX_ORGANIC_TO_EAT,
            MAX_ENTITIES_TO_EAT,
            MAX_ENERGY,
            MAX_AGE
        );

        canvas->change_world(world);
        return;
    }

    canvas->paintNow();

    Sleep(SPEED);
}

Form::Form()
    : wxFrame(
        NULL, 
        wxID_ANY, 
        "Ecosystem simulation", 
        wxPoint(150, 150), 
        wxSize(WINDOW_WIDTH, WINDOW_HEIGHT)
    )
{
    world = new World(
        WORLD_WIDTH,
        WORLD_HEIGHT,
        LIGHT_POWER,
        MAX_ORGANIC_TO_EAT,
        MAX_ENTITIES_TO_EAT,
        MAX_ENERGY,
        MAX_AGE
    );

    canvas = new world_canvas(300, 0, 600, 800, this, world);

    wxMenu* menuAbout = new wxMenu;
    menuAbout->Append(customWXId(MenuItemID::Wiki), "&Wiki", "Wiki");
    menuAbout->Append(customWXId(MenuItemID::Repository), "&Repository", "Repository");
    menuAbout->AppendSeparator();
    menuAbout->Append(customWXId(MenuItemID::Author), "&Author", "Author");

    wxMenu* menuWorld = new wxMenu;
    menuWorld->Append(customWXId(MenuItemID::Run), "&Run", "Run");
    menuWorld->Append(customWXId(MenuItemID::Pause), "&Pause", "Pause");
    menuWorld->Append(customWXId(MenuItemID::ByTick), "&Runbytick", "Run by tick");
    menuWorld->Append(customWXId(MenuItemID::Hybernate), "&Hybernate", "Hybernate");

    wxMenu* menuView = new wxMenu;
    menuView->Append(customWXId(MenuItemID::Terrain), "&Terrain", "Terrain");
    menuView->Append(customWXId(MenuItemID::Energy), "&Energy", "Energy");
    menuView->Append(customWXId(MenuItemID::Organic), "&Organic", "Organic");
    menuView->Append(customWXId(MenuItemID::Species), "&Species", "Species");
    menuView->Append(customWXId(MenuItemID::Age), "&Age", "Age");
    menuView->Append(customWXId(MenuItemID::Ration), "&Ration", "Ration");
    menuView->Append(customWXId(MenuItemID::Success), "&Success", "Success");
    menuView->Append(customWXId(MenuItemID::Generation), "&Generation", "Generation");

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuAbout, "&About");
    menuBar->Append(menuWorld, "&World");
    menuBar->Append(menuView, "&View");

    SetMenuBar(menuBar);

    Bind(wxEVT_MENU, [](wxCommandEvent&)
    {
        ShellExecuteA(0, 0, "https://github.com/imnetcat/ecosystem/wiki", NULL, NULL, SW_SHOW);
    }, customWXId(MenuItemID::Wiki));
    Bind(wxEVT_MENU, [](wxCommandEvent&)
    {
        ShellExecuteA(0, 0, "https://github.com/imnetcat/ecosystem", NULL, NULL, SW_SHOW);
    }, customWXId(MenuItemID::Repository));
    Bind(wxEVT_MENU, [](wxCommandEvent&)
    {
        ShellExecuteA(0, 0, "https://github.com/imnetcat", NULL, NULL, SW_SHOW);
    }, customWXId(MenuItemID::Author));

    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        if (hibernate)
        {
            return;
        }
        pause = false;
    }, customWXId(MenuItemID::Run));
    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        if (hibernate)
        {
            return;
        }
        pause = true;
    }, customWXId(MenuItemID::Pause));
    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        if (hibernate)
        {
            return;
        }
        pause = true;
        do_tic = true;
    }, customWXId(MenuItemID::ByTick));
    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        hibernate = !hibernate;
        pause = false;
        if (hibernate)
        {
            /*
            canvas->clear(sf::Color::White);
            canvas->display();
            speed_slider->setValue(0);
            speed_slider->setEnabled(false);
        }
        else
        {
            speed_slider->setEnabled(true);
            */
        }
    }, customWXId(MenuItemID::Hybernate));

    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        canvas->SetMode(world_canvas::Mode::terrain);
    }, customWXId(MenuItemID::Terrain));
    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        canvas->SetMode(world_canvas::Mode::energy);
    }, customWXId(MenuItemID::Energy));
    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        canvas->SetMode(world_canvas::Mode::organic);
    }, customWXId(MenuItemID::Organic));
    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        canvas->SetMode(world_canvas::Mode::species);
    }, customWXId(MenuItemID::Species));
    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        canvas->SetMode(world_canvas::Mode::age);
    }, customWXId(MenuItemID::Age));
    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        canvas->SetMode(world_canvas::Mode::ration);
    }, customWXId(MenuItemID::Ration));
    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        canvas->SetMode(world_canvas::Mode::success);
    }, customWXId(MenuItemID::Success));
    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        canvas->SetMode(world_canvas::Mode::generations);
    }, customWXId(MenuItemID::Generation));

    // Create sidebar

    zoom_slider = new wxSlider(this, -1, 1, 1, 3);

}
