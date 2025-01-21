/**
 * @file gameinterface.h
 *
 * @brief Header file for the GameInterface class
 *
 * The GameInterface class manages the game logic and interactions between players.
 * It handles moves, promotions, and communicates with the Stockfish engine.
 *
 * @author Adrian Horvath (xhorva14)
 */

#ifndef GAMEINTERFACE_H
#include "thc.h"

class GameInterface
{

public:
    /**
     * @brief The result enumeration represents the possible results of game actions.
     */
    enum result{
        VALID,    ///< The action is valid.
        REFRESH,  ///< The action requires refreshing the game state.
        PROMOTE,  ///< The action involves promoting a pawn.
        MATE,     ///< The game has reached a checkmate state.
        DRAW,     ///< The game has ended in a draw.
        INVALID   ///< The action is invalid.
    };

    /**
     * @brief Constructs a GameInterface instance.
     * @param board A pointer to the initial chessboard configuration.
     */
    explicit GameInterface(std::string* board);

    /**
     * @brief Handles the selection of a chess piece.
     * @param gridSourceCoordinates The source coordinates of the selected piece in grid format.
     * @param gridDestCoordinates The destination coordinates for the selected piece in grid format.
     * @param board A pointer to the current chessboard configuration.
     * @return The result of the piece selection.
     */
    GameInterface::result pieceSelected(const std::pair<int, int>& gridSourceCoordinates,
                                        const std::pair<int, int>& gridDestCoordinates,
                                        std::string* board);

    /**
     * @brief Handles the move received from StockFish.
     * @param stockfishmove The move received from StockFish.
     * @param board A pointer to the current chessboard configuration.
     * @return The result of the StockFish move.
     */
    GameInterface::result stockFishMove(std::string stockfishmove, std::string* board);

    /**
     * @brief Promotes a pawn to a specified piece.
     * @param piece The type of the piece to promote to.
     * @param board A pointer to the current chessboard configuration.
     */
    void promotePiece(const std::string piece, std::string* board);

    /**
     * @brief Gets the FEN position of the current chessboard.
     * @return The FEN representation of the chessboard.
     */
    std::string getFENPosition() { return cr.ForsythPublish().c_str(); }


private:
    /**
     * @brief Gets the piece at a specified position on the chessboard.
     * @param board The current chessboard configuration.
     * @param row The row index.
     * @param col The column index.
     * @return The piece at the specified position.
     */
    char getPieceAtPosition(const std::string& board, int row, int col);

    /**
     * @brief Converts row and column indices to chess coordinates.
     * @param row The row index.
     * @param col The column index.
     * @return The chess coordinates as a string.
     */
    std::string convertToChessCoordinates(int row, int col);

    /**
     * @brief Moves a chess piece on the chessboard.
     * @param piece The type of the piece.
     * @param srcRow The source row index.
     * @param srcCol The source column index.
     * @param dstRow The destination row index.
     * @param dstCol The destination column index.
     */
    void movePiece(char piece, int srcRow, int srcCol, int dstRow, int dstCol);

    /**
     * @brief Converts chess coordinates to grid coordinates.
     * @param chessCoords The chess coordinates as a string.
     * @return The grid coordinates as a pair of pairs.
     */
    std::pair<std::pair<int, int>, std::pair<int, int>> resultconvertToGridCoordinates(const std::string& chessCoords);

    /**
     * @brief The initial chessboard configuration.
     */
    std::string chessboard =
        "rnbqkbnr\n"
        "pppppppp\n"
        "........\n"
        "........\n"
        "........\n"
        "........\n"
        "PPPPPPPP\n"
        "RNBQKBNR\n";

    thc::ChessRules cr;  ///< The chess rules instance.
    thc::Move mv;        ///< The chess move instance.

    bool error;           ///< Indicates whether an error occurred.
    std::string source;    ///< The source chess coordinates.
    std::string destination;  ///< The destination chess coordinates.
};

#endif // GAMEINTERFACE_H
