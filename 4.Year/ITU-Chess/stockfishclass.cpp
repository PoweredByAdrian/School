/**
 * @file stockfish.cpp
 *
 * @brief Source file containing the StockFish class
 *
 * @author Adrian Horvath (xhorva14)
 */
#include "stockfishclass.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
#include <QDebug>

// Constructor for the StockFishClass
StockFishClass::StockFishClass(QObject *parent) : QObject(parent) {}

// Method to get the Stockfish response for a given FEN position
std::string StockFishClass::getStockfishResponse(const std::string &fenPosition)
{
    QNetworkAccessManager manager;
    QEventLoop loop;

    // Replace the following URL with the actual URL of the Stockfish API
    QUrl stockfishUrl("https://stockfish.online/api/stockfish.php");
    QUrlQuery query;
    query.addQueryItem("fen", QString::fromStdString(fenPosition));
    query.addQueryItem("depth", QString::number(getStockfishDepth()));
    query.addQueryItem("mode", "bestmove");
    stockfishUrl.setQuery(query);

    QNetworkRequest request(stockfishUrl);

    QNetworkReply *reply = manager.get(request);

    // Connect signals and slots to handle the network request
    QObject::connect(reply, &QNetworkReply::finished, [&]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
            QJsonObject jsonData = jsonDoc.object();
            QString movesString = jsonData.value("data").toString();
            // Extract the best move coordinates
            QString bestMove = movesString.split(' ').at(1);

            QString fromSquare = bestMove.mid(0, 2);
            QString toSquare = bestMove.mid(2, 2);
            QString movesArray = fromSquare + toSquare;
            // Emit signal to notify the ConnectionManager about the Stockfish move
            emit setMoveNext(movesArray.toStdString());
            qDebug() << "Stockfish analysis:" << jsonData;
        } else {
            qDebug() << "Error:" << reply->errorString();
        }
        loop.quit();
    });

    // Start the event loop to wait for the network request to finish
    loop.exec();

    return "";  // Modify this to return the result you need
}

// Method to get the depth for Stockfish analysis
int StockFishClass::getStockfishDepth()
{
    // Implement this method as needed
    return 5;  // Default depth value, modify as necessary
}
