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

signals:
    void j1(double);
    void j2(double);
    void j3(double);
    void j4(double);
    void j5(double);
    void j6(double);
    void j1_int(int);
    void j2_int(int);
    void j3_int(int);
    void j4_int(int);
    void j5_int(int);
    void j6_int(int);

private slots:
    void on_buttonConect_clicked();
    bool ativaRecebimentoDePacotes(long long taxaDeAmostragem);
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
