/****************************************************************************
    AUTHOR: Shaun Katz
            University of Cape Town (UCT)
            Department of Electrical Engineering
            Software Defined Radio Group (SDRG)
    DATE:   02/03/2012
****************************************************************************/

#include <QtGui/QApplication>
#include <QDesktopWidget>
#include "mainwindow.h"
#include "constants.h"


void center(QWidget &widget)
{
  int x, y;
  int screenWidth;
  int screenHeight;
  int width, height;
  QSize windowSize;

  QDesktopWidget *desktop = QApplication::desktop();

  width = widget.frameGeometry().width();
  height = widget.frameGeometry().height();

  screenWidth = desktop->width();
  screenHeight = desktop->height();

  x = (screenWidth - width) / 2;
  y = (screenHeight - height) / 2;

  widget.move( x, y );
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("SDRG");
    a.setOrganizationDomain("sdrg.ee.uct.ac.za");
    a.setApplicationName("Rome");
    a.setApplicationVersion(VERSION);
    //a.setWindowIcon(QIcon(ICON_APPLICATION));
    //a.setFont(QFont("Times", 12));
    MainWindow w;
    w.setWindowTitle("New Design[*] - " + a.applicationName() + " v" + a.applicationVersion());
    w.show();
    center(w);

    return a.exec();
}
