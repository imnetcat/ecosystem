#pragma once
#include <TGUI/TGUI.hpp>
#include <vector>
#include <string>
#include <set>
namespace Ecosystem
{
	namespace UI
	{
		class gui : public tgui::GuiSFML
		{
		private:
			std::set<tgui::String> menus;
			tgui::MenuBar::Ptr menubar_ = nullptr;
			const int LABEL_TEXT_SIZE = 13;
		public:

			const int MENUBAR_HEIGHT = 22;

			gui(sf::RenderWindow& rw)
				: tgui::GuiSFML(rw)
			{};


			template <typename Callback>
			void menubar(const std::vector<tgui::String>& items, Callback callback)
			{
				if (!items.size())
				{
					return;
				}

				if (!menubar_)
				{
					addMenuBar();
				}

				if (!menus.count(items[0]))
				{
					menus.insert(items[0]);
					menubar_->addMenuItem(items);
				}
				else
				{
					menubar_->addMenuItem(items, false);
				}

				menubar_->connectMenuItem(items, callback);
			}
		private:
			void addMenuBar()
			{
				menubar_ = tgui::MenuBar::create();
				menubar_->setHeight(MENUBAR_HEIGHT);
				this->add(menubar_);
			}
			void DisableMenuItem(const tgui::String& item)
			{
				menubar_->setMenuEnabled(item, false);
			}
		};
	}
}