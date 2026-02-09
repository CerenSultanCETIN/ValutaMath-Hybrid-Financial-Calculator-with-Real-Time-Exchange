#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void rakamBasildi();      // 0-9 arası rakamlar için
    void islemBasildi();      // +, -, *, / için
    void esittirBasildi();    // Sonuç için
    void temizleBasildi();    // AC butonu için
    void noktaBasildi();      // comma için
    void convertFormu ();


private:
    Ui::MainWindow *ui;
    double ilkSayi;           // İlk girilen sayıyı tutar
    QString bekleyenIslem;    // Hangi işlemin seçildiğini tutar (+, -, vb.)
    bool ikinciSayiYaziliyor; // Kullanıcı ikinci sayıyı girmeye başladı mı?

};
#endif // MAINWINDOW_H

