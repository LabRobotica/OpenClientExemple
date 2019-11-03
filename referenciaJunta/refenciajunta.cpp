#include "refenciajunta.h"
#include "ui_refenciajunta.h"

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

string EJ1, EJ2, EJ3, EJ4, EJ5, EJ6;
int SJ1=10000, SJ2=20000, SJ3=30000, SJ4=40000, SJ5=50000, SJ6=60000;

Timer t = Timer();
int sock;

char buf[4096];

refenciajunta::refenciajunta(QWidget *parent):QMainWindow(parent),ui(new Ui::refenciajunta)
{
    ui->setupUi(this);
}

refenciajunta::~refenciajunta()
{
    t.stop();
    delete ui;
}



bool refenciajunta::ativaEnvioRecebimentoDePacotes(int taxaDeAmostragem)
{
    t.setInterval([&]()
    {
        EJ1=std::to_string(ui->EJ1S->value());
        EJ2=std::to_string(ui->EJ2S->value());
        EJ3=std::to_string(ui->EJ3S->value());
        EJ4=std::to_string(ui->EJ4S->value());
        EJ5=std::to_string(ui->EJ5S->value());
        EJ6=std::to_string(ui->EJ6S->value());

        //Enviar Pacotes Via Rede

        string userInput="";
        userInput += EJ1;
        userInput += " ";
        userInput += EJ2;
        userInput += " ";
        userInput += EJ3;
        userInput += " ";
        userInput += EJ4;
        userInput += " ";
        userInput += EJ5;
        userInput += " ";
        userInput += EJ6;
        userInput += "\n";
        send(sock, userInput.c_str(), userInput.size() + 1, 0);

        //aaaaaaaaaaaaa

        char resposta[] = "00000 00000 00000 00000 00000 00000";
        memset(buf, 0, 4096);
        int bytesReceived = recv(sock, buf, 4096, 0);
        if (bytesReceived == -1)
        {
            cout << "There was an error getting response from server\r\n";
        }
        else
        {
            for(int k=0;k<bytesReceived;k++)
             resposta[k]=buf[k];
        }

        std::string::size_type sz;
        char * pedaco;//string pedaco;
        pedaco = strtok (resposta," ");
        SJ1=std::stoi(pedaco,&sz);
        pedaco = strtok (NULL, " ");
        SJ2=std::stoi(pedaco,&sz);;
        pedaco = strtok (NULL, " ");
        SJ3=std::stoi(pedaco,&sz);;
        pedaco = strtok (NULL, " ");
        SJ4=std::stoi(pedaco,&sz);;
        pedaco = strtok (NULL, " ");
        SJ5=std::stoi(pedaco,&sz);;
        pedaco = strtok (NULL, " ");
        SJ6=std::stoi(pedaco,&sz);;
        pedaco = strtok (NULL, " ");


        //Atualizar Interface Grafica com o Sinal de Saida do Robo
        ui->SJ1S->setValue(SJ1);
        ui->SJ1T->setValue(SJ1);
        ui->SJ2S->setValue(SJ2);
        ui->SJ2T->setValue(SJ2);
        ui->SJ3S->setValue(SJ3);
        ui->SJ3T->setValue(SJ3);
        ui->SJ4S->setValue(SJ4);
        ui->SJ4T->setValue(SJ4);
        ui->SJ5S->setValue(SJ5);
        ui->SJ5T->setValue(SJ5);
        ui->SJ6S->setValue(SJ6);
        ui->SJ6T->setValue(SJ6);
    }, taxaDeAmostragem);

    return 1;
}

int  refenciajunta::tryConnect()
{
    cout << "\nTentando criar um soket... ";
    sock = socket(AF_INET, SOCK_STREAM, 0); //	Create a socket
    if (sock == -1)
    {
        cout << "Ops! Deu errado... \r\n";
        return 1;
    }
    cout << "Criado com sucesso!\r\n";
    //ui->progressBar->setValue(20);

    cout << "Create a hint structure for the server we're connecting with... ";
    int port = 54000;//ui->boxPort->value();
    string ipAddress = "192.168.76.106";//"127.0.0.1";//ui->lineIp->text();
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
    cout << "Criado com sucesso!\r\n";
    //ui->progressBar->setValue(40);

    cout << "Connect to the server on the socket... ";
    int connectRes = ::connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connectRes == -1)
    {
        cout << "Could not CONNECT to server! Whoops!\r\n";
        return 1;
    }
    cout << "Criado com sucesso!\r\n";
    //ui->progressBar->setValue(60);

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
    //ui->progressBar->setValue(80);

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
        //ui->progressBar->setValue(100);
        return 0;
    }
    else
    {
        cout << "There was an error getting response from server\r\n";
        return 1;
    }
    return 0;
}

