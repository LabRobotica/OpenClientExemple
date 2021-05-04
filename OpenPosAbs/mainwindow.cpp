#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "timercpp.h"

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

using namespace std;

#define val 100000
string RA1, RA2, RA3, RA4, RA5, RA6;
int OA1=1*val, OA2=2*val, OA3=3*val, OA4=4*val, OA5=5*val, OA6=6*val;


Timer t = Timer();
int sock;

char buf[4096];

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    t.stop();
    delete ui;
}

int MainWindow::tryConnect()
{
    cout << "\nTentando criar um soket... ";
    sock = socket(AF_INET, SOCK_STREAM, 0); //	Create a socket
    if (sock == -1)
    {
        cout << "Ops! Deu errado... \r\n";
        return 1;
    }
    cout << "Criado com sucesso!\r\n";
    ui->progressStatus->setValue(20);

    cout << "Create a hint structure for the server we're connecting with... ";
    int port = ui->boxPort->value();
    string ipAddress = ui->lineIp->text().toUtf8().constData();
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
    cout << "Criado com sucesso!\r\n";
    ui->progressStatus->setValue(40);

    cout << "Connect to the server on the socket... ";
    int connectRes = ::connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connectRes == -1)
    {
        cout << "Could not CONNECT to server! Whoops!\r\n";
        return 1;
    }
    cout << "Criado com sucesso!\r\n";
    ui->progressStatus->setValue(60);

    cout << "Mandando um ok para o servidor... ";
    char buf[4096];
    string userInput="ok";
    int sendRes = send(sock, userInput.c_str(), userInput.size() + 1, 0);
    if (sendRes == -1)
    {
        cout << "Could not send to server! Whoops!\r\n";
        return 1;
    }
    cout << "Ok mandado com sucesso!\r\n";
    ui->progressStatus->setValue(80);

    cout << "Wait for response...";
    int i;
    int bytesReceived;
    for(i=0;i<100;i++)
    {
        memset(buf, 0, 4096);
        bytesReceived = recv(sock, buf, 4096, 0);
        if (bytesReceived == -1)    cout << ".";
        else
        {
            i=-1;
            break;
        }
    }
    if(i==-1)
    {
        cout << "SERVER> " << string(buf, bytesReceived) << "\r\n";
        ui->progressStatus->setValue(100);
        return 0;
    }
    else
    {
        cout << "There was an error getting response from server\r\n";
        return 1;
    }
    return 0;
}
void MainWindow::resgataSinalInicial()
{
    string userInput="l";
    send(sock, userInput.c_str(), userInput.size() + 1, 0);

    char resposta[] = "15500000 15500000 15500000 15500000 15500000 115500000  ";
    memset(buf, 0, 4096);
    int bytesReceived = recv(sock, buf, 4096, 0);
    if ((bytesReceived == -1)||(bytesReceived == 0))
    {
        cout << "There was an error getting response from server\r\n";
    }
    else
    {
        cout << bytesReceived << endl;
        for(int k=0;k<bytesReceived;k++)
        resposta[k]=buf[k];
        std::string::size_type sz;
        char *pedaco;
        cout << resposta << endl;
        try
        {
            pedaco = strtok (resposta," ");
            OA1=std::stoi(pedaco,&sz);
            pedaco = strtok (NULL, " ");
            OA2=std::stoi(pedaco,&sz);;
            pedaco = strtok (NULL, " ");
            OA3=std::stoi(pedaco,&sz);;
            pedaco = strtok (NULL, " ");
            OA4=std::stoi(pedaco,&sz);;
            pedaco = strtok (NULL, " ");
            OA5=std::stoi(pedaco,&sz);;
            pedaco = strtok (NULL, " ");
            OA6=std::stoi(pedaco,&sz);;
            pedaco = strtok (NULL, " ");
        }  catch (int error) {
            cerr << "Error: " << error << endl;
            cerr << resposta << endl;
        }

        ui->RA1S->setValue(OA1);
        ui->RA1B->setValue(((double) OA1)/val);
        ui->RA2S->setValue(OA2);
        ui->RA2B->setValue(((double) OA2)/val);
        ui->RA3S->setValue(OA3);
        ui->RA3B->setValue(((double) OA3)/val);
        ui->RA4S->setValue(OA4);
        ui->RA4B->setValue(((double) OA4)/val);
        ui->RA5S->setValue(OA5);
        ui->RA5B->setValue(((double) OA5)/val);
        ui->RA6S->setValue(OA6);
        ui->RA6B->setValue(((double) OA6)/val);

        ui->OA1S->setValue(OA1);
        ui->OA1B->setValue(((double) OA1)/val);
        ui->OA2S->setValue(OA2);
        ui->OA2B->setValue(((double) OA2)/val);
        ui->OA3S->setValue(OA3);
        ui->OA3B->setValue(((double) OA3)/val);
        ui->OA4S->setValue(OA4);
        ui->OA4B->setValue(((double) OA4)/val);
        ui->OA5S->setValue(OA5);
        ui->OA5B->setValue(((double) OA5)/val);
        ui->OA6S->setValue(OA6);
        ui->OA6B->setValue(((double) OA6)/val);
    }


}

