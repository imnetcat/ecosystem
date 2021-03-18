#pragma once
#include "../logic/world.h"
#include <TGUI/TGUI.hpp>
namespace Ecosystem
{
	namespace UI
	{
		class Map
		{
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


			Map(Logic::World* world);

			void Draw(tgui::Canvas::Ptr canvas);
			Logic::Color ObtainEntityColor(pool<Logic::Entity>::const_iterator entity) const;
			Logic::Color ObtainColor(const Logic::cell* cell) const;

			Mode GetMode();
			void SetMode(Mode new_val);

			void ScaleCellSize(float scale);
			unsigned int Width();
			unsigned int Height();
			size_t GetMaxGeneration();
			size_t GetEntitiesCount();

			const Logic::cell* GetCell(size_t x_px, size_t y_px);

		private:

			Logic::World* world;

			const short OUTLINE = 1;

			static const unsigned short cell_default_size = 10;
			unsigned short cell_size = cell_default_size;
			unsigned short cell_outline = cell_size + OUTLINE;
			unsigned int map_width = world->Width() * cell_outline + 1;
			unsigned int map_height = world->Height() * cell_outline + 1;

			Mode mode = Mode::terrain;
		};
	}
}