void refenciajunta::resgataSinalInicial()
{

    char resposta[] = "00000 00000 00000 00000 00000 00000";
    memset(buf, 0, 4096);
    int bytesReceived = recv(sock, buf, 4096, 0);
    if (bytesReceived == -1)
    {
        cout << "There was an error getting response from server\r\n";
    }
    else
    {
        for(int k=0;k<bytesReceived;k++)
         resposta[k]=buf[k];
    }

    std::string::size_type sz;
    char * pedaco;//string pedaco;
    pedaco = strtok (resposta," ");
    SJ1=std::stoi(pedaco,&sz);
    pedaco = strtok (NULL, " ");
    SJ2=std::stoi(pedaco,&sz);;
    pedaco = strtok (NULL, " ");
    SJ3=std::stoi(pedaco,&sz);;
    pedaco = strtok (NULL, " ");
    SJ4=std::stoi(pedaco,&sz);;
    pedaco = strtok (NULL, " ");
    SJ5=std::stoi(pedaco,&sz);;
    pedaco = strtok (NULL, " ");
    SJ6=std::stoi(pedaco,&sz);;
    pedaco = strtok (NULL, " ");

    ui->EJ1S->setValue(SJ1);
    ui->EJ1T->setValue(SJ1);
    ui->EJ2S->setValue(SJ2);
    ui->EJ2T->setValue(SJ2);
    ui->EJ3S->setValue(SJ3);
    ui->EJ3T->setValue(SJ3);
    ui->EJ4S->setValue(SJ4);
    ui->EJ4T->setValue(SJ4);
    ui->EJ5S->setValue(SJ5);
    ui->EJ5T->setValue(SJ5);
    ui->EJ6S->setValue(SJ6);
    ui->EJ6T->setValue(SJ6);
    ui->SJ1S->setValue(SJ1);
    ui->SJ1T->setValue(SJ1);
    ui->SJ2S->setValue(SJ2);
    ui->SJ2T->setValue(SJ2);
    ui->SJ3S->setValue(SJ3);
    ui->SJ3T->setValue(SJ3);
    ui->SJ4S->setValue(SJ4);
    ui->SJ4T->setValue(SJ4);
    ui->SJ5S->setValue(SJ5);
    ui->SJ5T->setValue(SJ5);
    ui->SJ6S->setValue(SJ6);
    ui->SJ6T->setValue(SJ6);
}

void refenciajunta::on_buttonTente_clicked()
{
    //ui->progressBar->setValue(0);
    //ui->buttonTente->setEnabled(0);
    if(refenciajunta::tryConnect())
    {
        //ui->buttonTente->setEnabled(1);
    }
    else
    {
        refenciajunta::resgataSinalInicial();
        if (refenciajunta::ativaEnvioRecebimentoDePacotes(10))
        {
            ui->entrada->setEnabled(1);
            ui->saida->setEnabled(1);
            //ui->buttonDesconectar->setEnabled(1);
            //ui->buttonTente->setEnabled(0);
        }
    }
}

void refenciajunta::on_buttonDesconectar_clicked()
{
    //	Close the socket
    t.stop();
    string userInput="\nCLOSE!\n";
    send(sock, userInput.c_str(), userInput.size() + 1, 0);

    ::close(sock);
    //ui->progressBar->setValue(0);
    ui->entrada->setEnabled(0);
    ui->saida->setEnabled(0);
    //ui->buttonDesconectar->setEnabled(0);
    //ui->buttonTente->setEnabled(1);

    ui->SJ1S->setValue(0);
    ui->SJ1T->setValue(0);
    ui->SJ2S->setValue(0);
    ui->SJ2T->setValue(0);
    ui->SJ3S->setValue(0);
    ui->SJ3T->setValue(0);
    ui->SJ4S->setValue(0);
    ui->SJ4T->setValue(0);
    ui->SJ5S->setValue(0);
    ui->SJ5T->setValue(0);
    ui->SJ6S->setValue(0);
    ui->SJ6T->setValue(0);
    ui->EJ1S->setValue(0);
    ui->EJ1T->setValue(0);
    ui->EJ2S->setValue(0);
    ui->EJ2T->setValue(0);
    ui->EJ3S->setValue(0);
    ui->EJ3T->setValue(0);
    ui->EJ4S->setValue(0);
    ui->EJ4T->setValue(0);
    ui->EJ5S->setValue(0);
    ui->EJ5T->setValue(0);
    ui->EJ6S->setValue(0);
    ui->EJ6T->setValue(0);
}


void refenciajunta::on_EJ1T_editingFinished()
{
    ui->EJ1S->setValue(ui->EJ1T->value());
}

void refenciajunta::on_EJ1S_sliderMoved(int position)
{
    ui->EJ1T->setValue(position);
}

void refenciajunta::on_EJ2T_editingFinished()
{
    ui->EJ2S->setValue(ui->EJ2T->value());
}

void refenciajunta::on_EJ2S_sliderMoved(int position)
{
    ui->EJ2T->setValue(position);
}

void refenciajunta::on_EJ3T_editingFinished()
{
    ui->EJ3S->setValue(ui->EJ3T->value());
}

void refenciajunta::on_EJ3S_sliderMoved(int position)
{
    ui->EJ3T->setValue(position);
}

void refenciajunta::on_EJ4T_editingFinished()
{
    ui->EJ4S->setValue(ui->EJ4T->value());
}

void refenciajunta::on_EJ4S_sliderMoved(int position)
{
    ui->EJ4T->setValue(position);
}

void refenciajunta::on_EJ5T_editingFinished()
{
    ui->EJ5S->setValue(ui->EJ5T->value());
}

void refenciajunta::on_EJ5S_sliderMoved(int position)
{
    ui->EJ5T->setValue(position);
}

void refenciajunta::on_EJ6T_editingFinished()
{
    ui->EJ6S->setValue(ui->EJ6T->value());
}

void refenciajunta::on_EJ6S_sliderMoved(int position)
{
    ui->EJ6T->setValue(position);
}
