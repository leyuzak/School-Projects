//Leyuza Koksoken 22100011029

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Ders {
    char ad[50];
    int vize;
    int final;
    struct Ders *sonraki;
};

struct Ogrenci {
    char ad[50];
    char soyad[50];
    int numara;
    struct Ders *dersler;
    struct Ogrenci *sonraki; //İç içe struct yapisini kullandim
};

struct Ogrenci *head = NULL; //Bagli listeyi olusturup ilk elemanini NULL yaptim


void ogrenciEkle(char ad[], char soyad[], int numara, struct Ders *dersler) {
    struct Ogrenci *yeniOgrenci = (struct Ogrenci*)malloc(sizeof(struct Ogrenci));

    if (yeniOgrenci == NULL) {
        fprintf(stderr, "Bellek tahsisi yapilamadi.\n");
        return;
    }

    strcpy(yeniOgrenci->ad, ad);
    strcpy(yeniOgrenci->soyad, soyad);
    yeniOgrenci->numara = numara;
    yeniOgrenci->dersler = dersler;
    yeniOgrenci->sonraki = NULL;  //Ogrenci bilgilerini kaydettik

    siraliEkle(yeniOgrenci);
}

void dosyayiGoster(char *dosyaAdi){
    FILE *dosya = fopen(dosyaAdi, "r");
    if (dosya == NULL)
    {
        printf("Dosya acma hatasi\n");
        return;
    }

    char satir[256];
    while (fgets(satir, sizeof(satir), dosya) != NULL)
    {
        printf("%s", satir);
    }

    fclose(dosya);
    }

void listele() {
    struct Ogrenci *gezici = head;
    while (gezici != NULL) {
        printf("Ogrenci: %s %s, Numara: %d\n", gezici->ad, gezici->soyad, gezici->numara);

        struct Ders *ders = gezici->dersler;
        while (ders != NULL) {
            printf("  Ders: %s, Vize: %d, Final: %d\n", ders->ad, ders->vize, ders->final);
            ders = ders->sonraki;
        }
        gezici = gezici->sonraki;
    }
}



void siraliEkle(struct Ogrenci *yeniOgrenci) {
    if (head == NULL || head-> numara>yeniOgrenci->numara) {
        yeniOgrenci->sonraki = head;
        head = yeniOgrenci;
    } else {
        struct Ogrenci *gezici = head;
        while (gezici->sonraki != NULL && gezici->sonraki->numara < yeniOgrenci->numara) {
            gezici = gezici->sonraki;
        }
        yeniOgrenci->sonraki = gezici->sonraki;
        gezici->sonraki = yeniOgrenci; //Ogrencileri kucukten buyuge sirali bir sekilde bagli listeye eklettim
    }
}

void derseGoreListele(char dersAdi[]) {
    struct Ogrenci *ogrenci = head;
    printf("Dersi alan ogrenciler:\n");
    while (ogrenci != NULL) {
        struct Ders *ders = ogrenci->dersler;
        while (ders != NULL) {
            if (strcmp(ders->ad, dersAdi) == 0) {
                printf("Ogrenci: %s %s, Numara: %d, Vize: %d, Final: %d\n",
                    ogrenci->ad, ogrenci->soyad, ogrenci->numara, ders->vize, ders->final);
            }
            ders = ders->sonraki;
        }
        ogrenci = ogrenci->sonraki;
    }
}

void ogrenciyeNumarasiniListele(int numara) {
    struct Ogrenci *ogrenci = head;
    while (ogrenci != NULL) {
        if (ogrenci->numara == numara) {
            printf("Ogrenci: %s %s, Numara: %d\n", ogrenci->ad, ogrenci->soyad, ogrenci->numara);
            struct Ders *ders = ogrenci->dersler;
            while (ders != NULL) {
                printf("Ders: %s, Vize: %d, Final: %d\n", ders->ad, ders->vize, ders->final);
                ders = ders->sonraki;
            }
            return;
        }
        ogrenci = ogrenci->sonraki;
    }
    printf("Ogrenci bulunamadi.\n");
}

void soyadaGoreSil(char soyad[]) {
    struct Ogrenci *gezici = head;
    struct Ogrenci *onceki = NULL;
    int silindimikontrol = 0;

    while (gezici != NULL) {
        if (strcmp(gezici->soyad, soyad) == 0) {
            if (onceki == NULL) {
                head = gezici->sonraki;
            } else {
                onceki->sonraki = gezici->sonraki;
            }

            struct Ders *ders = gezici->dersler;
            while (ders != NULL) {
                struct Ders *tempDers = ders;
                ders = ders->sonraki;
                free(tempDers);
            }

            free(gezici);
            silindimikontrol = 1;
            gezici = (onceki == NULL) ? head : onceki->sonraki;
        } else {
            onceki = gezici;
            gezici = gezici->sonraki;
        }
    }

    if (silindimikontrol) {
        printf("Soyadi '%s' olan tum ogrenciler silindi.\n", soyad);
    } else {
        printf("Soyadi '%s' olan ogrenci bulunamadi.\n", soyad);
    }
}


