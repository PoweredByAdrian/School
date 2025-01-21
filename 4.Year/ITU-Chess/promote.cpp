/**
 * @file promote.cpp
 *
 * @brief Source file containing the Promote UI class
 *
 * @author Adrian Horvath (xhorva14)
 */
#include "promote.h"
#include "ui_promote.h"

// Constructor for the Promote class
Promote::Promote(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Promote)
{
    ui->setupUi(this);

    // Connect signals from buttons to corresponding slots
    connect(ui->pushButton_3, &QPushButton::clicked, this, &Promote::onRookSelected);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &Promote::onBishopSelected);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &Promote::onQueenSelected);
    connect(ui->pushButton, &QPushButton::clicked, this, &Promote::onKnightSelected);
}

// Destructor for the Promote class
Promote::~Promote()
{
    delete ui; // Release resources allocated for the UI
}

// Slot for handling the selection of the Rook piece
void Promote::onRookSelected()
{
    selectedPiece = "rook"; // Set the selected piece to "rook"
    accept();  // Close the dialog and signal acceptance
}

// Slot for handling the selection of the Bishop piece
void Promote::onBishopSelected()
{
    selectedPiece = "bishop"; // Set the selected piece to "bishop"
    accept(); // Close the dialog and signal acceptance
}

// Slot for handling the selection of the Queen piece
void Promote::onQueenSelected()
{
    selectedPiece = "queen"; // Set the selected piece to "queen"
    accept(); // Close the dialog and signal acceptance
}

// Slot for handling the selection of the Knight piece
void Promote::onKnightSelected()
{
    selectedPiece = "knight"; // Set the selected piece to "knight"
    accept(); // Close the dialog and signal acceptance
}
