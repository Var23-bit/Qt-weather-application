#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    manager = new QNetworkAccessManager(this);

    connect(manager, &QNetworkAccessManager::finished,
            this, &MainWindow::onApiResult);

    
    fetchWeather();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fetchWeather()
{
    QString city = "Delhi";  
    QString apiKey = "333801ee06166454d4256e51afe56f1c";

    QString url =
        "https://api.openweathermap.org/data/2.5/weather?q=" +
        city + "&appid=" + apiKey + "&units=metric";

    QNetworkRequest req(url);
    manager->get(req);

    qDebug() << "Requesting weather for :" << city;
}

void MainWindow::onApiResult(QNetworkReply *reply)
{
    if (reply->error()) {
        qDebug() << "API Error:" << reply->errorString();
        reply->deleteLater();
        return;
    }

    QByteArray raw = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(raw);

    if (!doc.isObject()) {
        qDebug() << "Invalid API JSON";
        reply->deleteLater();
        return;
    }

    QJsonObject obj = doc.object();

    double temp = obj["main"].toObject()["temp"].toDouble();
    QString weather = obj["weather"].toArray()[0].toObject()["description"].toString();

    qDebug() << "------ Weather Report ------";
    qDebug() << "Temperature:" << temp << "°C";
    qDebug() << "Condition :" << weather;

    reply->deleteLater();
}
