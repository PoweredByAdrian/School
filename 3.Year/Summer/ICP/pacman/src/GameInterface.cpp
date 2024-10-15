/**
 * @brief Game interface holding most of the game logic contained from FrontEnd visual app.
 *
 * @author Matúš Vráblik (xvrabl05)
 */

#include "GameInterface.h"

GameInterface::GameInterface(mainwindow* window)
{
	this->logger = new Logger();
	config = new MazeConfigure(this->logger);
	maze = NULL;
	maxLives = -1;
	lives = -1;
	maxKeys = -1;
	player = tuple(-1, -1);
	door = tuple(-1, -1);
	this->walls = new list<tuple<int, int>>();
	this->ghosts = new list<tuple<int, int>>();
	this->keys = new list<tuple<int, int>>();
	this->window = window;
}

GameInterface::~GameInterface()
{
	delete this->config;
	this->config = nullptr;
	delete this->logger;
	this->logger = nullptr;
	this->window = nullptr;
	delete this->walls;
	this->walls = nullptr;
	delete this->ghosts;
	this->ghosts = nullptr;
	delete this->keys;
	this->keys = nullptr;
}

void GameInterface::loadMap(std::string file)
{
	config->loadMapFromFile(file);
	maze = config->createMaze();
	updateVariables();
}

void GameInterface::loadReplay(list<string> maze)
{
	config->startReading(maze.size(), maze.front().length());
	for (auto line : maze)
	{
		config->processLine(line);
	}
	config->stopReading();
	this->maze = config->createMaze();
	updateVariables();
	this->logger->erase();
	delete this->logger;
	this->logger = new Logger();
}

void GameInterface::loadMap()
{
	config->startReading(4, 3);
	config->processLine(".GG");
	config->processLine(".XT");
	config->processLine("KX.");
	config->processLine(".S.");
	config->stopReading();

	maze = config->createMaze();
	updateVariables();
}

void GameInterface::startGame()
{
	for (MazeObject* ghost : maze->ghosts())
	{
		ghost->start();
	}
}

void GameInterface::endGame()
{
	delete this->logger;
	this->logger = new Logger();
	delete this->config;
	this->config = new MazeConfigure(this->logger);
	delete this->ghosts;
	this->ghosts = new list<tuple<int, int>>();
	delete this->walls;
	this->walls = new list<tuple<int, int>>();
	delete this->keys;
	this->keys = new list<tuple<int, int>>();
}

int GameInterface::getLives()
{
	return this->lives;
}

int GameInterface::getMaxLives()
{
	return this->maxLives;
}

int GameInterface::getMaxKeys()
{
	return this->maxKeys;
}

tuple<int, int> GameInterface::getMapSize()
{
	return tuple<int, int>(maze->numRows(), this->maze->numCols());
}

list<tuple<int, int>> GameInterface::getWalls()
{
	return *this->walls;
}

list<tuple<int, int>> GameInterface::getGhosts()
{
	return *this->ghosts;
}

list<tuple<int, int>> GameInterface::getKeys()
{
	if (this->keys->empty()) { return list<tuple<int, int>>(); }
	return *this->keys;
}

tuple<int, int> GameInterface::getPlayer()
{
	return this->player;
}

tuple<int, int> GameInterface::getDoor()
{
	return this->door;
}

bool GameInterface::isDoorOpen()
{
	return this->doorOpen;
}

void GameInterface::movePlayer(int d)
{
	PathField* field = (PathField*)this->maze->getField(std::get<0>(this->player), std::get<1>(this->player));
	if (!field->fieldObjectList->empty())
	{
		for (MazeObject* obj : *field->fieldObjectList)
		{
			PacmanObject* player = (PacmanObject*)obj;
			if (typeid(*player) == typeid(PacmanObject))
			{
				player->move(Field::Direction(d));
				break;
			}
		}
	}
}

void GameInterface::notifyMove(int fromX, int fromY, int toX, int toY)
{
	updateVariables();
	this->window->updateMap(tuple(fromX, fromY), tuple(toX, toY));
}

void GameInterface::notifyPickKey(int x, int y)
{
	this->keys->remove(tuple(x, y));
	PathField* field = (PathField*)this->maze->getField(std::get<0>(this->door), std::get<1>(this->door));
	if (!field->fieldObjectList->empty())
	{
		for (MazeObject* obj : *field->fieldObjectList)
		{
			if (typeid(*obj) == typeid(DoorObject))
			{
				obj->start();
				break;
			}
		}
	}
	this->window->updateKeys();
	this->window->updateMap(tuple(x, y), tuple(-1, -1));
}

void GameInterface::notifyOpenDoors(int x, int y)
{
	this->doorOpen = true;
	this->window->updateMap(tuple(x, y), tuple(x, y));
}

void GameInterface::notifyLives()
{
	PathField* field = (PathField*)this->maze->getField(std::get<0>(this->player), std::get<1>(this->player));
	if (!field->fieldObjectList->empty())
	{
		for (MazeObject* obj : *field->fieldObjectList)
		{
			PacmanObject* player = (PacmanObject*)obj;
			if (typeid(*player) == typeid(PacmanObject))
			{
				this->lives = obj->getLives();
				break;
			}
		}
	}
	this->window->updateLives();
}

void GameInterface::notifyEndLevel()
{
	this->endGame();
	this->window->setEndScreen(true);
	this->window->updateEndGame();

}

void GameInterface::notifyGameOver()
{
	this->endGame();
	this->window->setEndScreen(false);
	this->window->updateEndGame();
}

void GameInterface::updateVariables()
{
	int walls = this->walls->empty();
	if (!this->ghosts->empty())
	{
		this->ghosts->clear();
	}
	if (!this->keys->empty())
	{
		this->keys->clear();
	}
	for (int x = 0; x < maze->numRows(); x++)
	{
		for (int y = 0; y < maze->numCols(); y++)
		{
			auto field = maze->getField(x, y);
			//walls
			if (walls && (field == nullptr || field->getType() == 'X'))
			{
				this->walls->push_back(tuple(x, y));
				continue;
			}
			if (field == nullptr || field->getType() == 'X') { continue; }

			PathField* pathField = (PathField*)field;
			for (MazeObject* obj : *pathField->getObjectList())
			{
				GhostObject* ghost = (GhostObject*)obj;
				PacmanObject* player = (PacmanObject*)obj;
				DoorObject* Door = (DoorObject*)obj;
				KeyObject* Key = (KeyObject*)obj;
				//ghosts
				if (typeid(*ghost) == typeid(GhostObject))
				{
					field->get()->attach(this);
					field->get()->setLogger(this->logger);
					this->ghosts->push_back(tuple(x, y));
					continue;
				}
				//player
				if (typeid(*player) == typeid(PacmanObject))
				{
					field->get()->attach(this);
					field->get()->setLogger(this->logger);
					this->player = tuple(x, y);
					this->lives = field->get()->getLives();
					if (this->maxLives < 0) { this->maxLives = this->lives; }
					continue;
				}
				//door
				if (this->door == tuple(-1, -1) && typeid(*Door) == typeid(DoorObject))
				{
					field->get()->attach(this);
					field->get()->setLogger(this->logger);
					this->door = tuple(x, y);
					continue;
				}
				//keys
				if (typeid(*Key) == typeid(KeyObject))
				{
					field->get()->attach(this);
					field->get()->setLogger(this->logger);
					this->keys->push_back(tuple(x, y));
					continue;
				}
			}
		}
	}
	if (maxKeys == -1)
	{
		this->maze->getField(std::get<0>(this->door), std::get<1>(this->door))->get()->start();
		maxKeys = keys->size();
	}

}
