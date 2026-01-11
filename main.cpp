#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QNetworkAccessManager manager;

    
    QString city = "Delhi";

    QString apiKey = "333801ee06166454d4256e51afe56f1c";

    QString url = "https://api.openweathermap.org/data/2.5/weather?q=" +
                  city + "&appid=" + apiKey + "&units=metric";

    QNetworkRequest request(url);

    QNetworkReply *reply = manager.get(request);

    
    QObject::connect(reply, &QNetworkReply::finished, [&]() {
        if (reply->error()) {
            qDebug() << "API Error:" << reply->errorString();
            reply->deleteLater();
            a.quit();
            return;
        }

        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);

        if (!doc.isObject()) {
            qDebug() << "Invalid JSON Response";
            reply->deleteLater();
            a.quit();
            return;
        }

        QJsonObject obj = doc.object();

        double temp = obj["main"].toObject()["temp"].toDouble();
        QString weather = obj["weather"].toArray()[0].toObject()["description"].toString();

        qDebug() << "\n--- Weather Report ---";
        qDebug() << "City:" << city;
        qDebug() << "Temperature:" << temp << "°C";
        qDebug() << "Condition:" << weather;

        reply->deleteLater();
        a.quit();
    });

    return a.exec();
}
