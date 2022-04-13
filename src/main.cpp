#define _CRT_SECURE_NO_WARNINGS // For wxWidgets lib
#include "ui/controls.h"
#include "ui/world_canvas.h"
#include "ui/benchmark.h"
#include "ui/form.h"

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

class MyApp : public wxApp
{
public:
    bool render_loop_on;
    bool OnInit();
    void onIdle(wxIdleEvent& evt);
    void onClose(wxCloseEvent& evt);

    Form* form;
public:
    void activateRenderLoop(bool on);
};

wxIMPLEMENT_APP(MyApp);

void MyApp::onClose(wxCloseEvent& evt)
{
    activateRenderLoop(false);
    evt.Skip(); // don't stop event, we still want window to close
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


bool MyApp::OnInit()
{
    unsigned __int64 GENOME_SEED = random::Generate();
    Genome::Init(GENOME_SEED);
    
    form = new Form();
    form->Show();

    activateRenderLoop(true);
    return true;
}
