#pragma once
#define _CRT_SECURE_NO_WARNINGS // For wxWidgets lib
#include "../logic/world.h"
#include "../logic/cell.h"
#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/sizer.h>

namespace ecosystem
{
	namespace ui
	{
		class world_canvas : public wxGrid
		{
		public:

			world_canvas(
				wxWindow* parent,
				logic::World* world
			);
			
			void render(wxDC& dc);
			void paintEvent(wxPaintEvent& evt);
			void on_select_cell(wxGridEvent& evt);
			void paintNow();

			void tick();

			void change_world(logic::World* world);

			const logic::cell* get_observed_cell() const;
			const logic::Entity* get_observed_entity() const;

			// some useful events
			/*
			 void mouseMoved(wxMouseEvent& event);
			 void mouseDown(wxMouseEvent& event);
			 void mouseWheelMoved(wxMouseEvent& event);
			 void mouseReleased(wxMouseEvent& event);
			 void rightClick(wxMouseEvent& event);
			 void mouseLeftWindow(wxMouseEvent& event);
			 void keyPressed(wxKeyEvent& event);
			 void keyReleased(wxKeyEvent& event);
			 */

			DECLARE_EVENT_TABLE()

		public:

			enum class Mode
			{
				terrain,
				organic,
				ration,
				energy,
				species,
				age,
				success,
				generations
			};


			logic::Color ObtainEntityColor(pool<logic::Entity>::const_iterator entity) const;
			logic::Color ObtainColor(const logic::cell* cell) const;

			Mode GetMode();
			void SetMode(Mode new_val);

			void ScaleCellSize(unsigned int new_size);
			size_t GetMaxGeneration();
			size_t GetEntitiesCount();

			logic::World* world;

			const logic::cell* observed_cell;
			const logic::Entity* observed_entity;

			Mode mode = Mode::terrain;
		};
	}
}