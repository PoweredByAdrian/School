/**
 * @file gameinterface.cpp
 *
 * @brief Source file containing the GameInterface class
 *
 * @author Adrian Horvath (xhorva14)
 */
#include "gameinterface.h"

//#include <QDebug>


// Constructor initializes the board using the default chessboard layout
GameInterface::GameInterface(std::string* board)
{
    *board = chessboard;
}

// Convert row and col to chess coordinates (A1, B2, ..., H8)
std::string GameInterface::convertToChessCoordinates(int row, int col) {
    char file = 'a' + col;
    char rank = '8' - row;
    std::string chessCoord = "";
    chessCoord += file;
    chessCoord += rank;
    return chessCoord;
}

// Move a piece on the chessboard
void GameInterface::movePiece(char piece, int srcRow, int srcCol, int dstRow, int dstCol){
    int srcIndex = srcRow * 9 + srcCol;
    int destIndex = dstRow * 9 + dstCol;

    // Update the chessboard string
    chessboard[destIndex] = piece;
    chessboard[srcIndex] = '.';

    // Print the updated chessboard
    //qDebug().noquote() << "Updated Chessboard:\n" << chessboard << "\n";

    // Get the new chessboard position from the chess rules library
    std::string s = cr.ToDebugStr();
    size_t firstNewlinePos = s.find('\n');
    size_t secondNewlinePos = s.find('\n', firstNewlinePos + 1);
    std::string newChessboard = s.substr(secondNewlinePos + 1);

    // If the chessboard has changed, update it
    if (chessboard != newChessboard){
        error = true;
        chessboard = newChessboard;
    }

    //qDebug().noquote() << newChessboard;
}

// Promote a pawn to the specified piece type
void GameInterface::promotePiece(const std::string piece, std::string* board){
    std::string move;

    // Convert the piece to FEN symbol
    if (piece == "rook") {
        move = source + destination + "R";
    } else if (piece == "bishop") {
        move = source + destination + "B";
    } else if (piece == "queen") {
        move = source + destination + "Q";
    } else if (piece == "knight") {
        move = source + destination + "N";
    }

    // Play the move and update the board
    if(mv.TerseIn(&cr, move.c_str()))
    {
        cr.PlayMove(mv);
        movePiece('Q', 0, 0, 0, 0);
    }
    *board = chessboard;
}

// Get the piece at the specified position on the board
char GameInterface::getPieceAtPosition(const std::string& board, int row, int col) {
    int index = row * (8 + 1) + col;
    if (index >= 0 && index < static_cast<int>(board.size())) {
        return board[index];
    } else {
        return '.';
    }
}

// Handle a player's move and determine its validity
GameInterface::result GameInterface::pieceSelected(const std::pair<int, int>& gridSourceCoordinates, const std::pair<int, int>& gridDestCoordinates, std::string* board)
{
    bool moveIsValid;
    error = false;

    // Get the piece at the source position
    char piece = getPieceAtPosition(chessboard, gridSourceCoordinates.first, gridSourceCoordinates.second);

    // Convert grid coordinates to chess coordinates
    source = convertToChessCoordinates(gridSourceCoordinates.first, gridSourceCoordinates.second);
    destination = convertToChessCoordinates(gridDestCoordinates.first, gridDestCoordinates.second);

    std::string move;
    //qDebug() << "Piece: " << piece << " selected at grid coordinates: (" << source << ") to coordinates: (" <<  destination << ")";

    // Create a move string in algebraic notation
    move = source + destination;

    // Check if the move is valid
    if(mv.TerseIn(&cr, move.c_str()))
    {
        moveIsValid = true;

        // Check for pawn promotion
        if(mv.special == thc::SPECIAL_PROMOTION_QUEEN){
            return PROMOTE;
        }

        // Play the move and update the board
        cr.PlayMove(mv);
        movePiece(piece, gridSourceCoordinates.first, gridSourceCoordinates.second, gridDestCoordinates.first, gridDestCoordinates.second);
    }

    // Evaluate the position for checkmate or stalemate
    thc::TERMINAL eval_penultimate_position;
    bool legal1 = cr.Evaluate( eval_penultimate_position );
    bool normal = (eval_penultimate_position == thc::NOT_TERMINAL);

    if(!normal){
        if(eval_penultimate_position  == thc::TERMINAL_BCHECKMATE || eval_penultimate_position  == thc::TERMINAL_WCHECKMATE){
            return MATE;
        }
        else if(eval_penultimate_position  == thc::TERMINAL_BSTALEMATE || eval_penultimate_position  == thc::TERMINAL_WSTALEMATE)
        {
            return DRAW;
        }
    }

    // If an error occurred during move handling, refresh the board
    if(error){
        *board = chessboard;
        return REFRESH;
    }

    // If the move is valid, return VALID, otherwise, return INVALID
    if(moveIsValid){
        return VALID;
    }

    return INVALID;
}

// Handle a move received from the Stockfish engine
GameInterface::result GameInterface::stockFishMove(std::string stockfishmove, std::string* board)
{
    bool moveIsValid;
    error = false;

    // Check if the Stockfish move is valid
    if(mv.TerseIn(&cr, stockfishmove.c_str()))
    {
        moveIsValid = true;

        // Check for pawn promotion
        if(mv.special == thc::SPECIAL_PROMOTION_QUEEN){
            return PROMOTE;
        }

        // Play the move and update the board
        cr.PlayMove(mv);
        movePiece('Q', 0, 0, 0, 0);
        *board = chessboard;
    }

    // Evaluate the position for checkmate or stalemate
    thc::TERMINAL eval_penultimate_position;
    bool legal1 = cr.Evaluate( eval_penultimate_position );
    bool normal = (eval_penultimate_position == thc::NOT_TERMINAL);

    if(!normal){
        if(eval_penultimate_position  == thc::TERMINAL_BCHECKMATE || eval_penultimate_position  == thc::TERMINAL_WCHECKMATE){
            return MATE;
        }
        else if(eval_penultimate_position  == thc::TERMINAL_BSTALEMATE || eval_penultimate_position  == thc::TERMINAL_WSTALEMATE)
        {
            return DRAW;
        }
    }

    // If an error occurred during move handling, refresh the board
    if(error){
        *board = chessboard;
        return REFRESH;
    }

    // If the move is valid, return VALID, otherwise, return INVALID
    if(moveIsValid){
        return VALID;
    }

    return INVALID;
}
