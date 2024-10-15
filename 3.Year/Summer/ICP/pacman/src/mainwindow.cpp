/**
 * @brief MainWindow class file
 *
 * @author Matus Vrablik (xvrabl05)
 *
 */

#include "mainwindow.h"

#define FIELDSIZE 200

mainwindow::mainwindow(QWidget* parent)
	: QMainWindow(parent)
{
	ui = new Ui::mainwindowClass();
	ui->setupUi(this);
	qApp->installEventFilter(this);
	connect(&keyPressTimer, &QTimer::timeout, this, &mainwindow::processKeyPressEvent);
	connect(&moveGhostsTimer, &QTimer::timeout, this, &mainwindow::moveGhosts);
	connect(&replayTimer, &QTimer::timeout, this, &mainwindow::replayTurn);

	ui->mainMenuWidget->setVisible(true);
	ui->newGameWidget->setVisible(false);
	ui->gameWidget->setVisible(false);
	ui->resultWidget->setVisible(false);
	ui->replayWidget->setVisible(false);
	ui->gameWidget->setFocusPolicy(Qt::NoFocus);
	ui->newGameWidget->setFocusPolicy(Qt::NoFocus);
	ui->mainMenuWidget->setFocusPolicy(Qt::NoFocus);
	ui->gameScorePane->setFocusPolicy(Qt::NoFocus);
	ui->gameGuidePane->setFocusPolicy(Qt::NoFocus);
	ui->resultWidget->setFocusPolicy(Qt::NoFocus);

	// mainMenuWidget
	connect(ui->exitButton, SIGNAL(clicked()), this, SLOT(exitButtonClicked()));
	connect(ui->newGameButton, SIGNAL(clicked()), this, SLOT(newGameButtonClicked()));
	connect(ui->loadGameLogButton, SIGNAL(clicked()), this, SLOT(loadGameLogButtonClicked()));
	connect(ui->replayStartButton, SIGNAL(clicked()), this, SLOT(replayStartButtonClicked()));
	connect(ui->replayEndButton, SIGNAL(clicked()), this, SLOT(replayEndButtonClicked()));

	// newGameWidget
	connect(ui->loadMapButton, SIGNAL(clicked()), this, SLOT(loadMapButtonClicked()));
	connect(ui->playMapButton, SIGNAL(clicked()), this, SLOT(playMapButtonClicked()));
	connect(ui->backToMainMenuButton, SIGNAL(clicked()), this, SLOT(backToMainMenuButtonClicked()));
	connect(ui->backToMainMenuButton_2, SIGNAL(clicked()), this, SLOT(backToMainMenuButtonClicked()));

	gameInterface = new GameInterface(this);
	QPixmap* map = new QPixmap(":/wall");
	wall = map->scaled(FIELDSIZE, FIELDSIZE, Qt::KeepAspectRatio);
	delete map;
	map = new QPixmap(":/player");
	player = map->scaled(FIELDSIZE, FIELDSIZE, Qt::KeepAspectRatio);
	delete map;
	map = new QPixmap(":/playerOutline");
	playerEmpty = map->scaled(FIELDSIZE, FIELDSIZE, Qt::KeepAspectRatio);
	delete map;
	map = new QPixmap(":/ghost");
	ghost = map->scaled(FIELDSIZE, FIELDSIZE, Qt::KeepAspectRatio);
	delete map;
	map = new QPixmap(":/doorOpen");
	doorOpen = map->scaled(FIELDSIZE, FIELDSIZE, Qt::KeepAspectRatio);
	delete map;
	map = new QPixmap(":/doorClosed");
	doorClosed = map->scaled(FIELDSIZE, FIELDSIZE, Qt::KeepAspectRatio);
	delete map;
	map = new QPixmap(":/key");
	key = map->scaled(FIELDSIZE, FIELDSIZE, Qt::KeepAspectRatio);
	delete map;
	map = new QPixmap(":/keyEmpty");
	keyEmpty = map->scaled(FIELDSIZE, FIELDSIZE, Qt::KeepAspectRatio);
	delete map;
	ui->gamePane->setScene(new QGraphicsScene());
	ui->gameScorePane->setScene(new QGraphicsScene());
	ui->gameScorePane->setAlignment(Qt::AlignTop);

	this->playerItem = nullptr;
	this->doorItem = nullptr;
	this->stepsItem = nullptr;
	this->pause = false;
	this->replayer = nullptr;
	this->forward = true;
}

