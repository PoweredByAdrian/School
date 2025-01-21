/**
 * @file mainwindow.h
 *
 * @brief Header file for the MainWindow class
 *
 * The MainWindow class represents the main window of the chess application.
 * It includes the chessboard, player controls, and game-related functionalities.
 *
 * @author Adrian Horvath (xhorva14)
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QVector>
#include <QPoint>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>
#include "connectionmanager.h"
#include <QDialog>
#include <QMessageBox>
#include "ui_menu.h"
#include "ui_ui_menu.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a MainWindow instance.
     * @param parent The parent widget.
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destroys the MainWindow instance.
     */
    ~MainWindow();

signals:
    /**
     * @brief Signal emitted when a mouse button is pressed.
     * @param sourceCoordinates The source coordinates.
     * @param destCoordinates The destination coordinates.
     */
    void mousePressed(const QPoint &sourceCoordinates, const QPoint &destCoordinates);

    /**
     * @brief Signal emitted when a pawn is promoted.
     * @param newPiece The type of the new piece after promotion.
     */
    void piecePromoted(const std::string newPiece);

    /**
     * @brief Signal emitted when the restart button is pressed.
     */
    void restartPressed();

protected:
    /**
     * @brief Overrides the mouse press event to handle mouse clicks.
     * @param event The mouse event.
     */
    void mousePressEvent(QMouseEvent *event) override;

public slots:
    /**
     * @brief Slot to handle moving a chess piece.
     * @param x The x-coordinate of the source position.
     * @param y The y-coordinate of the source position.
     * @param xDest The x-coordinate of the destination position.
     * @param yDest The y-coordinate of the destination position.
     */
    void movePiece(int x, int y, int xDest, int yDest);

    /**
     * @brief Slot to handle setting up chess pieces on the board.
     * @param chessboard The chessboard configuration.
     */
    void setupPieces(const std::string& chessboard);

    /**
     * @brief Slot to handle pawn promotion.
     */
    void promotionPiece();

    /**
     * @brief Slot to handle displaying the gameover window.
     * @param isDraw Indicates if the game ended in a draw.
     */
    void gameoverWindow(bool isDraw);

    /**
     * @brief Slot to handle the restart button click.
     */
    void onRestartButtonClicked();

private:
    Ui::MainWindow *ui;  ///< The UI instance.

    ConnectionManager* cm;  ///< The connection manager instance.

    QPushButton *restartButton;  ///< The restart button.

    Ui::ui_menu *menuDialog;  ///< Instance for the menu dialog UI.
    QDialog menuWidget;  ///< The menu dialog widget.
    QDialog rematchDialog;

    bool againstBot;  ///< Indicates if the game is played against a bot.

    QLabel *backgroundLabel;  ///< The background label.

    // Define grid parameters
    int gridSize = 79;  ///< The size of each grid cell.
    int numRows = 8;  ///< The number of rows on the chessboard.
    int numCols = 8;  ///< The number of columns on the chessboard.

    int boardSize = 700;  ///< The size of the chessboard.
    int figureSize = 78;  ///< The size of each chess piece.

    // 2D vector to represent the grid
    QVector<QVector<QRect>> grid;

    /**
     * @brief Initializes the grid.
     */
    void initializeGrid();

    /**
     * @brief Gets the grid coordinates for a given position.
     * @param position The position.
     * @return The grid coordinates.
     */
    QPoint getGridCoordinates(const QPoint &position);

    /**
     * @brief Draws borders for each square on the chessboard.
     */
    void drawGridBorders();

    /**
     * @brief 2D array of QLabel pointers representing the chessboard squares.
     */
    QLabel* gridLabels[8][8];

    /**
     * @brief Selects the player type (against bot or player).
     */
    void selectPlayerType();

    QPoint destinationCoord;  ///< The destination coordinates.
    QPoint sourceCoord;  ///< The source coordinates.

    bool sourceCoordSet;  ///< Indicates if the source coordinates are set.

    /**
     * @brief Restarts the game.
     */
    void restart();

private slots:
    /**
     * @brief Slot to handle the "Play Against Bot" button click.
     */
    void on_playAgainstBotButtonClicked();

    /**
     * @brief Slot to handle the "Play Against Player" button click.
     */
    void on_playAgainstPlayerButtonClicked();

    /**
     * @brief Slot to handle the "Rematch" button click.
     */
    void onRematchButtonClicked();

    /**
     * @brief Slot to handle the "Exit" button click.
     */
    void onExitButtonClicked();
};

#endif // MAINWINDOW_H
