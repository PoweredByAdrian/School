/**
 * @file connectionmanager.h
 *
 * @brief Header file for the ConnectionManager class
 *
 * The ConnectionManager class manages the connections between the main window, game logic, and Stockfish engine.
 * It handles signals and slots to coordinate player moves and game state.
 *
 * @author Adrian Horvath (xhorva14)
 */

#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>
#include <QPoint>
#include "GameInterface.h"
#include "stockfishclass.h"

class GameInterface;
class MainWindow;


class ConnectionManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a ConnectionManager instance.
     * @param parent The parent QObject.
     * @param bot Indicates whether a playing against bot is allowed or not.
     */
    explicit ConnectionManager(QObject *parent = nullptr, bool bot = false);

    /**
     * @brief Destroys the ConnectionManager instance.
     */
    ~ConnectionManager();

    /**
     * @brief Connects the GameInterface to the ConnectionManager.
     * @param gameInterface The GameInterface to connect.
     */
    void connectGameInterface(GameInterface *gameInterface);

    /**
     * @brief Connects the MainWindow to the ConnectionManager.
     * @param mainWindow The MainWindow to connect.
     */
    void connectMainWindow(MainWindow *mainWindow);

private slots:
    /**
     * @brief Handles the selection of a chess piece.
     * @param sourceCoord The source coordinate of the selected piece.
     * @param destinationCoord The destination coordinate for the selected piece.
     */
    void handlePieceSelection(const QPoint &sourceCoord, const QPoint &destinationCoord);

    /**
     * @brief Handles the promotion of a chess piece.
     * @param newPiece The type of the new piece after promotion.
     */
    void handlePiecePromotion(const std::string newPiece);

    /**
     * @brief Handles the selection of a move by the StockFish engine.
     * @param stockfishmove The move selected by the StockFish engine.
     */
    void handleStockFishSelection(const std::string &stockfishmove);

private:
    GameInterface* gi;          ///< Pointer to the GameInterface instance.
    StockFishClass* sf;         ///< Pointer to the StockFishClass instance.
    bool botAllowed;            ///< Indicates whether the bot is allowed or not.

signals:
    /**
     * @brief Signal emitted when a move of a chess piece is requested.
     * @param sourceX The x-coordinate of the source piece.
     * @param sourceY The y-coordinate of the source piece.
     * @param destX The x-coordinate of the destination.
     * @param destY The y-coordinate of the destination.
     */
    void movePieceRequested(int sourceX, int sourceY, int destX, int destY);

    /**
     * @brief Signal emitted when setting up the initial chessboard pieces is requested.
     * @param chessboard
     */
    void setupPiecesRequested(const std::string& chessboard);

    /**
     * @brief Signal emitted when the promotion of chess pieces is requested.
     */
    void promotionPiecesRequested();

    /**
     * @brief Signal emitted when the gameover state is requested.
     * @param isDraw Indicates whether the game ended in a draw.
     */
    void gameoverRequested(bool isDraw);
};

#endif // CONNECTIONMANAGER_H
