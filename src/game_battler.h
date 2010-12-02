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

#ifndef _GAME_BATTLER_H_
#define _GAME_BATTLER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <string>
#include <vector>

class Game_Actor;

////////////////////////////////////////////////////////////
/// Game_Battler class
////////////////////////////////////////////////////////////
class Game_Battler {
public:
	////////////////////////////////////////////////////////
	/// Constructor.
	////////////////////////////////////////////////////////
	Game_Battler();

	////////////////////////////////////////////////////////
	/// Get if battler has a state.
	/// @param state_id : database state id
	/// @return whether the battler has the state
	////////////////////////////////////////////////////////
	bool HasState(int state_id) const;

	/// @return Vector containing the ids of all states the battler has
	std::vector<int> GetStates() const;

	/// @return current hp
	int GetHp() const;

	/// @return current sp
	int GetSp() const;

	////////////////////////////////////////////////////////
	/// Sets the maximum HP.
	/// @param _maxhp : max hp to set
	////////////////////////////////////////////////////////
	void SetMaxHp(int _maxhp);

	/// @return current maxhp
	int GetMaxHp();

	////////////////////////////////////////////////////////
	/// Sets the maximum SP.
	/// @param _maxsp : max sp to set
	////////////////////////////////////////////////////////
	void SetMaxSp(int _maxsp);

	/// @return current maxsp
	int GetMaxSp();

	////////////////////////////////////////////////////////
	/// Sets the attack.
	/// @param _atk : attack to set
	////////////////////////////////////////////////////////
	void SetAtk(int _atk);

	////////////////////////////////////////////////////////
	/// Sets the defense.
	/// @param _def : defense to set
	////////////////////////////////////////////////////////
	void SetDef(int _def);

	////////////////////////////////////////////////////////
	/// Sets the spirit.
	/// @param _spi : spirit to set
	////////////////////////////////////////////////////////
	void SetSpi(int _spi);

	////////////////////////////////////////////////////////
	/// Sets the agility.
	/// @param _agi : agility to set
	////////////////////////////////////////////////////////
	void SetAgi(int _agi);

	/// @return current atk
	int GetAtk();

	/// @return current def
	int GetDef();

	/// @return current spi
	int GetSpi();

	/// @return current agi
	int GetAgi();

protected:
	std::string battler_name;
	int hp;
	int sp;
	int maxhp_plus;
	int maxsp_plus;
	int atk_plus;
	int def_plus;
	int spi_plus;
	int agi_plus;
	std::vector<int> states;
	bool hidden;
	bool immortal;
	bool damage_pop;
	int damage;
	bool critical;
	int animation_id;
	bool animation_hit;
	bool white_flash;
	bool blink;

	void SetGameActor(Game_Actor* _actor);

private:
	// Associated game actor, if any
	Game_Actor* actor;
};

#endif