#pragma once
#define _CRT_SECURE_NO_WARNINGS // For wxWidgets lib
#include <wx/wxprec.h>

#include "../logic/world.h"
#include "world_canvas.h"

namespace ecosystem
{
	namespace ui
	{
        class Form : public wxFrame
        {
        public:
            Form();
            ~Form();
            void onIdle(wxIdleEvent& evt);
            DECLARE_EVENT_TABLE()
        private:
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

            logic::World* world;
            world_canvas* canvas;
        };

	}
}