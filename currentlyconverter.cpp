#include "currentlyconverter.h"
#include "ui_currentlyconverter.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include "mainwindow.h"


CurrentlyConverter::CurrentlyConverter(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CurrentlyConverter)
{
    ui->setupUi(this);

    // 1. Birimleri Listeye Ekle
    QStringList currencies = {"USD", "EUR", "TRY", "GBP", "JPY", "CAD"};
    ui->comboBox_from->addItems(currencies);
    ui->comboBox_to->addItems(currencies);
    ui->comboBox_to->setCurrentText("TRY");

    // 2. İkon Yükleme
    QIcon calcIcon("C:/Users/ceren.cetin/Desktop/projects/qt/calculator/doubleArrow128.png");
    if (!calcIcon.isNull()) {
        ui->button_convertIcon->setIcon(calcIcon);
        ui->button_convertIcon->setIconSize(QSize(48, 48));
    }

    // 3. Network Manager Başlat
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &CurrentlyConverter::onRatesReceived);

    // 4. Sinyalleri Bağla (Döviz Otomatik Çeviri)
    connect(ui->lineEdit_amount, &QLineEdit::textChanged, this, &CurrentlyConverter::updateRates);
    connect(ui->comboBox_from, &QComboBox::currentIndexChanged, this, &CurrentlyConverter::updateRates);
    connect(ui->comboBox_to, &QComboBox::currentIndexChanged, this, &CurrentlyConverter::updateRates);

    // 5. Rakam Butonlarını Bağlama
    QList<QPushButton*> rakamButonlari = {ui->button_0, ui->button_1, ui->button_2, ui->button_3,
                                           ui->button_4, ui->button_5, ui->button_6, ui->button_7,
                                           ui->button_8, ui->button_9};
    for(auto btn : rakamButonlari) connect(btn, &QPushButton::clicked, this, &CurrentlyConverter::rakamBasildi);

    // 6. İşlem Butonlarını Bağlama
    connect(ui->button_addition, &QPushButton::clicked, this, &CurrentlyConverter::islemBasildi);
    connect(ui->button_minus, &QPushButton::clicked, this, &CurrentlyConverter::islemBasildi);
    connect(ui->button_multiplication, &QPushButton::clicked, this, &CurrentlyConverter::islemBasildi);
    connect(ui->button_divide, &QPushButton::clicked, this, &CurrentlyConverter::islemBasildi);
    connect(ui->button_percent, &QPushButton::clicked, this, &CurrentlyConverter::islemBasildi);
    connect(ui->button_negative, &QPushButton::clicked, this, &CurrentlyConverter::islemBasildi);
    connect(ui->button_comma, &QPushButton::clicked, this, &CurrentlyConverter::noktaBasildi);

    // 7. Eşittir ve Temizle Butonları
    connect(ui->button_equal, &QPushButton::clicked, this, &CurrentlyConverter::esittirBasildi);
    connect(ui->button_ac, &QPushButton::clicked, this, &CurrentlyConverter::temizleBasildi);
    connect(ui->button_del, &QPushButton::clicked, this, &CurrentlyConverter::temizleBasildi);
}

// --- HESAPLAMA MANTIĞI ---

void CurrentlyConverter::rakamBasildi() {
    QPushButton *button = (QPushButton *)sender();
    if ((ui->lineEdit_amount->text() == "0") || (ikinciSayiYaziliyor)) {
        ui->lineEdit_amount->setText(button->text());
        ikinciSayiYaziliyor = false;
    } else {
        ui->lineEdit_amount->setText(ui->lineEdit_amount->text() + button->text());
    }
}

void CurrentlyConverter::islemBasildi() {
    QPushButton *button = (QPushButton *)sender();
    QString buttonText = button->text();
    QString suAnkiMetin = ui->lineEdit_amount->text();

    // 1. Yüzde işlemi
    if (buttonText == "%") {
        double sayi = suAnkiMetin.toDouble() / 100.0;
        ui->lineEdit_amount->setText(QString::number(sayi, 'g', 15));
        return;
    }

    // 2. +/- Negatif Yapma
    if (buttonText == "+/-") {
        if(suAnkiMetin.startsWith("(-") && suAnkiMetin.endsWith(")")) {
            ui->lineEdit_amount->setText(suAnkiMetin.mid(2, suAnkiMetin.length() - 3));
        } else {
            ui->lineEdit_amount->setText("(-" + suAnkiMetin + ")");
        }
        return;
    }

    // --- ARDA ARDA İŞLEM ENGELLEME MANTIĞI ---
    // Eğer ekran boşsa veya sadece "0" varsa işleme izin verme (Eksi hariç tutulabilir)
    if (suAnkiMetin.isEmpty() || suAnkiMetin == "0") return;

    // Eğer son karakter zaten bir boşluksa (yani bir operatörden sonra geliyorsa)
    // Mevcut operatörü silip yenisini ekleyerek "değiştirme" yapıyoruz.
    if (suAnkiMetin.endsWith(" ")) {
        suAnkiMetin.chop(3); // " + " şeklindeki 3 karakteri (boşluk+operatör+boşluk) siler
    }

    // 3. İşlem Önceliği / Parantez Mantığı
    if ((buttonText == "x" || buttonText == "÷") && (suAnkiMetin.contains("+") || suAnkiMetin.contains("-"))) {
        if (!suAnkiMetin.startsWith("(")) suAnkiMetin = "(" + suAnkiMetin + ")";
    }

    bekleyenIslem = buttonText;
    ui->lineEdit_amount->setText(suAnkiMetin + " " + buttonText + " ");
    ikinciSayiYaziliyor = false;
}

