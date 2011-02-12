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
#include "data.h"
#include "color.h"
#include "cache.h"
#include "surface.h"
#include "input.h"
#include "game_system.h"
#include "window_battlecommand.h"

////////////////////////////////////////////////////////////
Window_BattleCommand::Window_BattleCommand(int x, int y, int width, int height,
										   const std::vector<std::string>& commands) :
	Window_Base(x, y, width, height), commands(commands) {

	num_commands = commands.size();
	disabled.resize(commands.size());
	index = -1;
	top_row = 0;
	cycle = 0;

	SetContents(Surface::CreateSurface(width - 16, height - 16));
	contents->SetTransparentColor(windowskin->GetTransparentColor());

	num_rows = contents->GetHeight() / 16;

	Refresh();
}

////////////////////////////////////////////////////////////
void Window_BattleCommand::SetCommands(const std::vector<std::string>& _commands) {
	commands = _commands;
	num_commands = commands.size();
	disabled.resize(num_commands);
	Refresh();
}

////////////////////////////////////////////////////////////
void Window_BattleCommand::SetEnabled(int index, bool enabled) {
	disabled[index] = enabled;
	Refresh();
}

////////////////////////////////////////////////////////////
void Window_BattleCommand::SetActive(bool active) {
	index = active ? 0 : -1;
	top_row = 0;
	Window::SetActive(active);
	Refresh();
}

////////////////////////////////////////////////////////////
void Window_BattleCommand::Update() {
	Window_Base::Update();

	int old_index = index;
	if (active && num_commands >= 0 && index >= 0) {
		if (Input::IsRepeated(Input::DOWN)) {
			Game_System::SePlay(Data::system.cursor_se);
			index++;
		}

		if (Input::IsRepeated(Input::UP)) {
			Game_System::SePlay(Data::system.cursor_se);
			index--;
		}

		index += num_commands;
		index %= num_commands;

		if (index < top_row)
			top_row = index;
		if (index > top_row + num_rows - 1)
			top_row = index - num_rows + 1;

		cycle++;
		if (cycle % 20 == 0 || old_index != index)
			Refresh();
	}

	UpdateCursorRect();
}

////////////////////////////////////////////////////////////
void Window_BattleCommand::UpdateCursorRect() {
	if (index >= 0)
		SetCursorRect(Rect(0, (index - top_row) * 16, contents->GetWidth(), 16));
	else
		SetCursorRect(Rect());
}

////////////////////////////////////////////////////////////
void Window_BattleCommand::Refresh() {
	contents->Clear();
	for (int i = 0; i < num_commands; i++) {
		Font::SystemColor color = disabled[i] ? Font::ColorDisabled : Font::ColorDefault;
		DrawItem(i, color);
	}

	SetUpArrow(false);
	SetDownArrow(false);
	if (active && (cycle / 20) % 2 == 0) {
		if (top_row > 0)
			SetUpArrow(true);
		if (top_row + num_rows < (int) num_commands)
			SetDownArrow(true);
	}
}

////////////////////////////////////////////////////////////
void Window_BattleCommand::DrawItem(int index, Font::SystemColor color) {
	int y = 16 * (index - top_row);
	if (y < 0 || y + 16 > contents->GetHeight())
		return;
	contents->ClearRect(Rect(0, y, contents->GetWidth(), 16));
	contents->TextDraw(2, y + 2, color, commands[index]);
}

////////////////////////////////////////////////////////////
int Window_BattleCommand::GetIndex() {
	return index;
}

////////////////////////////////////////////////////////////
void Window_BattleCommand::SetIndex(int _index) {
	index = _index;
}
