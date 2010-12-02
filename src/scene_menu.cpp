/////////////////////////////////////////////////////////////////////////////
// This file is part of EasyRPG Player.
//
// EasyRPG Player is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// EasyRPG Player is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
/////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "scene_menu.h"
#include "audio.h"
#include "graphics.h"
#include "game_party.h"
#include "game_system.h"
#include "input.h"
#include "player.h"
#include "scene_equip.h"
#include "scene_map.h"

////////////////////////////////////////////////////////////
Scene_Menu::Scene_Menu(int menu_index) :
	menu_index(menu_index) {
	type = Scene::Menu;
}

////////////////////////////////////////////////////////////
void Scene_Menu::MainFunction() {
	// Create Options Window
	std::vector<std::string> options;
	options.push_back(Data::terms.command_item);
	options.push_back(Data::terms.command_skill);
	options.push_back(Data::terms.menu_equipment);
	options.push_back(Data::terms.menu_save);
	options.push_back(Data::terms.menu_quit);

	command_window = new Window_Command(88, options);
	command_window->SetIndex(menu_index);

	// If there are no actors in the party disable Skills and Equipment
	// RPG2k does not do this, but crashes if you try to access these menus
	if (Game_Party::GetActors().empty()) {
		command_window->DisableItem(0);
		command_window->DisableItem(1);
		command_window->DisableItem(2);
	}

	// If save is forbidden disable this item
	if (Game_System::save_disabled) {
		command_window->DisableItem(3);
	}

	// Gold Window
	gold_window = new Window_Gold();
	gold_window->SetX(0);
	gold_window->SetY(208);

	// Status Window
	menustatus_window = new Window_MenuStatus();
	menustatus_window->SetX(88);
	menustatus_window->SetY(0);
	menustatus_window->SetActive(false);

	Graphics::Transition(Graphics::FadeIn, 20, false);

	while (instance == this) {
		Player::Update();
		Graphics::Update();
		Input::Update();
		Update();
	}

	Graphics::Transition(Graphics::FadeOut, 20, false);

	Scene::old_instance = this;
}

////////////////////////////////////////////////////////////
Scene_Menu::~Scene_Menu() {
	delete command_window;
	delete gold_window;
	delete menustatus_window;
}

////////////////////////////////////////////////////////////
void Scene_Menu::Update() {
	command_window->Update();
	gold_window->Update();
	menustatus_window->Update();

	if (command_window->GetActive()) {
		UpdateCommand();
	}
	else if (menustatus_window->GetActive()) {
		UpdateStatus();
	}
}

////////////////////////////////////////////////////////////
void Scene_Menu::UpdateCommand() {
	if (Input::IsTriggered(Input::CANCEL)) {
		Game_System::SePlay(Data::system.cancel_se);
		Scene::instance = new Scene_Map();
	} else if (Input::IsTriggered(Input::DECISION)) {
		switch (command_window->GetIndex()) {
		case 0: // Item
			Game_System::SePlay(Data::system.decision_se);
			break;
		case 1: // Tech Skill
		case 2: // Equipment
			Game_System::SePlay(Data::system.decision_se);
			command_window->SetActive(false);
			menustatus_window->SetActive(true);
			menustatus_window->SetIndex(0);
			break;
		case 3: // Save
			Game_System::SePlay(Data::system.decision_se);
			break;
		case 4: // Quit Game
			Game_System::SePlay(Data::system.decision_se);
			Audio::BGS_Fade(800);
			type = Scene::Null;
			instance = NULL;
			break;
		}
	}
}

////////////////////////////////////////////////////////////
void Scene_Menu::UpdateStatus() {
	if (Input::IsTriggered(Input::CANCEL)) {
		Game_System::SePlay(Data::system.cancel_se);
		command_window->SetActive(true);
		menustatus_window->SetActive(false);
		menustatus_window->SetIndex(-1);
	} else if (Input::IsTriggered(Input::DECISION)) {
		Game_System::SePlay(Data::system.decision_se);
		switch (command_window->GetIndex()) {
		case 1: // Tech Skill
			break;
		case 2: // Equipment
			Scene::instance = new Scene_Equip(menustatus_window->GetIndex());
			break;
		}
	}
}