void dosyadanOku(char *dosyaAdi) {
    FILE *dosya = fopen(dosyaAdi, "r");
    if (dosya == NULL) {
        printf("Dosya acilamiyor.\n");
        return;
    }

    while (1) {
        char ad[50], soyad[50];
        int numara, dersSayisi;

        if (fscanf(dosya, "%s %s %d %d", ad, soyad, &numara, &dersSayisi) != 4) {
            break;  // Eğer gerekli veri sayısı 4 değilse döngüden çık
        }

        struct Ders *ilkDers = NULL;
        struct Ders *sonDers = NULL;

        for (int i = 0; i < dersSayisi; i++) {
            struct Ders *yeniDers = (struct Ders*)malloc(sizeof(struct Ders));
            if (!yeniDers) {
                printf("Bellek tahsisi basarisiz.\n");
                fclose(dosya);
                exit(1);
            }
            fscanf(dosya, "%s %d %d", yeniDers->ad, &yeniDers->vize, &yeniDers->final);
            yeniDers->sonraki = NULL;

            // Dersleri bağlı listeye ekledim
            if (ilkDers == NULL) {
                ilkDers = yeniDers;
                sonDers = yeniDers;
            } else {
                sonDers->sonraki = yeniDers;
                sonDers = yeniDers;
            }
        }

        // Yeni öğrenciyi bağlı listeye ekledim
        ogrenciEkle(ad, soyad, numara, ilkDers);
    }

    fclose(dosya);  // Dosyayı kapat
}


void dosyayaYaz(char *dosyaAdi) {
    FILE *dosya = fopen(dosyaAdi, "r");
    if (dosya == NULL) {
        printf("Dosya acilamiyor.\n");
        return;
    }

    struct Ogrenci *ogrenci = head;
    while (ogrenci != NULL) {
        fprintf(dosya, "%s %s %d ", ogrenci->ad, ogrenci->soyad, ogrenci->numara);

        struct Ders *ders = ogrenci->dersler;
        int dersSayisi = 0;
        while (ders != NULL) {
            dersSayisi++;
            ders = ders->sonraki;
        }
        fprintf(dosya, "%d ", dersSayisi);

        ders = ogrenci->dersler;
        while (ders != NULL) {
            fprintf(dosya, "%s %d %d ", ders->ad, ders->vize, ders->final);
            ders = ders->sonraki;
        }

        fprintf(dosya, "\n");
        ogrenci = ogrenci->sonraki;
    }

    fclose(dosya);
}






int main() {
    dosyadanOku("veriler.txt");

    int secim;
    char soyad[50], dersAdi[50];
    int numara;

    printf("***-------------------------***\n");
    printf("1. Verileri Listele (kucukten buyuge)\n");
    printf("2. Derse Gore Listele\n");
    printf("3. Ogrenciye Gore Listele\n");
    printf("4. Soyada Gore Sil\n");
    printf("5. Ogrenci Ekle\n");
    printf("6. Cikis\n");
    printf("***-------------------------***\n");

    printf("Seciminizi giriniz: ");
    scanf("%d", &secim);

    while (secim != 6) {
        switch (secim) {
            case 1:
                listele();
                break;
            case 2:
                printf("Ders adi giriniz: ");
                scanf("%s", dersAdi);
                derseGoreListele(dersAdi);
                break;
            case 3:
                printf("Ogrenci numarasi giriniz: ");
                scanf("%d", &numara);
                ogrenciyeNumarasiniListele(numara);
                break;
            case 4:
                printf("Silinecek soyadi giriniz: ");
                scanf("%s", soyad);
                soyadaGoreSil(soyad);
                break;
            case 5: {
                char ad[50];
                char soyad[50];
                int numara;
                int dersSayisi;
                listele();

    printf("Ogrenci adi: ");
    scanf("%s", ad);
    printf("Ogrenci soyadi: ");
    scanf("%s", soyad);
    printf("Ogrenci numarasi: ");
    scanf("%d", &numara);
    printf("Ders sayisi: ");
    scanf("%d", &dersSayisi);

    struct Ders *ilkDers = NULL;
    struct Ders *sonDers = NULL;

    for (int i = 0; i < dersSayisi; i++) {
        struct Ders *yeniDers = (struct Ders*)malloc(sizeof(struct Ders));
        if (!yeniDers) {
            printf("Bellek tahsisi basarisiz.\n");
            return; // Sadece mevcut fonksiyondan çýk
        }

        printf("Ders adi: ");
        scanf("%s", yeniDers->ad);
        printf("Vize notu: ");
        scanf("%d", &yeniDers->vize);
        printf("Final notu: ");
        scanf("%d", &yeniDers->final);
        yeniDers->sonraki = NULL;


        if (ilkDers == NULL) {
            ilkDers = yeniDers;
            sonDers = yeniDers;
        } else {
            sonDers->sonraki = yeniDers;
            sonDers = yeniDers;
        }
    }

    ogrenciEkle(ad, soyad, numara, ilkDers);
    break;
}
            default:
                printf("Gecersiz secim.\n");
                break;
        }

        printf("\n***-------------------------***\n");
        printf("1. Verileri Listele\n");
        printf("2. Derse Gore Listele\n");
        printf("3. Ogrenciye Numarasina Gore Listele\n");
        printf("4. Soyada Gore Sil\n");
        printf("5. Ogrenci Ekle\n");
        printf("6. Cikis\n");
        printf("***-------------------------***\n");

        printf("Seciminizi giriniz: ");
        scanf("%d", &secim);
    }

    dosyayaYaz("veriler.txt");
    printf("Guncellenmis dosya icerigi:\n");
    dosyayiGoster("veriler.txt");
    printf("Program bitti.\n");
    return 0;
}