bool MainWindow::ativaEnvioRecebimentoDePacotes(int taxaDeAmostragem)
{
    t.setInterval([&]()
    {
        RA1=std::to_string(ui->RA1S->value());
        RA2=std::to_string(ui->RA2S->value());
        RA3=std::to_string(ui->RA3S->value());
        RA4=std::to_string(ui->RA4S->value());
        RA5=std::to_string(ui->RA5S->value());
        RA6=std::to_string(ui->RA6S->value());

        //Enviar Pacotes Via Rede

        string userInput="p ";
        userInput += RA1;
        userInput += " ";
        userInput += RA2;
        userInput += " ";
        userInput += RA3;
        userInput += " ";
        userInput += RA4;
        userInput += " ";
        userInput += RA5;
        userInput += " ";
        userInput += RA6;
        userInput += "\n";
        send(sock, userInput.c_str(), userInput.size() + 1, 0);

        //aaaaaaaaaaaaa

        char resposta[] = "m 0000000000 0000000000 0000000000 0000000000 0000000000 0000000000";;
        memset(buf, 0, 4096);
        int bytesReceived = recv(sock, buf, 4096, 0);
        if ((bytesReceived == -1)||(bytesReceived == 0))
        {
            cout << "There was an error getting response from server\r\n";
        }
        else
        {
            cout << bytesReceived << endl;
            for(int k=0;k<bytesReceived;k++)
             resposta[k]=buf[k];
            std::string::size_type sz;
            char *pedaco;//string pedaco;
            //cout << stoi(strtok (resposta," "),&sz) << endl;
            try
            {
                pedaco = strtok (resposta," ");
                OA1=std::stoi(pedaco,&sz);
                pedaco = strtok (NULL, " ");
                OA2=std::stoi(pedaco,&sz);;
                pedaco = strtok (NULL, " ");
                OA3=std::stoi(pedaco,&sz);;
                pedaco = strtok (NULL, " ");
                OA4=std::stoi(pedaco,&sz);;
                pedaco = strtok (NULL, " ");
                OA5=std::stoi(pedaco,&sz);;
                pedaco = strtok (NULL, " ");
                OA6=std::stoi(pedaco,&sz);;
                pedaco = strtok (NULL, " ");
            }  catch (int error) {
                cerr << "Error: " << error << endl;
                cerr << resposta << endl;
            }



            //Atualizar Interface Grafica com o Sinal de Saida do Robo
            ui->OA1S->setValue(OA1);
            ui->OA1B->setValue(((double) OA1)/val);
            ui->OA2S->setValue(OA2);
            ui->OA2B->setValue(((double) OA2)/val);
            ui->OA3S->setValue(OA3);
            ui->OA3B->setValue(((double) OA3)/val);
            ui->OA4S->setValue(OA4);
            ui->OA4B->setValue(((double) OA4)/val);
            ui->OA5S->setValue(OA5);
            ui->OA5B->setValue(((double) OA5)/val);
            ui->OA6S->setValue(OA6);
            ui->OA6B->setValue(((double) OA6)/val);
        }
    }, taxaDeAmostragem);

    return 1;
}


