/****************************************************************************
** SAKARYA ÜnİVERSİTESİ
** BİLGİSAYAR VE BİLİŞİM BİLİMLERİ FAKÜlTESİ
** BİLGİSAYAR MÜhENDİSLİĞİ BÖLÜMÜ
** PROGRAMLAMAYA GİRİŞİ DERSİ
**
** ÖDEV NUMARASI......: 2
** ÖĞRENCİ ADI........: Muhammad Fauzan Asyraf Syahputra
** ÖĞRENCİ NUMARASI...: b181210552
** DERS GRUBU.........: A
****************************************************************************/

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

class Tavuk {
private:
    int yumurtlamaDurumu; // Kaç gün yumurtlamadı
    int gelisGunu;        // Çiftliğe geliş günü
    int toplamYumurta;    // Toplam yumurtladığı yumurta sayısı

public:
    Tavuk(int gun) : yumurtlamaDurumu(0), gelisGunu(gun), toplamYumurta(0) {}

    void yemYe(double &yemMiktari) {
        double tuketim = (rand() % 21 + 100) / 1000.0; // 100-120 gram
        yemMiktari -= tuketim;
    }

    int yumurtla(int gun) {
        if (gun - gelisGunu < 3) {
            yumurtlamaDurumu = 0; // Yeni gelen tavuk 3 gün yumurtlamaz
            return 0;
        }

        int yumurta = rand() % 3; // 0, 1, veya 2 yumurta
        toplamYumurta += yumurta;
        yumurtlamaDurumu = (yumurta == 0) ? yumurtlamaDurumu + 1 : 0;
        return yumurta;
    }

    bool kesimDurumu() {
        return yumurtlamaDurumu >= 3 || toplamYumurta >= 100;
    }

    void yeniTavuk(int gun) {
        yumurtlamaDurumu = 0;
        gelisGunu = gun;
        toplamYumurta = 0;
    }
};

class Ciftlik {
private:
    double kalanPara;
    double yemKgFiyati;
    double yumurtaFiyati;
    double tavukAlimFiyati;
    double yemMiktari;
    vector<Tavuk> tavuklar;

public:
    Ciftlik(double sermaye, int tavukSayisi, double yemFiyat, double yumurtaFiyat, double alimFiyati)
        : kalanPara(sermaye), yemKgFiyati(yemFiyat), yumurtaFiyati(yumurtaFiyat), tavukAlimFiyati(alimFiyati), yemMiktari(700) {
        for (int i = 0; i < tavukSayisi; ++i) {
            tavuklar.emplace_back(0);
        }
        kalanPara -= tavukSayisi * tavukAlimFiyati; // Tavuk alım maliyeti
        kalanPara -= yemKgFiyati * 700;            // Yem alım maliyeti
    }

    void simulasyonYap(int gunSayisi) {
        srand(time(0));

        for (int gun = 1; gun <= gunSayisi; ++gun) {
            double gunlukYemTuketimi = 0;
            int gunlukYumurta = 0;
            double gunlukGelir = 0;
            double gunlukGider = 0;
            vector<int> kesilecekTavuklar;

            // Tavuklar yem yer ve yumurtlar
            for (size_t i = 0; i < tavuklar.size(); ++i) {
                tavuklar[i].yemYe(yemMiktari);
                gunlukYemTuketimi += (yemMiktari < 0) ? 0 : 0.1; // Yem biterse yem kullanılamaz
                gunlukYumurta += tavuklar[i].yumurtla(gun);

                if (tavuklar[i].kesimDurumu()) {
                    kesilecekTavuklar.push_back(i);
                }
            }

            // Yumurtalar satılır
            gunlukGelir += gunlukYumurta * yumurtaFiyati;
            kalanPara += gunlukGelir;

            // Kesilecek tavuklar için işlemler
            for (int index : kesilecekTavuklar) {
                kalanPara += tavukAlimFiyati / 2; // Kesim geliri
                gunlukGider += tavukAlimFiyati;   // Yeni tavuk maliyeti
                tavuklar[index].yeniTavuk(gun);
            }

            // Yem kontrolü
            if (yemMiktari < 70) {
                yemMiktari += 700;
                gunlukGider += yemKgFiyati * 700;
                kalanPara -= yemKgFiyati * 700;
            }

            // Gün sonu kontrol
            if (kalanPara < 0) {
                cout << "Gün: " << gun << "\tIflaş ettiniz..." << endl;
                break;
            }

            // Günlük durum yazdırma
            cout << gun << "\t" << gunlukYemTuketimi << "\t" << gunlukYumurta
                 << "\t" << kesilecekTavuklar.size() << "\t" << gunlukGelir
                 << "\t" << gunlukGider << "\t" << yemMiktari << "\t" << kalanPara << endl;
        }
    }
};

int main() {
    double sermaye, yemKgFiyati, yumurtaFiyati, tavukAlimFiyati;
    int tavukSayisi, gunSayisi;

    cout << "Başlangıç sermayesi: ";
    cin >> sermaye;
    cout << "Tavuk sayısı: ";
    cin >> tavukSayisi;
    cout << "Yem kilogram fiyatı: ";
    cin >> yemKgFiyati;
    cout << "Yumurta fiyatı: ";
    cin >> yumurtaFiyati;
    cout << "Tavuk alım fiyatı: ";
    cin >> tavukAlimFiyati;
    cout << "Simülasyon yapılacak gün sayısı: ";
    cin >> gunSayisi;

    Ciftlik ciftlik(sermaye, tavukSayisi, yemKgFiyati, yumurtaFiyati, tavukAlimFiyati);
    ciftlik.simulasyonYap(gunSayisi);

    return 0;
}
