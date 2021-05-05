#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    int tryConnect();
    void resgataSinalInicial();
    bool ativaEnvioRecebimentoDePacotes(int taxaDeAmostragem);

    void on_buttonConect_clicked();

    void on_buttonDisconect_clicked();

    void on_enable_clicked();

    void on_disable_clicked();

    void on_RA1B_editingFinished();

    void on_RA2B_editingFinished();

    void on_RA3B_editingFinished();

    void on_RA4B_editingFinished();

    void on_RA5B_editingFinished();

    void on_RA6B_editingFinished();

    void on_RA6S_sliderMoved(int position);

    void on_RA1S_sliderMoved(int position);

    void on_RA2S_sliderMoved(int position);

    void on_RA3S_sliderMoved(int position);

    void on_RA4S_sliderMoved(int position);

    void on_RA5S_sliderMoved(int position);

    void on_enableSet_clicked();

    void on_disableSet_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
