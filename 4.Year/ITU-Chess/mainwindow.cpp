/**
 * @file mainwindow.cpp
 *
 * @brief Source file containing the MainWindow class
 *
 * @author Adrian Horvath (xhorva14)
 */
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "ui_rematch.h"
#include "ui_ui_rematch.h"
#include "promote.h"
#include <QHBoxLayout>



// Constructor for the MainWindow class
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , backgroundLabel(new QLabel(this))
{
    ui->setupUi(this);

    sourceCoordSet = false;

    // Set the fixed size of the MainWindow
    setFixedSize(boardSize, boardSize + 3);

    // Load the board image as the background
    QPixmap boardPixmap(":/Resources/board.png");

    // Set the background pixmap to the label
    backgroundLabel->setPixmap(boardPixmap);

    // Set the geometry of the background label to cover the entire window
    backgroundLabel->setGeometry(0, 0, width(), height());

    // Make the background label a child of the central widget
    backgroundLabel->setParent(centralWidget());

    // Ensure the background label is behind other widgets
    backgroundLabel->lower();

    // Initialize the grid
    initializeGrid();

    // Choose player type (human or bot)
    selectPlayerType();

    // Initialize and connect to the ConnectionManager
    cm = new ConnectionManager(this, againstBot);
    cm->connectMainWindow(this);

    // Set up the restart button
    restartButton = new QPushButton(this);
    QPixmap pixmap(":/Resources/restart.png");

    // Resize the icon to a smaller size
    QPixmap resizedIcon = pixmap.scaled(QSize(102, 27), Qt::KeepAspectRatio);

    // Set the pixmap as the button's icon
    restartButton->setIcon(QIcon(resizedIcon));
    restartButton->setIconSize(resizedIcon.size());
    restartButton->setFixedSize(resizedIcon.size());

    // Connect the restart button click to the corresponding slot
    connect(restartButton, &QPushButton::clicked, this, &MainWindow::onRestartButtonClicked);

    // Set up the layout for the restart button
    QHBoxLayout *layout = new QHBoxLayout(centralWidget());
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(restartButton, 0, Qt::AlignTop | Qt::AlignRight);

    // Set a tooltip that will be displayed on hover
    restartButton->setToolTip("Restart");
}

// Destructor for the MainWindow class
MainWindow::~MainWindow()
{
    delete ui; // Release resources allocated for the UI
}

// Function to choose player type (human or bot) using a menu dialog
void MainWindow::selectPlayerType()
{
    // Create an instance of the main window UI
    menuDialog = new Ui::ui_menu;
    menuDialog->setupUi(&menuWidget);

    // Set the window flags to make the dialog stay on top
    menuWidget.setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);

    // Connect signals and slots for player type selection
    connect(menuDialog->pushButton, SIGNAL(clicked()), this, SLOT(on_playAgainstPlayerButtonClicked()));
    connect(menuDialog->pushButton_2, SIGNAL(clicked()), this, SLOT(on_playAgainstBotButtonClicked()));

    // Show the menu dialog modally (blocking)
    if (menuWidget.exec() == QDialog::Accepted) {
        qDebug() << "Dialog accepted.";
    } else {
        qDebug() << "Dialog rejected or closed.";
        qApp->quit(); // Quit the application
    }
}

// Slot for handling the play against player button click
void MainWindow::on_playAgainstPlayerButtonClicked()
{
    // Set the player type to human
    againstBot = false;
    // Perform other actions as needed
    menuWidget.accept();  // Close the menu dialog
}

// Slot for handling the play against bot button click
void MainWindow::on_playAgainstBotButtonClicked()
{
    // Set the player type to bot
    againstBot = true;
    // Perform other actions as needed
    menuWidget.accept();  // Close the menu dialog
}

// Function to restart the game
void MainWindow::restart()
{
    sourceCoordSet = false;

    // Choose player type (human or bot) again
    selectPlayerType();

    // Delete the existing ConnectionManager
    delete cm;

    // Initialize and connect to the ConnectionManager again
    cm = new ConnectionManager(this, againstBot);
    cm->connectMainWindow(this);
}

// Function to initialize the chessboard grid
void MainWindow::initializeGrid()
{
    // Resize the gridLabels array
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            QLabel* label = new QLabel(this);
            int x = 35 + col * gridSize;
            int y = 36 + row * gridSize;
            label->setGeometry(x, y, gridSize, gridSize);
            label->setAlignment(Qt::AlignCenter);  // Optional: Center text in QLabel
            label->setFrameStyle(QFrame::Box);    // Optional: Add a border to the QLabel
            gridLabels[row][col] = label;
        }
    }
}

// Slot for handling the restart button click
void MainWindow::onRestartButtonClicked()
{
    restart();
}

// Function to handle pawn promotion using a promotion dialog
void MainWindow::promotionPiece()
{
    // Create an instance of the promotion dialog
    Promote promotionDialog(this);
    promotionDialog.setWindowTitle("Pawn Promotion");

    // Show the promotion dialog
    if (promotionDialog.exec() == QDialog::Accepted) {
        // Check which piece is selected in the dialog and emit the corresponding signal
        if (promotionDialog.isRookSelected()) {
            emit piecePromoted("rook");
        } else if (promotionDialog.isBishopSelected()) {
            emit piecePromoted("bishop");
        } else if (promotionDialog.isQueenSelected()) {
            emit piecePromoted("queen");
        } else if (promotionDialog.isKnightSelected()) {
            emit piecePromoted("knight");
        }
    }
}

