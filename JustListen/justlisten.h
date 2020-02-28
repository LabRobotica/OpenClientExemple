#ifndef JUSTLISTEN_H
#define JUSTLISTEN_H

#include <QMainWindow>

namespace Ui {
class JustListen;
}

class JustListen : public QMainWindow
{
    Q_OBJECT

public:
    explicit JustListen(QWidget *parent = 0);
    ~JustListen();

private slots:
    void on_buttonConect_clicked();
    bool ativaRecebimentoDePacotes(int taxaDeAmostragem);
    int  tryConnect();
    void on_buttonDisconect_clicked();

    void on_radioButton_5_clicked();

    void on_radioButton_4_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_1_clicked();

private:
    Ui::JustListen *ui;
};



#endif // JUSTLISTEN_H