mainwindow::~mainwindow() {
	delete gameInterface;
	delete ui;
	for (auto& wall : wallItems) {
		delete wall.second;
	}
	for (auto& ghost : ghostItems) {
		delete ghost.second;
	}
	for (auto& key : keyItems) {
		delete key.second;
	}
	for (auto& live : liveItems) {
		delete live;
	}
	for (auto& key : keyScoreItems) {
		delete key;
	}
	wallItems.clear();
	ghostItems.clear();
	keyItems.clear();
	liveItems.clear();
	keyScoreItems.clear();
	delete this->playerItem;
	delete this->doorItem;
	this->playerItem = nullptr;
	this->doorItem = nullptr;
	keyPressTimer.stop();
	moveGhostsTimer.stop();
}

void mainwindow::updateMap(tuple<int, int> from, tuple<int, int> to)
{
	updateGhostItems();
	updatePlayerItem();
	updateDoorItem();
	if (tuple(-1, -1) == to) { updateKeyItems(from); }
	ui->gamePane->update();
}

void mainwindow::updateLives()
{
	updateLiveItems();
	ui->gameScorePane->update();
}

void mainwindow::updateKeys()
{
	updateKeyScoreItems();
	ui->gameScorePane->update();
}


void mainwindow::newGameButtonClicked()
{
	ui->mainMenuWidget->setVisible(false);
	ui->newGameWidget->setVisible(true);
}

void mainwindow::replayStep(int step)
{
	this->steps = step;
	this->updateSteps();
}

void mainwindow::replayLive(int prev, int curr)
{
	int count = 1;
	for (auto& live : this->liveItems)
	{
		if (count <= curr)
		{
			live->setBrush(QBrush(player));
		}
		else {
			live->setBrush(QBrush(playerEmpty));
		}
		count++;
	}
}

void mainwindow::replayGhostMove(int prevRow, int prevCol, int nextRow, int nextCol)
{
	auto ghost = this->ghostItems.find(tuple(prevCol, prevRow));
	QGraphicsItem* tmp = ghost->second;
	tmp->setPos(nextCol * FIELDSIZE, nextRow * FIELDSIZE);
	this->ghostItems.erase(ghost->first);
	this->ghostItems.insert(std::make_pair(tuple(nextCol, nextRow), tmp));
	ui->gamePane->update();
}

void mainwindow::replayPacmanMove(int prevRow, int prevCol, int nextRow, int nextCol)
{
	this->playerItem->setPos(nextCol * FIELDSIZE, nextRow * FIELDSIZE);
	ui->gamePane->update();
}

void mainwindow::replayKey(int Row, int Col)
{
	auto it = this->keyItems.find(tuple(Col, Row));
	bool found = true;
	if (it == this->keyItems.end())
	{
		found = false;
		QGraphicsItem* keyItem = ui->gamePane->scene()->addRect(QRectF(Row * FIELDSIZE, Col * FIELDSIZE, FIELDSIZE, FIELDSIZE), Qt::NoPen, QBrush(this->key));
		keyItem->setZValue(0);
		this->keyItems[tuple(Col, Row)] = keyItem;
		ui->gamePane->update();
	}

	if (found)
	{
		ui->gamePane->scene()->removeItem(it->second);
		this->keyItems.erase(tuple(Col, Row));
	}

	int count = gameInterface->getMaxKeys();
	for (auto& key : this->keyScoreItems)
	{
		if (count > this->keyItems.size())
		{
			key->setBrush(QBrush(this->key));
		}
		else {
			key->setBrush(QBrush(keyEmpty));
		}
		count--;
	}
	ui->gameScorePane->update();
}

void mainwindow::replayDoor(bool open)
{
	if (open)
	{
		this->doorItem->setBrush(QBrush(this->doorOpen));
	}
	else
	{
		this->doorItem->setBrush(QBrush(this->doorClosed));
	}
	ui->gamePane->update();
}

void mainwindow::loadGameLogButtonClicked()
{
	this->file = QFileDialog::getOpenFileName(this, "Choose map file", "C://", "Text file (*.txt)");
	//guard no file selected
	if (this->file == NULL) { return; }
	ui->mainMenuWidget->setVisible(false);
	ui->replayWidget->setVisible(true);
}

