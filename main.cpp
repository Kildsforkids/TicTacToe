#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.resize(w.width(), w.height());
    w.setFixedSize(w.width(), w.height());
    w.setWindowTitle("Tic-tac-toe");
    w.show();

    return a.exec();
}
