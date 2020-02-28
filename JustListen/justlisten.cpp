#include "justlisten.h"
#include "ui_justlisten.h"

#include <timercpp.h>

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

using namespace std;

int joinValue;

Timer t = Timer();
int sock;
int speed=500;
char buf[4096];

JustListen::JustListen(QWidget *parent):QMainWindow(parent),ui(new Ui::JustListen)
{
    ui->setupUi(this);
}

JustListen::~JustListen()
{
    t.stop();
    delete ui;
}


bool JustListen::ativaRecebimentoDePacotes(int taxaDeAmostragem)
{
    t.setInterval([&]()
    {
        try
        {
            char buf[4096];
            string userInput="l"; // l = listen
            int sendRes = send(sock, userInput.c_str(), userInput.size() + 1, 0);
            if (sendRes == -1)
            {
                cout << "Could not send to server! Whoops!" << endl;
                return 1;
            }
            //Receber Pacote
            char resposta[] = "00000 00000 00000 00000 00000 00000";
            memset(buf, 0, 4096);
            int bytesReceived = recv(sock, buf, 4096, 0);
            if (bytesReceived == -1)
            {
                cout << "There was an error getting response from server" << endl;
            }
            else
            {
                for(int k=0;k<bytesReceived;k++)
                    resposta[k]=buf[k];
            }

            //Desconcaternar Pacotes e Exibir
            std::string::size_type sz;

            joinValue = std::stoi(strtok (resposta," "),&sz);
            ui->SJ1->setValue(joinValue);
            ui->TJ1->setValue(joinValue/100);

            joinValue = std::stoi(strtok (NULL," "),&sz);
            ui->SJ2->setValue(joinValue);
            ui->TJ2->setValue(joinValue/100);

            joinValue = std::stoi(strtok (NULL," "),&sz);
            ui->SJ3->setValue(joinValue);
            ui->TJ3->setValue(joinValue/100);

            joinValue = std::stoi(strtok (NULL," "),&sz);
            ui->SJ4->setValue(joinValue);
            ui->TJ4->setValue(joinValue/100);

            joinValue = std::stoi(strtok (NULL," "),&sz);
            ui->SJ5->setValue(joinValue);
            ui->TJ5->setValue(joinValue/100);

            joinValue = std::stoi(strtok (NULL," "),&sz);
            ui->SJ6->setValue(joinValue);
            ui->TJ6->setValue(joinValue/100);
        }
        catch(int error)
        {
            return 0;
        }
    }, taxaDeAmostragem);

    return 1;
}

int  JustListen::tryConnect()
{
    try
    {
        cout << "\nTentando criar um soket... ";
        sock = socket(AF_INET, SOCK_STREAM, 0); //	Create a socket
        if (sock == -1) throw 1;
        cout << "Criado com sucesso!" << endl;
        ui->boxStatus->setValue(20);

        cout << "Create a hint structure for the server we're connecting with... ";
        int port = ui->boxPort->value();
        string ipAddress = ui->lineIp->text().toUtf8().constData();
        sockaddr_in hint;
        hint.sin_family = AF_INET;
        hint.sin_port = htons(port);
        if(!inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr)) throw 2;
        cout << "Criado com sucesso!" << endl;
        ui->boxStatus->setValue(40);

        cout << "Connect to the server on the socket... ";
        int connectRes = ::connect(sock, (sockaddr*)&hint, sizeof(hint));
        if (connectRes == -1) throw 3;
        cout << "Criado com sucesso!" << endl;
        ui->boxStatus->setValue(60);

        cout << "Mandando um ok para o servidor... ";
        char buf[4096];
        string userInput="ok";
        int sendRes = send(sock, userInput.c_str(), userInput.size() + 1, 0);
        if (sendRes == -1) throw 4;
        cout << "Ok mandado com sucesso!" << endl;
        ui->boxStatus->setValue(80);

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
            cout << "SERVER> " << string(buf, bytesReceived) << endl;
            ui->boxStatus->setValue(100);
        }
        else throw 5;
    }
    catch (int error)
    {
        switch (error)
        {
            case 1:
                cout << "Can't create a soket!" << endl;
                break;
            case 2:
                cout << "Can't create a hint structure!" << endl;
                break;
            case 3:
                cout << "Can't connect the server on the socket!" << endl;
                break;
            case 4:
                cout << "Can't connect to server!" << endl;
                break;
            case 5:
                cout << "There was an error getting response from server!" << endl;
                break;
        }
        return 1;
    }
    return 0;
}




void JustListen::on_buttonDisconect_clicked()
{
    //	Close the socket
    try
    {
        t.stop();
        string userInput="\nCLOSE!\n";
        send(sock, userInput.c_str(), userInput.size() + 1, 0);
        ::close(sock);

        ui->boxStatus->setValue(0);
        ui->buttonDisconect->setEnabled(0);
        ui->buttonConect->setEnabled(1);
        ui->speedBox->setEnabled(1);
        ui->boxPort->setEnabled(1);
        ui->lineIp->setEnabled(1);

        ui->TJ1->setValue(0);
        ui->SJ1->setValue(0);
        ui->TJ2->setValue(0);
        ui->SJ2->setValue(0);
        ui->TJ3->setValue(0);
        ui->SJ3->setValue(0);
        ui->TJ4->setValue(0);
        ui->SJ4->setValue(0);
        ui->TJ5->setValue(0);
        ui->SJ5->setValue(0);
        ui->TJ6->setValue(0);
        ui->SJ6->setValue(0);
    }
    catch (int e)
    {
        cout << "An exception occurred. Exception Nr. " << e << '\n';
    }
}

void JustListen::on_buttonConect_clicked()
{
    if(!JustListen::tryConnect())
    {
        ui->buttonConect->setEnabled(0);
        ui->buttonDisconect->setEnabled(1);
        ui->speedBox->setEnabled(0);
        ui->boxPort->setEnabled(0);
        ui->lineIp->setEnabled(0);
        ativaRecebimentoDePacotes(speed);
    }
    else
    {
        JustListen::on_buttonDisconect_clicked();
    }
}

void JustListen::on_radioButton_1_clicked() {speed = 1;}
void JustListen::on_radioButton_2_clicked() {speed = 2;}
void JustListen::on_radioButton_3_clicked() {speed = 4;}
void JustListen::on_radioButton_4_clicked() {speed = 8;}
void JustListen::on_radioButton_5_clicked() {speed = 16;}
