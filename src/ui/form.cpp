#include "form.h"
#include "controls.h"
#include "layout.h"
#include "benchmark.h"
#include <wx/splitter.h>
#include <wx/statline.h>
using namespace ecosystem::ui;
using namespace ecosystem::logic;

BEGIN_EVENT_TABLE(Form, wxFrame)
END_EVENT_TABLE()

#define customWXId(ID) (wxID_HIGHEST + (int)ID)

Form::~Form()
{
    delete world;
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

    Sleep(5);
}

Form::Form()
    : wxFrame(
        NULL,
        wxID_ANY,
        "Ecosystem simulation",
        wxPoint(0, 0),
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

    canvas = new world_canvas(this, world);

    // Create and show bottom status bar
    CreateStatusBar();
    SetStatusText("mode: terrain");

    // Set up win menu panel
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
        SetStatusText("mode: terrain");
    }, customWXId(MenuItemID::Terrain));
    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        canvas->SetMode(world_canvas::Mode::energy);
        SetStatusText("mode: energy");
    }, customWXId(MenuItemID::Energy));
    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        canvas->SetMode(world_canvas::Mode::organic);
        SetStatusText("mode: organic");
    }, customWXId(MenuItemID::Organic));
    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        canvas->SetMode(world_canvas::Mode::species);
        SetStatusText("mode: species");
    }, customWXId(MenuItemID::Species));
    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        canvas->SetMode(world_canvas::Mode::age);
        SetStatusText("mode: age");
    }, customWXId(MenuItemID::Age));
    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        canvas->SetMode(world_canvas::Mode::ration);
        SetStatusText("mode: ration");
    }, customWXId(MenuItemID::Ration));
    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        canvas->SetMode(world_canvas::Mode::success);
        SetStatusText("mode: success");
    }, customWXId(MenuItemID::Success));
    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        canvas->SetMode(world_canvas::Mode::generations);
        SetStatusText("mode: generations");
    }, customWXId(MenuItemID::Generation));

    // Set up layout
    wxGridSizer* form_layout = new wxFlexGridSizer(2);
    wxBoxSizer* sidebar_layout = new wxBoxSizer(wxVERTICAL);

    wxStaticBoxSizer* speed_controls_box = new wxStaticBoxSizer(wxHORIZONTAL, this, "Speed");
    speed_controls_box->Add(
        new wxStaticText(
            speed_controls_box->GetStaticBox(),
            wxID_ANY,
            "Speed: "
        )
    );
    speed_controls_box->Add(
        new wxSlider(
            speed_controls_box->GetStaticBox(),
            wxID_ANY, SPEED, MIN_SPEED, MAX_SPEED,
            wxDefaultPosition, wxDefaultSize,
            wxSL_LABELS
        )
    );

    wxStaticBoxSizer* controls_box = new wxStaticBoxSizer(wxHORIZONTAL, this, "Controls");
    controls_box->Add(
        new wxStaticText(
            controls_box->GetStaticBox(),
            wxID_ANY, 
            "Zoom: "
        )
    );
    controls_box->Add(
        new wxSlider(
            controls_box->GetStaticBox(),
            wxID_ANY, ZOOM, MIN_ZOOM, MAX_ZOOM,
            wxDefaultPosition, wxDefaultSize,
            wxSL_LABELS
        )
    );
    
    wxStaticBoxSizer* info_box = new wxStaticBoxSizer(wxHORIZONTAL, this, "System info");
    wxBoxSizer* info_box_layout = new wxBoxSizer(wxVERTICAL);
    wxStaticText* max_generation_info = new wxStaticText(
        info_box->GetStaticBox(),
        wxID_ANY,
        "Max generation: "
    );
    wxStaticText* entities_count_info = new wxStaticText(
        info_box->GetStaticBox(),
        wxID_ANY,
        "Entities: "
    );
    wxStaticText* ticks_info = new wxStaticText(
        info_box->GetStaticBox(),
        wxID_ANY,
        "Tics: "
    );

    info_box_layout->Add(max_generation_info);
    info_box_layout->Add(entities_count_info);
    info_box_layout->Add(ticks_info);

    info_box->Add(info_box_layout);

    wxStaticBoxSizer* selected_cell_info_box = new wxStaticBoxSizer(wxHORIZONTAL, this, "Selected cell");
    wxBoxSizer* selected_cell_info_box_layout = new wxBoxSizer(wxVERTICAL);
    wxStaticText* cell_age_info = new wxStaticText(
        selected_cell_info_box->GetStaticBox(),
        wxID_ANY,
        "age: -"
    );
    wxStaticText* cell_generation_info = new wxStaticText(
        selected_cell_info_box->GetStaticBox(),
        wxID_ANY,
        "generation: -"
    );
    wxStaticText* cell_mutation_chance_info = new wxStaticText(
        selected_cell_info_box->GetStaticBox(),
        wxID_ANY,
        "mutation chance: -"
    );
    wxStaticText* cell_organic_energy_info = new wxStaticText(
        selected_cell_info_box->GetStaticBox(),
        wxID_ANY,
        "organic energy: -"
    );
    wxStaticText* cell_light_energy_info = new wxStaticText(
        selected_cell_info_box->GetStaticBox(),
        wxID_ANY,
        "light energy: -"
    );
    wxStaticText* cell_genome_info = new wxStaticText(
        selected_cell_info_box->GetStaticBox(),
        wxID_ANY,
        "genome: -"
    );
    wxStaticText* cell_genome_args_info = new wxStaticText(
        selected_cell_info_box->GetStaticBox(),
        wxID_ANY,
        "genome args: -"
    );

    selected_cell_info_box_layout->Add(cell_age_info);
    selected_cell_info_box_layout->Add(cell_generation_info);
    selected_cell_info_box_layout->Add(cell_mutation_chance_info);
    selected_cell_info_box_layout->Add(cell_organic_energy_info);
    selected_cell_info_box_layout->Add(cell_light_energy_info);
    selected_cell_info_box_layout->Add(cell_genome_info);
    selected_cell_info_box_layout->Add(cell_genome_args_info);

    selected_cell_info_box->Add(selected_cell_info_box_layout);
    
    sidebar_layout->Add(speed_controls_box, wxSizerFlags(0).Border(wxBOTTOM, MARGIN).Align(wxEXPAND));
    sidebar_layout->Add(controls_box, wxSizerFlags(0).Border(wxBOTTOM, MARGIN).Align(wxEXPAND));
    sidebar_layout->Add(info_box, wxSizerFlags(0).Border(wxBOTTOM, MARGIN).Align(wxEXPAND));
    sidebar_layout->Add(selected_cell_info_box , wxSizerFlags(0).Border(wxBOTTOM, MARGIN).Align(wxEXPAND));

    form_layout->Add(sidebar_layout, wxSizerFlags(0).Border(wxALL, MARGIN));
    form_layout->Add(canvas);

    // use the sizer for layout and size window
    // accordingly and prevent it from being resized
    // to smaller size
    SetSizerAndFit(form_layout);
}
