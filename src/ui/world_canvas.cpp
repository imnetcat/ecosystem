#include "world_canvas.h"
#include "controls.h"
#include "layout.h"
#include "colors.h"
using namespace ecosystem::ui;
using namespace ecosystem::logic;

class OutlinedCellRenderer : public wxGridCellRenderer
{
public:
	OutlinedCellRenderer() = default;
	OutlinedCellRenderer(const OutlinedCellRenderer& obj)
	{
		m_clientData = obj.m_clientData;
		m_clientDataType = obj.m_clientDataType;
		m_clientObject = obj.m_clientObject;
	}
	// Should not used in my app
	OutlinedCellRenderer* Clone() const
	{
		return new OutlinedCellRenderer(*this);
	}
	// Not used in my app
	wxSize GetBestSize(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, int row, int col)
	{
		return {};
	}
	void Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected);
};

void OutlinedCellRenderer::Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected)
{
	wxGridCellRenderer::Draw(grid, attr, dc, rect, row, col, isSelected);

	// top line
	dc.SetPen(wxPen(BLACK, 1));
	dc.DrawLine(rect.x, rect.y, rect.x + rect.width, rect.y);

	// left hand line
	dc.SetPen(wxPen(BLACK, 1));
	dc.DrawLine(rect.x, rect.y, rect.x, rect.y + rect.height);

	// right hand line
	dc.SetPen(wxPen(BLACK, 1));
	dc.DrawLine(rect.x + rect.width, rect.y, rect.x + rect.width, rect.y + rect.height);

	// bottom line
	dc.SetPen(wxPen(BLACK, 1));
	dc.DrawLine(rect.x, rect.y + rect.height, rect.x + rect.width, rect.y + rect.height);
}

BEGIN_EVENT_TABLE(world_canvas, wxPanel)
// some useful events
/*
 EVT_MOTION(BasicDrawPane::mouseMoved)
 EVT_LEFT_DOWN(BasicDrawPane::mouseDown)
 EVT_LEFT_UP(BasicDrawPane::mouseReleased)
 EVT_RIGHT_DOWN(BasicDrawPane::rightClick)
 EVT_LEAVE_WINDOW(BasicDrawPane::mouseLeftWindow)
 EVT_KEY_DOWN(BasicDrawPane::keyPressed)
 EVT_KEY_UP(BasicDrawPane::keyReleased)
 EVT_MOUSEWHEEL(BasicDrawPane::mouseWheelMoved)
*/
EVT_GRID_SELECT_CELL(world_canvas::on_select_cell)
// catch paint events
EVT_PAINT(world_canvas::paintEvent)

END_EVENT_TABLE()


// some useful events
/*
 void BasicDrawPane::mouseMoved(wxMouseEvent& event) {}
 void BasicDrawPane::mouseDown(wxMouseEvent& event) {}
 void BasicDrawPane::mouseWheelMoved(wxMouseEvent& event) {}
 void BasicDrawPane::mouseReleased(wxMouseEvent& event) {}
 void BasicDrawPane::rightClick(wxMouseEvent& event) {}
 void BasicDrawPane::mouseLeftWindow(wxMouseEvent& event) {}
 void BasicDrawPane::keyPressed(wxKeyEvent& event) {}
 void BasicDrawPane::keyReleased(wxKeyEvent& event) {}
*/

world_canvas::world_canvas(
	wxWindow* parent,
	logic::World* world
)
	: wxGrid(
		parent, wxID_ANY, { 0, 0 },
		{
			static_cast<int>(WORLD_WIDTH * CELL_SIZE + WORLD_WIDTH * 2),
			static_cast<int>(WORLD_HEIGHT * CELL_SIZE + WORLD_HEIGHT * 2)
		}
	)
	, observed_cell(nullptr)
	, observed_entity(nullptr)
	, world(world)
{
	// Set up grid dimension and diable selecting
	CreateGrid(WORLD_HEIGHT, WORLD_WIDTH, wxGrid::wxGridSelectionModes::wxGridSelectNone);

	// Disable cells borders
	EnableGridLines(false);

	// Set up cells size
	SetDefaultRowSize(CELL_SIZE, true);
	SetDefaultColSize(CELL_SIZE, true);

	// Make all cells read-only for user
	EnableEditing(false);
	DisableDragColMove();
	DisableDragGridSize();

	// Set cells borders color
	SetGridLineColour(BLACK);

	// Hide grid labels
	HideColLabels();
	HideRowLabels();

	// Set {0, 0} cell observed
	observed_cell = world->get_cell(0, 0);
	if (observed_cell->ContainsEntity())
	{
		observed_entity = *(observed_cell->GetEntity());
	}
}

