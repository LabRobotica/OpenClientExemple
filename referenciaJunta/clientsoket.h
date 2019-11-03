#ifndef CLIENTSOKET_H
#define CLIENTSOKET_H

#include <QMainWindow>

namespace Ui
{
    class clientsoket;
}


class clientsocket : public QMainWindow
{
    Q_OBJECT

    public:
        explicit clientsocket(QWidget *parent = 0);
        ~clientsocket();

    private slots:

    private:
        Ui::clientsocket *ui;
};

#endif // CLIENTSOKET_H
