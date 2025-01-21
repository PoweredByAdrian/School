/**
 * @file promote.h
 *
 * @brief Header file for the Promote Ui class
 *
 * The Promote class represents the promotion dialog for pawn promotion in the chess game.
 * Players can choose the promoted piece (rook, bishop, queen, knight) using this dialog.
 *
 * @author Adrian Horvath (xhorva14)
 */

#ifndef PROMOTE_H
#define PROMOTE_H

#include <QDialog>

namespace Ui {
class Promote;
}

class Promote : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a Promote instance.
     * @param parent The parent widget.
     */
    explicit Promote(QWidget *parent = nullptr);

    /**
     * @brief Destroys the Promote instance.
     */
    ~Promote();

    /**
     * @brief Checks if the rook is selected for promotion.
     * @return True if the rook is selected, false otherwise.
     */
    bool isRookSelected() const { return selectedPiece == "rook"; }

    /**
     * @brief Checks if the bishop is selected for promotion.
     * @return True if the bishop is selected, false otherwise.
     */
    bool isBishopSelected() const { return selectedPiece == "bishop"; }

    /**
     * @brief Checks if the queen is selected for promotion.
     * @return True if the queen is selected, false otherwise.
     */
    bool isQueenSelected() const { return selectedPiece == "queen"; }

    /**
     * @brief Checks if the knight is selected for promotion.
     * @return True if the knight is selected, false otherwise.
     */
    bool isKnightSelected() const { return selectedPiece == "knight"; }

private slots:
    /**
     * @brief Slot to handle the selection of the rook.
     */
    void onRookSelected();

    /**
     * @brief Slot to handle the selection of the bishop.
     */
    void onBishopSelected();

    /**
     * @brief Slot to handle the selection of the queen.
     */
    void onQueenSelected();

    /**
     * @brief Slot to handle the selection of the knight.
     */
    void onKnightSelected();

private:
    QString selectedPiece;  ///< The selected piece type.
    Ui::Promote *ui;  ///< The UI instance.
};

#endif // PROMOTE_H
