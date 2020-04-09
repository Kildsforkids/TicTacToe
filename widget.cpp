#include "widget.h"
#include "ui_widget.h"
#include <QtGUI>
#include <QCursor>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    x = 0;
    y = 0;
    block = -1;
    scale = Widget::width()/9;
    turn = 1;
    for (int i = 0; i < 9; i++)
    {
        blocks[i] = 'n';
        for (int j = 0; j < 9; j++)
            cells[i][j] = 'n';
    }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(QPen(Qt::gray, 3, Qt::SolidLine, Qt::FlatCap));
    for (int i = 0, j = 0; i < Widget::width(); i+=scale, j++)
    {
        if (j == 3 || j == 6)
        {
            painter.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::FlatCap));
            painter.drawLine(0, i, Widget::width(), i);
            painter.drawLine(i, 0, i, Widget::width());
            painter.setPen(QPen(Qt::gray, 3, Qt::SolidLine, Qt::FlatCap));
        }
        else
        {
            painter.drawLine(0, i, Widget::width(), i);
            painter.drawLine(i, 0, i, Widget::width());
        }
    }
    for (int i = 0; i < 9; i++)
    {
        if (blocks[i]=='x')
        {
            painter.setPen(QPen(Qt::black, 4, Qt::SolidLine, Qt::FlatCap));
            painter.drawLine(i%3*scale*3, i/3*scale*3, i%3*scale*3+scale*3, i/3*scale*3+scale*3);
            painter.drawLine(i%3*scale*3, i/3*scale*3+scale*3, i%3*scale*3+scale*3, i/3*scale*3);
        }
        if (blocks[i]=='o')
        {
            painter.setPen(QPen(Qt::black, 4, Qt::SolidLine, Qt::FlatCap));
            painter.drawEllipse(i%3*scale*3, i/3*scale*3, scale*3, scale*3);
        }
        if (blocks[i]=='d')
        {
            painter.setPen(QPen(Qt::black, 4, Qt::SolidLine, Qt::FlatCap));
            for (int j = 0; j <= scale*3; j += 50)
                painter.drawLine(i%3*scale*3+j, i/3*scale*3, i%3*scale*3+scale*3, i/3*scale*3+scale*3-j);
        }
    }
    for (int x = 0; x < 9; x++)
    {
        for (int y = 0; y < 9; y++)
        {
            if (cells[x][y] == 'x')
            {
                painter.setPen(QPen(Qt::blue, 3, Qt::SolidLine, Qt::FlatCap));
                painter.drawLine(scale*x+scale/4, scale*y+scale/4, scale*x+scale/4*3, scale*y+scale/4*3);
                painter.drawLine(scale*x+scale/4, scale*y+scale/4*3, scale*x+scale/4*3, scale*y+scale/4);
            }
            if (cells[x][y] == 'o')
            {
                painter.setPen(QPen(Qt::red, 3, Qt::SolidLine, Qt::FlatCap));
                painter.drawEllipse(scale*x+scale/4, scale*y+scale/4, scale/2, scale/2);
            }
        }
    }
    if (block == -1)
    {
        painter.setPen(QPen(Qt::green, 4, Qt::SolidLine, Qt::FlatCap));
        painter.drawRect(0, 0, Widget::width(), Widget::width());
    }
    else
    {
        painter.setPen(QPen(Qt::green, 4, Qt::SolidLine, Qt::FlatCap));
        painter.drawRect(block%3*scale*3, block/3*scale*3, scale*3, scale*3);
    }
    if (isGlobalWin()!='n')
    {
        painter.setPen(QPen(Qt::black, 4, Qt::SolidLine, Qt::FlatCap));
        painter.setFont(QFont("times", Widget::width()/19));
        painter.drawText(Widget::width()/2-Widget::width()/4, Widget::height()/2, "Game Over!");
        Widget::setWindowTitle("Tic-tac-toe: Game Over!");
        Widget::setEnabled(false);
    }
}

int Widget::minBlockBound(int coord)
{
    return coord/3*3;
}

int Widget::maxBlockBound(int coord)
{
    return coord/3*3+2;
}

bool Widget::isDraw(int x, int y)
{
    for (int i = minBlockBound(x); i <= maxBlockBound(x); i++)
        for (int j = minBlockBound(y); j <= maxBlockBound(y); j++)
            if (cells[i][j]=='n') return false;
    return true;
}

char Widget::isGlobalWin()
{
    bool draw = true;
    for (int i = 0; i < 9; i++)
        if (blocks[i]=='n')
        {
            draw = false;
            break;
        }
    if (draw) return 'd';
    if (blocks[0]!='n'&&blocks[0]!='d'&&blocks[0]==blocks[4]&&blocks[0]==blocks[8]) return blocks[0];
    if (blocks[0]!='n'&&blocks[0]!='d'&&blocks[0]==blocks[3]&&blocks[0]==blocks[6]) return blocks[0];
    if (blocks[0]!='n'&&blocks[0]!='d'&&blocks[0]==blocks[1]&&blocks[0]==blocks[2]) return blocks[0];
    if (blocks[1]!='n'&&blocks[1]!='d'&&blocks[1]==blocks[4]&&blocks[1]==blocks[7]) return blocks[1];
    if (blocks[2]!='n'&&blocks[2]!='d'&&blocks[2]==blocks[5]&&blocks[2]==blocks[8]) return blocks[2];
    if (blocks[2]!='n'&&blocks[2]!='d'&&blocks[2]==blocks[4]&&blocks[2]==blocks[6]) return blocks[2];
    if (blocks[6]!='n'&&blocks[6]!='d'&&blocks[6]==blocks[7]&&blocks[6]==blocks[8]) return blocks[6];
    if (blocks[3]!='n'&&blocks[3]!='d'&&blocks[3]==blocks[4]&&blocks[3]==blocks[5]) return blocks[3];
    return 'n';
}