void MainWindow::on_buttonConect_clicked()
{
    ui->progressStatus->setValue(0);
    ui->buttonConect->setEnabled(0);
    ui->buttonDisconect->setEnabled(1);

    if(MainWindow::tryConnect()) //Se deu erro
    {
        ui->progressStatus->setValue(0);
        ui->buttonConect->setEnabled(1);
        ui->buttonDisconect->setEnabled(0);
    }
    else
    {
        ui->boxPort->setEnabled(0);
        ui->lineIp->setEnabled(0);
        ui->signal->setEnabled(1);
    }
}

void MainWindow::on_buttonDisconect_clicked()
{
    MainWindow::on_disable_clicked();
    //	Close the socket

    string userInput="\nCLOSE!\n";
    send(sock, userInput.c_str(), userInput.size() + 1, 0);
    ::close(sock);

    ui->progressStatus->setValue(0);
    ui->reference->setEnabled(0);
    ui->out->setEnabled(0);
    ui->buttonDisconect->setEnabled(0);
    ui->buttonConect->setEnabled(1);
    ui->boxPort->setEnabled(1);
    ui->lineIp->setEnabled(1);
    ui->signal->setEnabled(0);
}

void MainWindow::on_enable_clicked()
{
    ui->enable->setEnabled(0);
    ui->disable->setEnabled(1);
    MainWindow::resgataSinalInicial();

    if (MainWindow::ativaEnvioRecebimentoDePacotes(ui->baute->value()))
    {
        ui->reference->setEnabled(1);
        ui->out->setEnabled(1);
    }
}

void MainWindow::on_disable_clicked()
{
    t.stop();
    ui->enable->setEnabled(1);
    ui->disable->setEnabled(0);
    ui->reference->setEnabled(0);
    ui->out->setEnabled(0);
}




void MainWindow::on_RA1B_editingFinished()
{
    ui->RA1S->setValue(ui->RA1B->value()*val);
}

void MainWindow::on_RA2B_editingFinished()
{
    ui->RA2S->setValue(ui->RA2B->value()*val);
}

void MainWindow::on_RA3B_editingFinished()
{
    ui->RA3S->setValue(ui->RA3B->value()*val);
}

void MainWindow::on_RA4B_editingFinished()
{
    ui->RA4S->setValue(ui->RA4B->value()*val);
}

void MainWindow::on_RA5B_editingFinished()
{
    ui->RA5S->setValue(ui->RA5B->value()*val);
}

void MainWindow::on_RA6B_editingFinished()
{
    ui->RA6S->setValue(ui->RA6B->value()*val);
}

void MainWindow::on_RA1S_sliderMoved(int position)
{
    ui->RA1B->setValue(((double)position)/val);
}

void MainWindow::on_RA2S_sliderMoved(int position)
{
    ui->RA2B->setValue(((double)position)/val);
}

void MainWindow::on_RA3S_sliderMoved(int position)
{
    ui->RA3B->setValue(((double)position)/val);
}

void MainWindow::on_RA4S_sliderMoved(int position)
{
    ui->RA4B->setValue(((double)position)/val);
}

void MainWindow::on_RA5S_sliderMoved(int position)
{
    ui->RA5B->setValue(((double)position)/val);
}

void MainWindow::on_RA6S_sliderMoved(int position)
{
    ui->RA6B->setValue(((double)position)/val);
}

