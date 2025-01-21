/**
 * @file ui_menu.cpp
 *
 * @brief Source file containing the ui_menu class
 *
 * @author Adrian Horvath (xhorva14)
 */
#include "ui_menu.h"
#include "ui_ui_menu.h"

// Constructor for the ui_menu class
ui_menu::ui_menu(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ui_menu)
{
    // Set up the user interface for the menu dialog
    ui->setupUi(this);
}

// Destructor for the ui_menu class
ui_menu::~ui_menu()
{
    // Release the resources associated with the user interface
    delete ui;
}
