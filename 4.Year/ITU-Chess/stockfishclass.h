/**
 * @file stockfish.h
 *
 * @brief Header file for the StockFish class
 *
 * The StockFishClass class handles communication with the Stockfish chess engine.
 * It sends FEN positions to Stockfish and receives the best move.
 *
 * @author Adrian Horvath (xhorva14)
 */

#ifndef STOCKFISHCLASS_H
#define STOCKFISHCLASS_H

#include <QObject>
#include <QString>

class StockFishClass : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a StockFishClass instance.
     * @param parent The parent QObject.
     */
    explicit StockFishClass(QObject *parent = nullptr);

    /**
     * @brief Gets the Stockfish engine response for a given FEN position.
     * @param fenPosition The Forsyth-Edwards Notation (FEN) position.
     * @return The Stockfish engine response.
     */
    std::string getStockfishResponse(const std::string &fenPosition);

signals:
    /**
     * @brief Signal emitted when Stockfish provides the next move.
     * @param moves The moves provided by Stockfish.
     */
    void setMoveNext(const std::string &moves);

private:
    /**
     * @brief Gets the depth for Stockfish analysis.
     * @return The Stockfish depth.
     *
     * Implement this method as needed.
     */
    int getStockfishDepth();
};

#endif // STOCKFISHCLASS_H
