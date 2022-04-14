#include "form.h"
#include "controls.h"
#include "layout.h"
#include "benchmark.h"
#include <wx/splitter.h>
#include <wx/statline.h>
#include <string>
using namespace std;
using namespace ecosystem::ui;
using namespace ecosystem::logic;

BEGIN_EVENT_TABLE(Form, wxFrame)
END_EVENT_TABLE()

#define convertToWxId(ID) (wxID_HIGHEST + (int)ID)

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

    max_generation_info->SetLabel("Max generation: " + to_string(world->MaxGeneration()));
    entities_count_info->SetLabel("Entities: " + to_string(world->Entities().size()));
    ticks_info->SetLabel("Tics: " + to_string(ticks));

    const logic::Entity* observed_entity = canvas->get_observed_entity();
    if (observed_entity)
    {
        cell_age_info->SetLabel("age: " + to_string(observed_entity->Age()));
        cell_generation_info->SetLabel("generation: " + to_string(observed_entity->GetGenome().Generation()));
        cell_mutation_chance_info->SetLabel("mutation chance: " + to_string(observed_entity->GetGenome().MutationChance()));
        cell_genome_info->SetLabel("genome: " + to_string(observed_entity->GetGenome().Data()));
        cell_genome_args_info->SetLabel("genome args: " + to_string(observed_entity->GetGenome().Args()));
    }
    else
    {
        cell_age_info->SetLabel("age: -");
        cell_generation_info->SetLabel("generation: -");
        cell_mutation_chance_info->SetLabel("mutation chance: -");
        cell_genome_info->SetLabel("genome: -");
        cell_genome_args_info->SetLabel("genome args: -");
    }

    const logic::cell* observed_cell = canvas->get_observed_cell();
    if (observed_cell)
    {
        cell_coords_info->SetLabel("coords: x=" + to_string(observed_cell->x()) + " y=" + to_string(observed_cell->y()));
        if (observed_cell->ContainsOrganic())
        {
            const auto* organic = *observed_cell->GetOrganic();
            cell_organic_energy_info->SetLabel("organic energy: " + to_string(organic->Energy()));
        }
        cell_light_energy_info->SetLabel("light energy: " + to_string(observed_cell->LightPower()));
    }
    else
    {
        cell_coords_info->SetLabel("coords: -");
        cell_organic_energy_info->SetLabel("organic energy: -");
        cell_light_energy_info->SetLabel("light energy: -");
    }

    ticks++;

    Sleep(250 * MAX_SPEED - 250 * SPEED);
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

    canvas = new world_canvas(this, world);

    // Create and show bottom status bar
    CreateStatusBar();
    SetStatusText("mode: terrain");

    // Set up win menu panel
    wxMenu* menuAbout = new wxMenu;
    menuAbout->Append(convertToWxId(MenuItemID::Wiki), "&Wiki", "Wiki");
    menuAbout->Append(convertToWxId(MenuItemID::Repository), "&Repository", "Repository");
    menuAbout->AppendSeparator();
    menuAbout->Append(convertToWxId(MenuItemID::Author), "&Author", "Author");

    wxMenu* menuWorld = new wxMenu;
    menuWorld->Append(convertToWxId(MenuItemID::Run), "&Run", "Run");
    menuWorld->Append(convertToWxId(MenuItemID::Pause), "&Pause", "Pause");
    menuWorld->Append(convertToWxId(MenuItemID::ByTick), "&Runbytick", "Run by tick");
    menuWorld->Append(convertToWxId(MenuItemID::Hybernate), "&Hybernate", "Hybernate");

    wxMenu* menuView = new wxMenu;
    menuView->Append(convertToWxId(MenuItemID::Terrain), "&Terrain", "Terrain");
    menuView->Append(convertToWxId(MenuItemID::Energy), "&Energy", "Energy");
    menuView->Append(convertToWxId(MenuItemID::Organic), "&Organic", "Organic");
    menuView->Append(convertToWxId(MenuItemID::Species), "&Species", "Species");
    menuView->Append(convertToWxId(MenuItemID::Age), "&Age", "Age");
    menuView->Append(convertToWxId(MenuItemID::Ration), "&Ration", "Ration");
    menuView->Append(convertToWxId(MenuItemID::Success), "&Success", "Success");
    menuView->Append(convertToWxId(MenuItemID::Generation), "&Generation", "Generation");

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuAbout, "&About");
    menuBar->Append(menuWorld, "&World");
    menuBar->Append(menuView, "&View");

    SetMenuBar(menuBar);

    Bind(wxEVT_MENU, [](wxCommandEvent&)
    {
        ShellExecuteA(0, 0, "https://github.com/imnetcat/ecosystem/wiki", NULL, NULL, SW_SHOW);
    }, convertToWxId(MenuItemID::Wiki));
    Bind(wxEVT_MENU, [](wxCommandEvent&)
    {
        ShellExecuteA(0, 0, "https://github.com/imnetcat/ecosystem", NULL, NULL, SW_SHOW);
    }, convertToWxId(MenuItemID::Repository));
    Bind(wxEVT_MENU, [](wxCommandEvent&)
    {
        ShellExecuteA(0, 0, "https://github.com/imnetcat", NULL, NULL, SW_SHOW);
    }, convertToWxId(MenuItemID::Author));

    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        if (hibernate)
        {
            return;
        }
        pause = false;
    }, convertToWxId(MenuItemID::Run));
    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        if (hibernate)
        {
            return;
        }
        pause = true;
    }, convertToWxId(MenuItemID::Pause));
    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        if (hibernate)
        {
            return;
        }
        pause = true;
        do_tic = true;
    }, convertToWxId(MenuItemID::ByTick));
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
    }, convertToWxId(MenuItemID::Hybernate));

    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        canvas->SetMode(world_canvas::Mode::terrain);
        SetStatusText("mode: terrain");
    }, convertToWxId(MenuItemID::Terrain));
    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        canvas->SetMode(world_canvas::Mode::energy);
        SetStatusText("mode: energy");
    }, convertToWxId(MenuItemID::Energy));
    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        canvas->SetMode(world_canvas::Mode::organic);
        SetStatusText("mode: organic");
    }, convertToWxId(MenuItemID::Organic));
    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        canvas->SetMode(world_canvas::Mode::species);
        SetStatusText("mode: species");
    }, convertToWxId(MenuItemID::Species));
    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        canvas->SetMode(world_canvas::Mode::age);
        SetStatusText("mode: age");
    }, convertToWxId(MenuItemID::Age));
    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        canvas->SetMode(world_canvas::Mode::ration);
        SetStatusText("mode: ration");
    }, convertToWxId(MenuItemID::Ration));
    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        canvas->SetMode(world_canvas::Mode::success);
        SetStatusText("mode: success");
    }, convertToWxId(MenuItemID::Success));
    Bind(wxEVT_MENU, [&](wxCommandEvent&)
    {
        canvas->SetMode(world_canvas::Mode::generations);
        SetStatusText("mode: generations");
    }, convertToWxId(MenuItemID::Generation));

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
    max_generation_info = new wxStaticText(
        info_box->GetStaticBox(),
        wxID_ANY,
        ""
    );
    entities_count_info = new wxStaticText(
        info_box->GetStaticBox(),
        wxID_ANY,
        ""
    );
    ticks_info = new wxStaticText(
        info_box->GetStaticBox(),
        wxID_ANY,
        ""
    );

    info_box_layout->Add(max_generation_info);
    info_box_layout->Add(entities_count_info);
    info_box_layout->Add(ticks_info);

    info_box->Add(info_box_layout);

    wxStaticBoxSizer* selected_cell_info_box = new wxStaticBoxSizer(wxHORIZONTAL, this, "Selected cell");
    wxBoxSizer* selected_cell_info_box_layout = new wxBoxSizer(wxVERTICAL);
    cell_coords_info = new wxStaticText(
        selected_cell_info_box->GetStaticBox(),
        wxID_ANY,
        ""
    );
    cell_age_info = new wxStaticText(
        selected_cell_info_box->GetStaticBox(),
        wxID_ANY,
        ""
    );
    cell_generation_info = new wxStaticText(
        selected_cell_info_box->GetStaticBox(),
        wxID_ANY,
        ""
    );
    cell_mutation_chance_info = new wxStaticText(
        selected_cell_info_box->GetStaticBox(),
        wxID_ANY,
        ""
    );
    cell_organic_energy_info = new wxStaticText(
        selected_cell_info_box->GetStaticBox(),
        wxID_ANY,
        ""
    );
    cell_light_energy_info = new wxStaticText(
        selected_cell_info_box->GetStaticBox(),
        wxID_ANY,
        ""
    );
    cell_genome_info = new wxStaticText(
        selected_cell_info_box->GetStaticBox(),
        wxID_ANY,
        ""
    );
    cell_genome_args_info = new wxStaticText(
        selected_cell_info_box->GetStaticBox(),
        wxID_ANY,
        ""
    );

    selected_cell_info_box_layout->Add(cell_coords_info);
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
