#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "currentlyconverter.h"





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QIcon calcIcon("C:/Users/ceren.cetin/Desktop/projects/qt/calculator/calculator.icon.png"); // İsmi tam olarak bu şekilde güncelledik

    if (calcIcon.isNull()) {
        qDebug() << "HATA: Ikon bulunamadi! Yolunuzu kontrol edin: :/calculator.icon.png";
    } else {
        qDebug() << "BASARILI: Ikon tanindi.";
        ui->button_convert->setIcon(calcIcon); // Buradaki buton isminin dogru olduguna emin ol
        ui->button_convert->setIconSize(QSize(48, 48));
    }



    // Tüm rakam butonlarını tek bir slot'a bağlıyoruz (Pratik yöntem)
    connect(ui->button_0, SIGNAL(clicked()), this, SLOT(rakamBasildi()));
    connect(ui->button_1, SIGNAL(clicked()), this, SLOT(rakamBasildi()));
    connect(ui->button_3, SIGNAL(clicked()), this, SLOT(rakamBasildi()));
    connect(ui->button_4, SIGNAL(clicked()), this, SLOT(rakamBasildi()));
    connect(ui->button_5, SIGNAL(clicked()), this, SLOT(rakamBasildi()));
    connect(ui->button_6, SIGNAL(clicked()), this, SLOT(rakamBasildi()));
    connect(ui->button_7, SIGNAL(clicked()), this, SLOT(rakamBasildi()));
    connect(ui->button_8, SIGNAL(clicked()), this, SLOT(rakamBasildi()));
    connect(ui->button_9, SIGNAL(clicked()), this, SLOT(rakamBasildi()));


    // İşlem butonlarını bağlıyoruz
    connect(ui->button_addition, SIGNAL(clicked()), this, SLOT(islemBasildi()));
    connect(ui->button_minus, SIGNAL(clicked()), this, SLOT(islemBasildi()));
    connect(ui->button_divide, SIGNAL(clicked()), this, SLOT(islemBasildi()));
    connect(ui->button_multiplication, SIGNAL(clicked()), this, SLOT(islemBasildi()));
    connect(ui->button_percent, SIGNAL(clicked()), this, SLOT(islemBasildi()));
    connect(ui->button_negative, SIGNAL(clicked()), this, SLOT(islemBasildi()));
    connect(ui->button_comma, SIGNAL(clicked()), this, SLOT(noktaBasildi()));


    // Eşittir ve Temizle
    connect(ui->button_equal, SIGNAL(clicked()), this, SLOT(esittirBasildi()));
    connect(ui->button_del, SIGNAL(clicked()), this, SLOT(temizleBasildi()));
    connect(ui->button_ac, SIGNAL(clicked()), this, SLOT(temizleBasildi()));

    //convert
     connect(ui->button_convert, SIGNAL(clicked()), this, SLOT(convertFormu()));


}

void MainWindow::rakamBasildi()
{
    // Hangi rakam butonuna basıldığını yakalıyoruz
    QPushButton *button = (QPushButton *)sender();

    double yeniSayi;
    QString yeniEkranDegeri;

    // Eğer ekranda sadece "0" varsa veya yeni bir sayıya başlıyorsak
    if ((ui->lineEdit->text() == "0") || (ikinciSayiYaziliyor)) {
        ui->lineEdit->setText(button->text());
        ikinciSayiYaziliyor = false;
    } else {
        // Eğer 0 değilse, mevcut metnin (sayı veya "2 + ") yanına ekle
        ui->lineEdit->setText(ui->lineEdit->text() + button->text());
    }
}


void MainWindow::islemBasildi()
{
    QPushButton *button = (QPushButton *)sender();
    QString buttonText = button->text();
    QString suAnkiMetin = ui->lineEdit->text();

    // 1. Yüzde işlemi
    if (buttonText == "%") {
        double sayi = ui->lineEdit->text().toDouble();
        sayi = sayi / 100.0;
        ui->lineEdit->setText(QString::number(sayi, 'g', 15));
        return;
    }

    // 2. +/- Negatif Yapma İşlemi
    if (buttonText == "+/-") {
        QString metin = ui->lineEdit->text();
        if (metin.contains(" ")) {
            QStringList parcalar = metin.split(" ", Qt::SkipEmptyParts);
            if (!parcalar.isEmpty()) {
                QString sonParca = parcalar.last();
                if(sonParca.startsWith("(") && sonParca.endsWith(")")) {
                    sonParca = sonParca.mid(2, sonParca.length() - 3);
                } else {
                    sonParca = "(-" + sonParca + ")";
                }
                parcalar.removeLast();
                parcalar.append(sonParca);
                ui->lineEdit->setText(parcalar.join(" "));
            }
        } else {
            if(metin.startsWith("(") && metin.endsWith(")")) {
                ui->lineEdit->setText(metin.mid(2, metin.length() - 3));
            } else {
                ui->lineEdit->setText("(-" + metin + ")");
            }
        }
        return; // İşlem burada biter, aşağıya geçmez.
    }

    // --- ARDA ARDA İŞLEM ENGELLEME MANTIĞI ---
    // Eğer ekran boşsa veya sadece "0" varsa işleme izin verme (Eksi hariç tutulabilir)
    if (suAnkiMetin.isEmpty() || suAnkiMetin == "0") return;

    // Eğer son karakter zaten bir boşluksa (yani bir operatörden sonra geliyorsa)
    // Mevcut operatörü silip yenisini ekleyerek "değiştirme" yapıyoruz.
    if (suAnkiMetin.endsWith(" ")) {
        suAnkiMetin.chop(3); // " + " şeklindeki 3 karakteri (boşluk+operatör+boşluk) siler
    }

    // 3. Standart İşlemler (+, -, x, ÷) ve Parantez Mantığı


    // Görsel Parantez: Çarpma/Bölme ise ve içeride + veya - varsa paranteze al
    if ((buttonText == "x" || buttonText == "÷") &&
        (suAnkiMetin.contains("+") || suAnkiMetin.contains("-"))) {

        if (!suAnkiMetin.startsWith("(")) {
            suAnkiMetin = "(" + suAnkiMetin + ")";
        }
    }

    // Değişkenleri güncelle ve ekrana yazdır
    bekleyenIslem = buttonText;
    ui->lineEdit->setText(suAnkiMetin + " " + buttonText + " ");
    ikinciSayiYaziliyor = false;
}


