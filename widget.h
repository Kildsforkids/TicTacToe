#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QApplication>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    int x, y, scale, block;
    char cells[9][9], blocks[9];
    bool turn;
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    int getCurrentBlockNumber(int x, int y);
    int getNextBlockNumber(int x, int y);
    int minBlockBound(int coord);
    int maxBlockBound(int coord);
    bool isBlocked(int blockNum);
    bool isWin(int x, int y, char curPlayer);
    char isGlobalWin();
    bool isDraw(int x, int y);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
