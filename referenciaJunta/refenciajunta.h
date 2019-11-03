#ifndef REFENCIAJUNTA_H
#define REFENCIAJUNTA_H

#include <QMainWindow>

namespace Ui
{
    class refenciajunta;
}

class refenciajunta : public QMainWindow
{
    Q_OBJECT

    public:
        explicit refenciajunta(QWidget *parent = 0);
        ~refenciajunta();

    private slots:

    void on_EJ1T_editingFinished();
    void on_EJ1S_sliderMoved(int position);

    void on_EJ2T_editingFinished();
    void on_EJ2S_sliderMoved(int position);

    void on_EJ3T_editingFinished();
    void on_EJ3S_sliderMoved(int position);

    void on_EJ4T_editingFinished();
    void on_EJ4S_sliderMoved(int position);

    void on_EJ5T_editingFinished();
    void on_EJ5S_sliderMoved(int position);

    void on_EJ6T_editingFinished();
    void on_EJ6S_sliderMoved(int position);

    void resgataSinalInicial();
    bool ativaEnvioRecebimentoDePacotes(int taxaDeAmostragem);
    void on_buttonTente_clicked();

    int tryConnect();

    void on_buttonDesconectar_clicked();

private:
        Ui::refenciajunta *ui;
};

#endif // REFENCIAJUNTA_H
