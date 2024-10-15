/**
* @brief Action header file to store action from logfile
*
* @author Matus Vrablik(xvrabl05)
*
*/
#pragma once
#include <string>
#include <tuple>

class Action {
public:
	Action(bool openDoor);
	Action(int keyX, int keyY);
	Action(int hpFrom, int hpTo, bool player);
	Action(int fromX, int fromY, int toX, int toY, bool player);
	~Action();

	std::string getType();

	std::tuple<int, int> moveFrom;
	std::tuple<int, int> moveTo;
	bool player;
	int hpFrom;
	int hpTo;
	std::tuple<int, int> key;
	std::string type;
	bool door;
private:
	void valueFill();
};