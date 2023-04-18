#include "mainwindow.h"

MainWindow::MainWindow()
{
    void MainWindow::connects{
        connect(open, &QPushButton::clicked, this, &MainWindow::onOpenImage);
    }

}