bool Widget::isWin(int x, int y, char curPlayer)
{
    int minX = minBlockBound(x), maxX = maxBlockBound(x);
    int minY = minBlockBound(y), maxY = maxBlockBound(y);
    for (int i = x+1, k = 0; i <= maxX; i++)
    {
        if (cells[i][y]==curPlayer) k++;
        else break;
        if (k==2) return true;
        else
        {
            if (i+1>maxX)
                if (cells[x-1][y]==curPlayer) return true;
        }
    }
    for (int i = x-1, k = 0; i >= minX; i--)
    {
        if (cells[i][y]==curPlayer) k++;
        else break;
        if (k==2)return true;
        else
        {
            if (i-1<minX)
                if (cells[x+1][y]==curPlayer) return true;
        }
    }
    for (int i = y+1, k = 0; i <= maxY; i++)
    {
        if (cells[x][i]==curPlayer) k++;
        else break;
        if (k==2)return true;
        else
        {
            if (i+1>maxY)
                if (cells[x][y-1]==curPlayer) return true;
        }
    }
    for (int i = y-1, k = 0; i >= minY; i--)
    {
        if (cells[x][i]==curPlayer) k++;
        else break;
        if (k==2)return true;
        else
        {
            if (i-1<minY)
                if (cells[x][y+1]==curPlayer) return true;
        }
    }
    for (int i = x+1, j = y+1, k = 0; i <= maxX && j <= maxY; i++, j++)
    {
        if (cells[i][j]==curPlayer) k++;
        else break;
        if (k==2) return true;
        else
        {
            if ((i+1>maxX)&&(j+1>maxY))
                if (cells[x-1][y-1]==curPlayer) return true;
        }
    }
    for (int i = x-1, j = y-1, k = 0; i >= minX && j >= minY; i--, j--)
    {
        if (cells[i][j]==curPlayer) k++;
        else break;
        if (k==2) return true;
        else
        {
            if ((i-1<minX)&&(j-1>minY))
                if (cells[x+1][y+1]==curPlayer) return true;
        }
    }
    for (int i = x+1, j = y-1, k = 0; i <= maxX && j >= minY; i++, j--)
    {
        if (cells[i][j]==curPlayer) k++;
        else break;
        if (k==2) return true;
        else
        {
            if ((i+1>maxX)&&(j-1<minY))
                if (cells[x-1][y+1]==curPlayer) return true;
        }
    }
    for (int i = x-1, j = y+1, k = 0; i >= minX && j <= maxY; i--, j++)
    {
        if (cells[i][j]==curPlayer) k++;
        else break;
        if (k==2) return true;
        else
        {
            if ((i-1<minX)&&(j+1>maxY))
                if (cells[x+1][y-1]==curPlayer) return true;
        }
    }
    return false;
}

bool Widget::isBlocked(int blockNum)
{
    if (blocks[blockNum]=='x'||blocks[blockNum]=='o'||blocks[blockNum]=='d') return true;
    else return false;
}

int Widget::getCurrentBlockNumber(int x, int y)
{
    return x/3+y/3*3;
}

int Widget::getNextBlockNumber(int x, int y)
{
    if (y==0||y==3||y==6)
    {
        if (x==0||x==3||x==6) return 0;
        if (x==1||x==4||x==7) return 1;
        if (x==2||x==5||x==8) return 2;
    }
    if (y==1||y==4||y==7)
    {
        if (x==0||x==3||x==6) return 3;
        if (x==1||x==4||x==7) return 4;
        if (x==2||x==5||x==8) return 5;
    }
    if (y==2||y==5||y==8)
    {
        if (x==0||x==3||x==6) return 6;
        if (x==1||x==4||x==7) return 7;
        if (x==2||x==5||x==8) return 8;
    }
    return -1;
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    x = QWidget::mapFromGlobal(QCursor::pos()).x()/scale;
    y = QWidget::mapFromGlobal(QCursor::pos()).y()/scale;
    int curBlock = getCurrentBlockNumber(x, y);
    if (cells[x][y] != 'x' && cells[x][y] != 'o' && (block == -1 || block == curBlock) && !isBlocked(curBlock))
    {
        char curPlayer;
        if (turn) curPlayer = 'x';
        else curPlayer = 'o';
        cells[x][y] = curPlayer;
        if (isDraw(x, y)) blocks[curBlock] = 'd';
        else
        {
            if (isWin(x, y, curPlayer)) blocks[curBlock] = curPlayer;
        }
        turn = !turn;
        block = getNextBlockNumber(x, y);
        if (isBlocked(block)) block = -1;
        repaint();
    }
}