void mainwindow::exitButtonClicked()
{
	this->close();
}

void mainwindow::loadMapButtonClicked()
{
	QString file = QFileDialog::getOpenFileName(this, "Choose map file", "C://", "Text file (*.txt)");
	//guard no file selected
	if (file == NULL) { return; }
	gameInterface->loadMap(file.toStdString());
	this->playGame();
}

void mainwindow::playMapButtonClicked()
{
	gameInterface->loadMap();
	this->playGame();
}

void mainwindow::backToMainMenuButtonClicked()
{
	ui->newGameWidget->setVisible(false);
	ui->replayWidget->setVisible(false);
	ui->mainMenuWidget->setVisible(true);
}

void mainwindow::replayStartButtonClicked()
{
	this->forward = true;
	this->replayer = new ReplayMode(this->file.toStdString(), this, this->forward);
	ui->replayWidget->setVisible(false);
	ui->gameWidget->setVisible(true);
	this->startReplay();
}

void mainwindow::replayEndButtonClicked()
{
	this->forward = false;
	this->replayer = new ReplayMode(this->file.toStdString(), this, this->forward);
	ui->replayWidget->setVisible(false);
	ui->gameWidget->setVisible(true);
	this->startReplay();
}

void mainwindow::playGame()
{
	ui->newGameWidget->setVisible(false);
	ui->gameWidget->setVisible(true);

	//TODO - add selection from gridPane to loadMap arguments
	int z = 0;
	for (auto& wall : gameInterface->getWalls())
	{
		int x = std::get<1>(wall);
		int y = std::get<0>(wall);
		QGraphicsItem* wallItem = ui->gamePane->scene()->addRect(QRectF(x * FIELDSIZE, y * FIELDSIZE, FIELDSIZE, FIELDSIZE), QPen(), QBrush(this->wall));
		wallItem->setZValue(z);
		this->wallItems[wall] = wallItem;
	}
	auto door = gameInterface->getDoor();
	int x = std::get<1>(door);
	int y = std::get<0>(door);
	QGraphicsRectItem* doorItem = ui->gamePane->scene()->addRect(QRectF(x * FIELDSIZE, y * FIELDSIZE, FIELDSIZE, FIELDSIZE), Qt::NoPen, QBrush(this->doorClosed));
	doorItem->setZValue(z);
	this->doorItem = doorItem;
	z = 0;
	for (auto& key : gameInterface->getKeys())
	{
		int x = std::get<1>(key);
		int y = std::get<0>(key);
		QGraphicsItem* keyItem = ui->gamePane->scene()->addRect(QRectF(x * FIELDSIZE, y * FIELDSIZE, FIELDSIZE, FIELDSIZE), Qt::NoPen, QBrush(this->key));
		keyItem->setZValue(z);
		this->keyItems[key] = keyItem;
	}
	updatePlayerItem();
	updateGhostItems();

	ui->gamePane->fitInView(0, 0, ui->gamePane->scene()->width(), ui->gamePane->scene()->height(), Qt::KeepAspectRatio);
	ui->gamePane->update();
	ui->gamePane->setFocus();

	auto maxLives = gameInterface->getMaxLives();
	for (int i = 0; i < maxLives; i++)
	{
		QGraphicsRectItem* liveItem = ui->gameScorePane->scene()->addRect(QRectF(0, 0, FIELDSIZE, FIELDSIZE), Qt::NoPen, QBrush(this->player));
		liveItem->setPos(i * FIELDSIZE, 0);
		this->liveItems.push_back(liveItem);
	}
	auto maxKeys = gameInterface->getMaxKeys();
	for (int i = 0; i < maxKeys; i++)
	{
		QGraphicsRectItem* keyItem = ui->gameScorePane->scene()->addRect(QRectF(i * FIELDSIZE, FIELDSIZE, FIELDSIZE, FIELDSIZE), Qt::NoPen, QBrush(this->keyEmpty));
		keyItem->setPos(i * FIELDSIZE, 0);
		this->keyScoreItems.push_back(keyItem);
	}
	updateSteps();
	ui->gameScorePane->fitInView(0, 0, ui->gameScorePane->scene()->width(), ui->gameScorePane->scene()->height(), Qt::KeepAspectRatio);
	ui->gameScorePane->update();

	this->moveGhostsTimer.start(500);
}

