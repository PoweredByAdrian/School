/**
 * @file connectionmanager.cpp
 *
 * @brief Source file containing the ConnectionManager class
 *
 * @author Adrian Horvath (xhorva14)
 */
#include "ConnectionManager.h"
#include "MainWindow.h"



// Constructor for the ConnectionManager class
ConnectionManager::ConnectionManager(QObject *parent, bool bot)
    : QObject(parent), botAllowed(bot), gi(nullptr), sf(nullptr)
{
}

// Destructor for the ConnectionManager class
ConnectionManager::~ConnectionManager()
{
    delete gi; // Release resources allocated for GameInterface instance
    delete sf; // Release resources allocated for StockFishClass instance
}

// Connect signals and slots between MainWindow and ConnectionManager
void ConnectionManager::connectMainWindow(MainWindow *mainWindow)
{
    // Connect signals and slots
    connect(mainWindow, &MainWindow::mousePressed, this, &ConnectionManager::handlePieceSelection);
    connect(mainWindow, &MainWindow::piecePromoted, this, &ConnectionManager::handlePiecePromotion);
    connect(this, &ConnectionManager::movePieceRequested, mainWindow, &MainWindow::movePiece);
    connect(this, &ConnectionManager::setupPiecesRequested, mainWindow, &MainWindow::setupPieces);
    connect(this, &ConnectionManager::promotionPiecesRequested, mainWindow, &MainWindow::promotionPiece);
    connect(this, &ConnectionManager::gameoverRequested, mainWindow, &MainWindow::gameoverWindow);

    // Initialize GameInterface and StockFishClass instances
    std::string board;
    gi = new GameInterface(&board);
    sf = new StockFishClass(this);

    // Connect StockFishClass signal to handleStockFishSelection slot
    connect(sf, &StockFishClass::setMoveNext, this, &ConnectionManager::handleStockFishSelection);

    // Emit signal to set up initial pieces on the board
    emit setupPiecesRequested(board);
}

// Utility function to convert QPoint to std::pair<int, int>
std::pair<int, int> convertQPointToPair(const QPoint &point)
{
    return std::make_pair(point.x(), point.y());
}

// Handle the promotion of a pawn to a new piece
void ConnectionManager::handlePiecePromotion(const std::string newPiece)
{
    std::string board;
    gi->promotePiece(newPiece, &board);
    emit setupPiecesRequested(board);
}

// Handle the selection of a chess piece on the board
void ConnectionManager::handlePieceSelection(const QPoint &sourceCoord, const QPoint &destinationCoord)
{
    // Convert QPoint to std::pair<int, int>
    const std::pair<int, int> &convertedSourceCoordinates = convertQPointToPair(sourceCoord);
    const std::pair<int, int> &convertedDestCoordinates = convertQPointToPair(destinationCoord);

    bool isMoveValid = false;
    bool refreshboard = false;
    bool promotion = false;

    std::string board;

    GameInterface::result validity;

    // Determine the result of the piece selection
    validity = gi->pieceSelected(convertedSourceCoordinates, convertedDestCoordinates, &board);

    if (validity == GameInterface::PROMOTE)
    {
        // Promote the piece and update the board
        emit promotionPiecesRequested();
    }
    else if (validity == GameInterface::VALID)
    {
        // Move the piece and update the board
        emit movePieceRequested(convertedSourceCoordinates.first, convertedSourceCoordinates.second,
                                convertedDestCoordinates.first, convertedDestCoordinates.second);
        qDebug() << "Valid move";
        if (botAllowed)
        {
            // If playing against a bot, get the bot's move
            sf->getStockfishResponse(gi->getFENPosition());
        }
    }
    else if (validity == GameInterface::REFRESH)
    {
        // Refresh the board without making any moves
        emit setupPiecesRequested(board);
        if (botAllowed)
        {
            // If playing against a bot, get the bot's move
            sf->getStockfishResponse(gi->getFENPosition());
        }
    }
    else if (validity == GameInterface::DRAW)
    {
        // Handle a draw situation
        emit movePieceRequested(convertedSourceCoordinates.first, convertedSourceCoordinates.second,
                                convertedDestCoordinates.first, convertedDestCoordinates.second);
        emit gameoverRequested(true);
    }
    else if (validity == GameInterface::MATE)
    {
        // Handle a checkmate situation
        emit movePieceRequested(convertedSourceCoordinates.first, convertedSourceCoordinates.second,
                                convertedDestCoordinates.first, convertedDestCoordinates.second);
        emit gameoverRequested(false);
    }
    else
    {
        qDebug() << "Invalid move";
    }
}

// Handle the response from the Stockfish engine
void ConnectionManager::handleStockFishSelection(const std::string &stockfishmove)
{
    std::string board;

    GameInterface::result validity;

    // Determine the result of the Stockfish move
    validity = gi->stockFishMove(stockfishmove, &board);

    if (validity == GameInterface::PROMOTE)
    {
        // Promote the piece and update the board
        emit setupPiecesRequested(board);
    }
    else if (validity == GameInterface::VALID)
    {
        // Update the board after a valid move
        emit setupPiecesRequested(board);
        qDebug() << "Valid move";
    }
    else if (validity == GameInterface::REFRESH)
    {
        // Refresh the board without making any moves
        emit setupPiecesRequested(board);
    }
    else if (validity == GameInterface::DRAW)
    {
        // Handle a draw situation
        emit setupPiecesRequested(board);
        emit gameoverRequested(true);
    }
    else if (validity == GameInterface::MATE)
    {
        // Handle a checkmate situation
        emit setupPiecesRequested(board);
        emit gameoverRequested(false);
    }
    else
    {
        qDebug() << "Invalid move";
    }
}
