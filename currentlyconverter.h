#ifndef CURRENTLYCONVERTER_H
#define CURRENTLYCONVERTER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QWidget>

namespace Ui {
class CurrentlyConverter;
}

class CurrentlyConverter : public QWidget
{
    Q_OBJECT

public:
    explicit CurrentlyConverter(QWidget *parent = nullptr);
    ~CurrentlyConverter();

private:
    Ui::CurrentlyConverter *ui;
    QNetworkAccessManager *manager;

    bool ikinciSayiYaziliyor = false;
    QString bekleyenIslem = "";
    double ilkSayi = 0;


private slots:
    void rakamBasildi();
    void islemBasildi();
    void esittirBasildi();
    void temizleBasildi();
    void noktaBasildi();
    void returnpage();

    void updateRates();
    void onRatesReceived(QNetworkReply *reply);
    void on_button_switch_clicked();
    void on_button_convertIcon_clicked();
    void on_button_return_clicked();
};

#endif // CURRENTLYCONVERTER_H