void mainwindow::updateEndGame()
{
	this->keyPressTimer.stop();
	this->moveGhostsTimer.stop();

	ui->resultWidget->setVisible(true);
	ui->resultWidget->setFocus();
}

void mainwindow::setEndScreen(bool win)
{
	if (win)
	{
		ui->statusLabel->setText("You Win!");
		ui->statusLabel->setStyleSheet("background-color: rgba(0,0,0,0);color:rgba(0,150,0,255);");
	}
	else
	{
		ui->statusLabel->setText("You Lose!");
		ui->statusLabel->setStyleSheet("background-color: rgba(0,0,0,0);color:rgba(150,0,0,255);");
	}
	ui->resultWidget->setStyleSheet("background-color: rgba(20, 20, 20, 220);");
	ui->legendLabel->setStyleSheet("background-color: rgba(0,0,0,0);color:rgba(200,200,200,255);");
	ui->stepsLabel->setStyleSheet("background-color: rgba(0,0,0,0);color:rgba(200,200,200,255);");

	ui->legendLabel_5->setVisible(false);
	ui->stepsLabel->setText("Steps taken: " + QString::number(this->steps));
	ui->stepsLabel->setVisible(true);
}

void mainwindow::setPauseScreen()
{
	ui->statusLabel->setText("Paused");
	ui->statusLabel->setStyleSheet("background-color: rgba(0,0,0,0);color:rgba(200,200,200,255);");
	ui->resultWidget->setStyleSheet("background-color: rgba(20, 20, 20, 220);");
	ui->legendLabel_5->setVisible(true);
	ui->legendLabel->setStyleSheet("background-color: rgba(0,0,0,0);color:rgba(200,200,200,255);");
	ui->legendLabel_5->setStyleSheet("background-color: rgba(0,0,0,0);color:rgba(200,200,200,255);");
	ui->stepsLabel->setVisible(false);
}

void mainwindow::clearAfterGame()
{
	delete gameInterface;
	gameInterface = new GameInterface(this);
	ui->mainMenuWidget->setFocus();
	for (auto& wall : wallItems) {
		delete wall.second;
	}
	for (auto& ghost : ghostItems) {
		delete ghost.second;
	}
	for (auto& key : keyItems) {
		delete key.second;
	}
	for (auto& live : liveItems) {
		delete live;
	}
	for (auto& key : keyScoreItems) {
		delete key;
	}
	wallItems.clear();
	ghostItems.clear();
	keyItems.clear();
	liveItems.clear();
	keyScoreItems.clear();
	delete this->playerItem;
	delete this->doorItem;
	delete this->stepsItem;
	this->playerItem = nullptr;
	this->doorItem = nullptr;
	this->stepsItem = nullptr;
	this->steps = 0;
	delete ui->gamePane->scene();
	ui->gamePane->setScene(new QGraphicsScene);
	delete ui->gameScorePane->scene();
	ui->gameScorePane->setScene(new QGraphicsScene);
	delete this->replayer;
	this->replayer = nullptr;
}

bool mainwindow::updatePlayerItem()
{
	int z = 1;
	auto player = gameInterface->getPlayer();
	int x = std::get<1>(player);
	int y = std::get<0>(player);
	if (this->playerItem == nullptr)
	{
		QGraphicsRectItem* playerItem = ui->gamePane->scene()->addRect(QRectF(0, 0, FIELDSIZE, FIELDSIZE), Qt::NoPen, QBrush(this->player));
		playerItem->setPos(x * FIELDSIZE, y * FIELDSIZE);
		playerItem->setZValue(z);
		this->playerItem = playerItem;
		return true;
	}
	if (this->playerItem->pos().x() / FIELDSIZE == x && this->playerItem->pos().y() / FIELDSIZE == y)
	{
		return false;
	}

	this->playerItem->setPos(x * FIELDSIZE, y * FIELDSIZE);
	this->steps++;
	updateSteps();
	return true;
}