void MainWindow::esittirBasildi()
{
    QString tamMetin = ui->lineEdit->text();

    // Parantezli negatifleri (-5) temizleyip sayıya çeviren yardımcı lambda
   // auto temizle = [](QString s) {
   //     if(s.startsWith("(") && s.endsWith(")"))
    //        return s.mid(1, s.length() - 2).toDouble();
   //     return s.toDouble();
   // };

    auto temizle = [](QString s) {
        QString saf = s;
        saf.remove('(').remove(')'); // Tüm '(' ve ')' karakterlerini siler
        return saf.toDouble();
    };


    // Boşluklara göre parçala: ["9", "x", "5", "+", "7"]
    QStringList parcalar = tamMetin.split(" ", Qt::SkipEmptyParts);
    if (parcalar.size() < 3) return;

    // --- 1. ADIM: Çarpma ve Bölme İşlemlerini Yap (Öncelik) ---
    for (int i = 0; i < parcalar.size(); ++i) {
        if (parcalar[i] == "x" || parcalar[i] == "÷") {
            double s1 = temizle(parcalar[i - 1]);
            double s2 = temizle(parcalar[i + 1]);
            double araSonuc = 0;

            if (parcalar[i] == "x") araSonuc = s1 * s2;
            else araSonuc = (s2 != 0) ? s1 / s2 : 0;

            // İşlemi yapılan parçaları listeden çıkar ve sonucu oraya koy
            parcalar[i - 1] = QString::number(araSonuc);
            parcalar.removeAt(i);     // Operatörü sil
            parcalar.removeAt(i);     // İkinci sayıyı sil
            i--; // Listeyi kısalttığımız için indeksi geri al
        }
    }

    // --- 2. ADIM: Toplama ve Çıkarma İşlemlerini Yap ---
    double sonuc = temizle(parcalar[0]);
    for (int i = 1; i < parcalar.size(); i += 2) {
        QString op = parcalar[i];
        double s2 = temizle(parcalar[i + 1]);

        if (op == "+") sonuc += s2;
        else if (op == "-") sonuc -= s2;
    }

    // Ekranda işlem önceliğini göstermek istersen sonucu yazdır
    ui->lineEdit->setText(QString::number(sonuc, 'g', 15));
    ikinciSayiYaziliyor = true;
}



void MainWindow::temizleBasildi()
{
    QPushButton *button = (QPushButton *)sender();
    QString buttonText = button->text();

    if (buttonText == "AC") {
        // Tamamen sıfırla
        ilkSayi = 0;
        bekleyenIslem = "";
        ui->lineEdit->setText("0");
        ikinciSayiYaziliyor = false;
    }
    else if (buttonText == "DEL") {
        // Sadece ekrandaki son karakteri sil
        QString ekranMetni = ui->lineEdit->text();

        if (ekranMetni.length() > 0) {
            ekranMetni.chop(1); // Son karakteri keser

            // Eğer her şey silindiyse veya boş kaldıysa "0" göster
            if (ekranMetni.isEmpty() || ekranMetni == "-") {
                ekranMetni = "0";
            }
            ui->lineEdit->setText(ekranMetni);
        }
    }
}

void MainWindow::convertFormu() {
    // 1. Yeni formu oluştur (CurrentlyConverter senin yeni form sınıfının adı olmalı)
    // currentlyconverter.h içindeki class ismine dikkat et
    CurrentlyConverter *yeniPencere = new CurrentlyConverter();

    // 2. Yeni pencereyi göster
    yeniPencere->show();

    // 3. Mevcut MainWindow'u (Hesap Makinesini) kapat
    this->hide();
}

void MainWindow::noktaBasildi()
{
    // Eğer ekranda zaten bir nokta yoksa ekle
    if (!ui->lineEdit->text().contains(".")) {
        ui->lineEdit->setText(ui->lineEdit->text() + ".");
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
