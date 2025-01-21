/**
 * @file ui_rematch.cpp
 *
 * @brief Source file containing the ui_Rematch class
 *
 * @author Adrian Horvath (xhorva14)
 */
#include "ui_rematch.h"
#include "ui_ui_rematch.h"

// Constructor for the ui_Rematch class
ui_Rematch::ui_Rematch(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ui_Rematch)
{
    // Set up the user interface for the rematch widget
    ui->setupUi(this);
}

// Destructor for the ui_Rematch class
ui_Rematch::~ui_Rematch()
{
    // Release the resources associated with the user interface
    delete ui;
}