void mainwindow::updateGhostItems()
{
	int z = 2;
	if (this->ghostItems.empty())
	{
		for (auto ghost : gameInterface->getGhosts())
		{
			int x = std::get<1>(ghost);
			int y = std::get<0>(ghost);
			QGraphicsItem* ghostItem = ui->gamePane->scene()->addRect(QRectF(0, 0, FIELDSIZE, FIELDSIZE), Qt::NoPen, QBrush(this->ghost));
			ghostItem->setPos(x * FIELDSIZE, y * FIELDSIZE);
			ghostItem->setZValue(z);
			this->ghostItems.insert(std::make_pair(tuple(x, y), ghostItem));
		}
		return;
	}
	list<tuple<int, int>> ghosts = gameInterface->getGhosts();
	list<tuple<int, int>> toRemove;
	for (auto& ghostItem : this->ghostItems)
	{
		int x = std::get<1>(ghostItem.first);
		int y = std::get<0>(ghostItem.first);
		bool found = false;
		for (auto ghost : ghosts)
		{
			if (ghost == tuple(x, y)) {
				ghosts.remove(ghost);
				found = true;
				break;
			}
		}
		if (!found)
		{
			toRemove.push_back(ghostItem.first);
		}
	}
	for (auto key : toRemove)
	{
		QGraphicsItem* tmp = this->ghostItems.find(key)->second;
		this->ghostItems.erase(key);
		int x = std::get<1>(ghosts.back());
		int y = std::get<0>(ghosts.back());
		ghosts.pop_back();
		tmp->setPos(x * FIELDSIZE, y * FIELDSIZE);
		this->ghostItems.insert(std::make_pair(tuple(x, y), tmp));
	}
}

void mainwindow::updateLiveItems()
{
	int count = 1;
	for (auto& live : this->liveItems)
	{
		if (count <= gameInterface->getLives())
		{
			live->setBrush(QBrush(player));
		}
		else {
			live->setBrush(QBrush(playerEmpty));
		}
		count++;
	}
}

void mainwindow::updateDoorItem()
{
	if (this->doorItem == nullptr) { return; }
	int x = std::get<1>(gameInterface->getDoor());
	int y = std::get<0>(gameInterface->getDoor());
	if (gameInterface->isDoorOpen())
	{
		this->doorItem->setBrush(QBrush(this->doorOpen));
	}
	else
	{
		this->doorItem->setBrush(QBrush(this->doorClosed));
	}
}

void mainwindow::updateKeyItems(tuple<int, int>from)
{
	auto it = this->keyItems.find(tuple(std::get<0>(from), std::get<1>(from)));
	if (it == this->keyItems.end()) { throw new exception(); }
	ui->gamePane->scene()->removeItem(it->second);
	this->keyItems.erase(tuple(std::get<0>(from), std::get<1>(from)));
}

void mainwindow::updateKeyScoreItems()
{
	int count = gameInterface->getMaxKeys();
	for (auto& key : this->keyScoreItems)
	{
		if (count > gameInterface->getKeys().size())
		{
			key->setBrush(QBrush(this->key));
		}
		else {
			key->setBrush(QBrush(keyEmpty));
		}
		count--;
	}
}

void mainwindow::updateSteps()
{
	if (this->stepsItem == nullptr) {
		this->stepsItem = new QGraphicsTextItem("Steps: " + QString::number(this->steps));
		this->stepsItem->setFont(ui->legendLabel->font());
		this->stepsItem->setPos(0, ui->gameScorePane->height() - this->stepsItem->boundingRect().height());
		this->stepsItem->setTextWidth(ui->gameScorePane->width());
		this->stepsItem->setScale(7);
		ui->gameScorePane->scene()->addItem(this->stepsItem);
	}
	this->stepsItem->setPlainText("Steps: " + QString::number(this->steps));
	ui->gameScorePane->update();
}

void mainwindow::startReplay()
{
	list<string> maze = this->replayer->getMaze();
	gameInterface->loadReplay(maze);
	this->playGame();
	if (!forward) {
		while (this->replayer->nextTurn()) {}
	}
	this->moveGhostsTimer.stop();
	ui->gamePane->clearFocus();
	ui->gameGuidePane->setFocus();
	this->replayTimer.start(800);
}

