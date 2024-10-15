/**
* @brief Action form logfile
*
* @author	Matus Vrablik(xvrabl05)
*
*/
#include "Action.h"

Action::Action(int fromX, int fromY, int toX, int toY, bool player) {
	valueFill();
	this->moveFrom = std::tuple(fromX, fromY);
	this->moveTo = std::tuple(toX, toY);
	this->player = player;
	this->type = "move";
}

Action::Action(int hpFrom, int hpTo, bool player) {
	valueFill();
	this->hpFrom = hpFrom;
	this->hpTo = hpTo;
	this->type = "hp";
}

Action::Action(int keyX, int keyY) {
	valueFill();
	this->key = std::tuple(keyX, keyY);
	this->type = "key";
}

Action::Action(bool openDoor) {
	valueFill();
	this->door = true;
	this->type = "door";
}

void Action::valueFill() {
	this->moveFrom = std::tuple(-1, -1);
	this->moveTo = std::tuple(-1, -1);
	this->player = false;
	this->hpFrom = -1;
	this->hpTo = -1;
	this->key = std::tuple(-1, -1);
	this->type = "none";
	this->door = false;
}

Action::~Action() {
}

std::string Action::getType() {
	if (this->door) return "door";
	if (std::get<0>(this->key) >= 0) return "key";
	if (this->hpFrom >= 0) return "hp";
	if (this->player) return "movePlayer";
	if (std::get<0>(this->moveFrom) >= 0) return "moveGhost";
	return "none";
}
