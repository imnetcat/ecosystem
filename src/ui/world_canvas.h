#pragma once
#define _CRT_SECURE_NO_WARNINGS // For wxWidgets lib
#include "../logic/world.h"
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
				int pos_x, 
				int pos_y, 
				int size_x, 
				int size_y, 
				wxFrame* parent,
				logic::World* world
			);
			
			void render(wxDC& dc);
			void paintEvent(wxPaintEvent& evt);
			void paintNow();

			void tick();

			void change_world(logic::World* world);

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

			void ScaleCellSize(float scale);
			unsigned int Width();
			unsigned int Height();
			size_t GetMaxGeneration();
			size_t GetEntitiesCount();

			const logic::cell* GetCell(int x_px, int y_px);

			logic::World* world;

		private:

			const int OUTLINE = 1;

			static const int cell_default_size = 10;
			int cell_size = cell_default_size;
			int cell_outline = cell_size + OUTLINE;
			int map_width = world->Width() * cell_outline + 1;
			int map_height = world->Height() * cell_outline + 1;

			Mode mode = Mode::terrain;
		};
	}
}