bool mainwindow::eventFilter(QObject* obj, QEvent* event)
{
	if (event->type() == QEvent::KeyPress)
	{
		if (obj == ui->gamePane)
		{
			QKeyEvent* keyEvent = (QKeyEvent*)event;
			if (keyEvent->key() == Qt::Key_Escape)
			{
				this->keyPressTimer.stop();
				this->moveGhostsTimer.stop();
				this->pendingKey = 0;
				ui->gamePane->clearFocus();
				ui->resultWidget->setFocus();
				ui->gameWidget->setVisible(false);
				ui->resultWidget->setVisible(false);
				ui->mainMenuWidget->setVisible(true);
				this->clearAfterGame();
			}
			else if (keyEvent->key() == Qt::Key_Space)
			{
				this->pause = true;
				this->keyPressTimer.stop();
				this->moveGhostsTimer.stop();
				this->pendingKey = 0;
				ui->gamePane->clearFocus();
				ui->resultWidget->setFocus();
				ui->resultWidget->setVisible(true);
				this->setPauseScreen();
			}
			else {
				this->keyPressTimer.start(500);
				this->pendingKey = keyEvent->key();
			}
		}
		if (obj == ui->resultWidget)
		{
			QKeyEvent* keyEvent = (QKeyEvent*)event;
			if (keyEvent->key() == Qt::Key_Escape)
			{
				ui->resultWidget->clearFocus();
				ui->gameWidget->setVisible(false);
				ui->resultWidget->setVisible(false);
				ui->mainMenuWidget->setVisible(true);
				this->clearAfterGame();
			}
			else if (keyEvent->key() == Qt::Key_Space && pause && this->replayer == nullptr)
			{
				this->pause = false;
				this->moveGhostsTimer.start(500);
				ui->gamePane->setFocus();
				ui->resultWidget->setVisible(false);
			}
			else if (keyEvent->key() == Qt::Key_Space && pause && this->replayer != nullptr)
			{
				this->pause = false;
				this->replayTimer.start(800);
				ui->gameGuidePane->setFocus();
				ui->resultWidget->setVisible(false);
			}
		}
		if (obj == ui->gameGuidePane)
		{
			QKeyEvent* keyEvent = (QKeyEvent*)event;
			if (keyEvent->key() == Qt::Key_Escape)
			{
				ui->gameGuidePane->clearFocus();
				ui->gameWidget->setVisible(false);
				ui->resultWidget->setVisible(false);
				ui->mainMenuWidget->setVisible(true);
				this->clearAfterGame();
			}
			else if (keyEvent->key() == Qt::Key_Right)
			{
				this->forward = true;
				this->replayTimer.stop();
				this->replayTurn();
			}
			else if (keyEvent->key() == Qt::Key_Left)
			{
				this->forward = false;
				this->replayTimer.stop();
				this->replayTurn();
			}
			else if (keyEvent->key() == Qt::Key_Up)
			{
				this->forward = true;
				this->replayTimer.start(800);
			}
			else if (keyEvent->key() == Qt::Key_Down)
			{
				this->forward = false;
				this->replayTimer.start(800);
			}
			else if (keyEvent->key() == Qt::Key_Space)
			{
				if (this->pause) {
					this->pause = false;
					ui->resultWidget->setVisible(false);
				}
				else {
					this->pause = true;
					this->replayTimer.stop();
					ui->resultWidget->setVisible(true);
					this->setPauseScreen();
				}
			}
		}
	}
	return QObject::eventFilter(obj, event);
}

void mainwindow::processKeyPressEvent()
{
	int keyEvent = this->pendingKey;
	if (keyEvent != 0) {
		if (keyEvent == Qt::Key_Right) {
			gameInterface->movePlayer(2);
		}
		if (keyEvent == Qt::Key_Left) {
			gameInterface->movePlayer(0);
		}
		if (keyEvent == Qt::Key_Up) {
			gameInterface->movePlayer(1);
		}
		if (keyEvent == Qt::Key_Down) {
			gameInterface->movePlayer(3);
		}
		pendingKey = 0;
	}
}

void mainwindow::moveGhosts()
{
	gameInterface->startGame();
}

void mainwindow::replayTurn()
{
	if (this->replayer == nullptr) {
		return;
	}
	if (this->forward) {
		if (!this->replayer->nextTurn()) {
			this->replayTimer.stop();
			//delete this->replayer;
			//this->replayer = nullptr;
		}
	}
	else {
		if (!this->replayer->prevTurn()) {
			this->replayTimer.stop();
			//delete this->replayer;
			//this->replayer = nullptr;
		}
	}

}