void world_canvas::tick()
{
	paintNow();
}

void world_canvas::paintNow()
{
	wxClientDC dc(this);
	render(dc);
}

void world_canvas::paintEvent(wxPaintEvent& evt)
{
	wxPaintDC dc(this);
	render(dc);
}

void world_canvas::change_world(logic::World* w)
{
	world = w;
}

Color world_canvas::ObtainEntityColor(pool<Entity>::const_iterator entity) const
{
	switch (mode)
	{
	case Mode::terrain:
		return ENTITY_COLOR;
	case Mode::organic:
		return ORGANIC_COLOR;
	case Mode::ration:
		return entity->GetGenome().Ration();
	case Mode::species:
		return entity->GetGenome().Species();
	case Mode::energy:
	{
		if (entity->Energy() < (entity->MaxEnergy() / 2))
		{
			return { 
				255, 
				static_cast<unsigned char>(
					255 * (entity->Energy() / (double)(entity->MaxEnergy() / 2))
				),
				0 
			};
		}
		else
		{
			return { 
				static_cast<unsigned char>(
					255 - 255 * (entity->Energy() / (double)entity->MaxEnergy())
				),
				255,
				0 
			};
		}
	}
	case Mode::age:
	{
		unsigned char c = static_cast<unsigned char>(
			255 - 255 * ((double)entity->Age() / entity->MaxAge())
		);
		return { c, c, c };
	}
	case Mode::success:
	{
		switch (world->SuccessRule(entity))
		{
		case Coefficient::reduce:
			return RED;
		case Coefficient::unchanged:
			return YELLOW;
		case Coefficient::enlarge:
			return GREEN;
		default:
			return BLACK;
		}
	}
	case Mode::generations:
	{
		unsigned char c = static_cast<unsigned char>(
			255 * ((double)entity->GetGenome().Generation()) / world->MaxGeneration()
		);
		return { c, c, c }; 
	}
	default:
		return BLACK;
	}
}

Color world_canvas::ObtainColor(const cell* cell) const
{
	if (cell->ContainsEntity())
	{
		return ObtainEntityColor(cell->GetEntity());
	}
	else if (cell->ContainsOrganic())
	{
		return	ORGANIC_COLOR;
	}

	return BLUE;
}

void world_canvas::on_select_cell(wxGridEvent& evt)
{
	const int x = evt.GetCol();
	const int y = evt.GetRow();
	observed_cell = world->get_cell(x, y);
	if (observed_cell->ContainsEntity())
	{
		observed_entity = *(observed_cell->GetEntity());
	}
}

void world_canvas::render(wxDC& dc)
{
	// Clear the grid buffers
	ClearGrid();
	for (int i = 0; i < WORLD_HEIGHT; i++)
	{
		for (int j = 0; j < WORLD_WIDTH; j++)
		{
			// Clear backgraound
			SetCellBackgroundColour(i, j, WHITE);
			// Remove outlined cells
			auto attr = GetCellAttr({ i, j });
			attr->SetRenderer(GetDefaultRenderer());
		}
	}

	// Draw the organic
	auto organic = world->Organics().begin();
	while (organic != world->Organics().end())
	{
		auto x = organic->x();
		auto y = organic->y();
		SetCellBackgroundColour(y, x, ORGANIC_COLOR);
		organic++;
	}

	if (mode == Mode::organic)
	{
		// For "Show Organic" mode this is enought, so we can exit
		Render(dc);
		return;
	}
	
	// Draw the entities
	auto entity = world->Entities().begin();
	while (entity != world->Entities().end())
	{
		auto x = entity->x();
		auto y = entity->y();
		auto color = ObtainEntityColor(entity);
		SetCellBackgroundColour(y, x, color);
		auto attr = GetCellAttr({ y, x });
		attr->SetRenderer(new OutlinedCellRenderer());
		entity++;
	}

	Render(dc);
}

size_t world_canvas::GetMaxGeneration()
{
	return world->MaxGeneration();
}

void world_canvas::ScaleCellSize(unsigned int new_size)
{
	CELL_SIZE = new_size;
	SetDefaultRowSize(CELL_SIZE, true);
	SetDefaultColSize(CELL_SIZE, true);
}

size_t world_canvas::GetEntitiesCount()
{
	return world->Entities().size();
}
world_canvas::Mode world_canvas::GetMode()
{
	return mode;
}
void world_canvas::SetMode(Mode new_val)
{
	mode = new_val;
}
const cell* world_canvas::get_observed_cell() const
{
	return observed_cell;
}
const Entity* world_canvas::get_observed_entity() const
{
	return observed_entity;
}

