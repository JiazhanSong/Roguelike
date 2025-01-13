#include "precompiled_header.h"

int Experience::GetLevel()
{
	return _it - _intervals.begin() + 1;
}

int Experience::GetNextExperienceThreshold()
{
	if (_it == _intervals.end()) return _intervals.back();

	return *_it;
}

PlayerExperience::PlayerExperience(Actor* owner)
{
	_intervals = std::vector<int>{ 4, 8, 16, 32, 64 };
	_it = _intervals.begin();
	_owner = owner;
}

void PlayerExperience::Gain(int experience)
{
	_exp += experience;

	while (_exp >= *_it && _it < _intervals.end())
	{
		_exp -= *_it; // reset exp
		_it++;

		_owner->_attacker->_power = std::ceil(_owner->_attacker->_power * 1.3f);

		auto hpGain = std::ceil(_owner->_destructible->_maxHp * 0.3f);
		_owner->_destructible->_hp += hpGain;
		_owner->_destructible->_maxHp += hpGain;

		kEngine._gui->AppendMessage(TCODColor::gold, "You have leveled up!");
	}
}