void CurrentlyConverter::esittirBasildi() {
    QString tamMetin = ui->lineEdit_amount->text();
    auto temizle = [](QString s) {
        QString saf = s;
        saf.remove('(').remove(')');
        return saf.toDouble();
    };

    QStringList parcalar = tamMetin.split(" ", Qt::SkipEmptyParts);
    if (parcalar.size() < 3) return;

    for (int i = 0; i < parcalar.size(); ++i) {
        if (parcalar[i] == "x" || parcalar[i] == "÷") {
            double s1 = temizle(parcalar[i - 1]);
            double s2 = temizle(parcalar[i + 1]);
            double ara = (parcalar[i] == "x") ? (s1 * s2) : (s2 != 0 ? s1 / s2 : 0);
            parcalar[i - 1] = QString::number(ara);
            parcalar.removeAt(i); parcalar.removeAt(i);
            i--;
        }
    }

    double sonuc = temizle(parcalar[0]);
    for (int i = 1; i < parcalar.size(); i += 2) {
        double s2 = temizle(parcalar[i + 1]);
        if (parcalar[i] == "+") sonuc += s2;
        else if (parcalar[i] == "-") sonuc -= s2;
    }

    ui->lineEdit_amount->setText(QString::number(sonuc, 'g', 15));
    ikinciSayiYaziliyor = true;
}

void CurrentlyConverter::temizleBasildi() {
    QPushButton *button = (QPushButton *)sender();
    QString buttonText = button->text();

    if (buttonText == "AC") {
        ui->lineEdit_amount->setText("0");
        ikinciSayiYaziliyor = false;
    } else if (buttonText == "DEL") {
        QString ekranMetni = ui->lineEdit_amount->text();
        if (ekranMetni.length() > 0) {
            ekranMetni.chop(1);
            if (ekranMetni.isEmpty() || ekranMetni == "-") ekranMetni = "0";
            ui->lineEdit_amount->setText(ekranMetni);
        }
    }
}

void CurrentlyConverter::noktaBasildi() {
    if (!ui->lineEdit_amount->text().contains(".")) {
        ui->lineEdit_amount->setText(ui->lineEdit_amount->text() + ".");
    }
}

// --- DÖVİZ MANTIĞI ---

void CurrentlyConverter::updateRates() {
    if (ui->lineEdit_amount->text().isEmpty()) {
        ui->lineEdit_result->clear();
        return;
    }
    // Sadece rakam varken (boşluk yokken) kur çek
    if (ui->lineEdit_amount->text().contains(" ")) return;

    QString fromCurrency = ui->comboBox_from->currentText().toLower();
    QUrl url("http://www.floatrates.com/daily/" + fromCurrency + ".json");
    manager->get(QNetworkRequest(url));
}

void CurrentlyConverter::onRatesReceived(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonObject allRates = QJsonDocument::fromJson(response).object();
        QString targetCurrency = ui->comboBox_to->currentText().toLower();
        double amount = ui->lineEdit_amount->text().toDouble();
        double rate = (ui->comboBox_from->currentText() == ui->comboBox_to->currentText()) ? 1.0 :
                          (allRates.contains(targetCurrency) ? allRates[targetCurrency].toObject()["rate"].toDouble() : 1.0);

        ui->lineEdit_result->setText(QString::number(amount * rate, 'f', 2));
    }
    reply->deleteLater();
}

void CurrentlyConverter::on_button_switch_clicked() {
    int fromIdx = ui->comboBox_from->currentIndex();
    int toIdx = ui->comboBox_to->currentIndex();
    ui->comboBox_from->setCurrentIndex(toIdx);
    ui->comboBox_to->setCurrentIndex(fromIdx);
}



void CurrentlyConverter::returnpage() {
    // 1. Yeni bir MainWindow nesnesi oluştur
    MainWindow *mainWin = new MainWindow();

    // 2. Ana ekranı göster
    mainWin->show();

    // 3. Şu anki döviz ekranını kapat
    this->hide();
}

CurrentlyConverter::~CurrentlyConverter(){
    delete ui;
}

void CurrentlyConverter::on_button_convertIcon_clicked(){
    on_button_switch_clicked();
}


void CurrentlyConverter::on_button_return_clicked()
{
    returnpage();
}