// Function to display a gameover dialog
void MainWindow::gameoverWindow(bool isDraw)
{
    // Create an instance of the rematch dialog UI

    Ui::ui_Rematch uiRematch;  // Instance for the rematch dialog UI
    uiRematch.setupUi(&rematchDialog);

    // Setup the UI for the rematch dialog
    uiRematch.setupUi(&rematchDialog);

    if (isDraw) {
        uiRematch.label_2->setText("It's a draw!");
    } else {
        uiRematch.label_2->setText("Checkmate!");
    }

    // Connect signals and slots for rematch dialog buttons
    connect(uiRematch.pushButton, &QPushButton::clicked, this, &MainWindow::onRematchButtonClicked);
    connect(uiRematch.pushButton_2, &QPushButton::clicked, this, &MainWindow::onExitButtonClicked);

    // Show the rematch dialog modally (blocking)
    if (rematchDialog.exec() == QDialog::Accepted) {
        qDebug() << "Dialog accepted.";
        rematchDialog.close();
    } else {
        qDebug() << "Dialog rejected or closed.";
        qApp->quit(); // Quit the application
    }
}

// Slot for handling the rematch button click
void MainWindow::onRematchButtonClicked()
{
    rematchDialog.accept();
    restart();
}

// Slot for handling the exit button click
void MainWindow::onExitButtonClicked()
{
    qApp->quit();  // Quit the application
}

// Function to get grid coordinates based on the mouse click position
QPoint MainWindow::getGridCoordinates(const QPoint &position)
{
    // Find the grid coordinates for a given position
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            if (gridLabels[row][col]->geometry().contains(position)) {
                return QPoint(row, col);
            }
        }
    }
    return QPoint(-1, -1);  // Invalid coordinates
}

// Function to set up chess pieces on the chessboard based on the provided chessboard string
void MainWindow::setupPieces(const std::string& chessboard)
{
    // Loop through each square on the chessboard and set up pieces
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            QLabel *pieceLabel = gridLabels[row][col];
            QPixmap piecePixmap;

            // Get the piece character from the chessboard string
            char pieceChar = chessboard[row * (numCols + 1) + col];

            // Set up pieces based on the piece character
            switch (pieceChar) {
            case 'p':
                piecePixmap.load(":Resources/Black/pawn.png");
                break;
            case 'r':
                piecePixmap.load(":/Resources/Black/rook.png");
                break;
            case 'n':
                piecePixmap.load(":/Resources/Black/knight.png");
                break;
            case 'b':
                piecePixmap.load(":/Resources/Black/bishop.png");
                break;
            case 'q':
                piecePixmap.load(":/Resources/Black/queen.png");
                break;
            case 'k':
                piecePixmap.load(":/Resources/Black/king.png");
                break;
            case 'P':
                piecePixmap.load(":/Resources/White/pawn.png");
                break;
            case 'R':
                piecePixmap.load(":/Resources/White/rook.png");
                break;
            case 'N':
                piecePixmap.load(":/Resources/White/knight.png");
                break;
            case 'B':
                piecePixmap.load(":/Resources/White/bishop.png");
                break;
            case 'Q':
                piecePixmap.load(":/Resources/White/queen.png");
                break;
            case 'K':
                piecePixmap.load(":/Resources/White/king.png");
                break;
            default:
                // Handle other pieces or empty squares as needed
                piecePixmap = QPixmap(); // An empty pixmap for now
                break;
            }

            // Resize the piece
            piecePixmap = piecePixmap.scaled(figureSize, figureSize, Qt::KeepAspectRatio);

            pieceLabel->setPixmap(piecePixmap);
        }
    }
}

// Function to move a chess piece on the chessboard
void MainWindow::movePiece(int x, int y, int xDest, int yDest)
{
    // Assuming you have a 2D array of QLabel pointers representing the chessboard
    QLabel *selectedPiece = gridLabels[x][y];

    // Check if the destination coordinates are within bounds
    if (xDest >= 0 && xDest < numRows && yDest >= 0 && yDest < numCols) {
        // Move the piece to the destination coordinates
        gridLabels[xDest][yDest]->setPixmap(selectedPiece->pixmap());
        // Optionally, clear the pixmap at the source coordinates to remove the original piece
        selectedPiece->clear();
    } else {
        qDebug() << "Invalid destination coordinates.";
    }
}

// Function to handle mouse clicks on the chessboard
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (!sourceCoordSet) {
        // First click - set the source coordinates
        sourceCoord = getGridCoordinates(event->pos());
        qDebug() << "Clicked on source grid coordinates:" << sourceCoord;
        sourceCoordSet = true;
    } else {
        // Second click - set the destination coordinates
        destinationCoord = getGridCoordinates(event->pos());
        qDebug() << "Clicked on destination grid coordinates:" << destinationCoord;
        sourceCoordSet = false;

        // Emit a signal to notify the ConnectionManager about the piece movement
        emit mousePressed(sourceCoord, destinationCoord);
    }
}

