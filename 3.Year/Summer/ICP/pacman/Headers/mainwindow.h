/**
* @brief Header file for main window of application
*
* @author Matus Vrablik(xvrabl05)
*
*/
#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QGraphicsRectItem>
#include <QPixmap>
#include <QFileDialog>
#include <QThread>
#include <QKeyEvent>
#include <QTimer>
#include <QColor>
#include "ui_mainwindow.h"
#include "GameInterface.h"
#include "ReplayMode.h"	
class ReplayMode;
using namespace std;
class mainwindow : public QMainWindow
{
	Q_OBJECT

public:
	mainwindow(QWidget* parent = nullptr);
	~mainwindow();
	void updateMap(std::tuple<int, int> from, std::tuple<int, int>to);
	void updateLives();
	void updateKeys();
	void updateEndGame();
	void setEndScreen(bool win);
	void setPauseScreen();
	void clearAfterGame();

	void replayStep(int step);
	void replayLive(int prev, int curr);
	void replayGhostMove(int prevRow, int prevCol, int nextRow, int nextCol);
	void replayPacmanMove(int prevRow, int prevCol, int nextRow, int nextCol);
	void replayKey(int Row, int Col);
	void replayDoor(bool open);

private slots:
	void newGameButtonClicked();
	void loadGameLogButtonClicked();
	void exitButtonClicked();
	void loadMapButtonClicked();
	void playMapButtonClicked();
	void backToMainMenuButtonClicked();
	void replayStartButtonClicked();
	void replayEndButtonClicked();
	void processKeyPressEvent();
	void moveGhosts();
	void replayTurn();

private:
	Ui::mainwindowClass* ui;

	void playGame();
	bool updatePlayerItem();
	void updateGhostItems();
	void updateLiveItems();
	void updateDoorItem();
	void updateKeyItems(tuple<int, int>);
	void updateKeyScoreItems();
	void updateSteps();
	void startReplay();

	QPixmap wall;
	QPixmap player;
	QPixmap playerEmpty;
	QPixmap ghost;
	QPixmap doorOpen;
	QPixmap doorClosed;
	QPixmap key;
	QPixmap keyEmpty;
	map<tuple<int, int>, QGraphicsItem*> wallItems;
	QGraphicsRectItem* playerItem;
	multimap<tuple<int, int>, QGraphicsItem*> ghostItems;
	QGraphicsRectItem* doorItem;
	map<tuple<int, int>, QGraphicsItem*> keyItems;
	list<QGraphicsRectItem*> liveItems;
	list<QGraphicsRectItem*> keyScoreItems;

	QGraphicsTextItem* stepsItem;
	int steps = 0;

	QTimer keyPressTimer;
	int pendingKey;
	bool pause;
	QTimer moveGhostsTimer;
	QTimer replayTimer;
	bool forward;

	ReplayMode* replayer;
	QString file;

protected:
	bool eventFilter(QObject* obj, QEvent* event);
	GameInterface* gameInterface;
};
