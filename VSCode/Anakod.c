#include"raylib.h"
#include<stdio.h>
#include<string.h>

#define ENGEL_SAYISI 4

typedef struct {

    Vector2 konum;
    Vector2 hiz;
    Vector2 boyut;
    Color renk;

} NesneVeri;

int MetinOrtala(char metin[], int font_uzunluk, const int ekran_genislik) {

    return (ekran_genislik - MeasureText(metin, font_uzunluk)) / 2;

}

void EngelSifirla(NesneVeri engel[], int i, bool *skoraktif) {

    engel[i].konum = (Vector2) {1045.0f, 295.0f};
    engel[i].hiz = (Vector2) {0.0f, 0.0f};
    engel[i].boyut = (Vector2) {25.0f, 55.0f};
    *skoraktif = false;

}

void OyunuSifirla(NesneVeri *bak, NesneVeri engel[], bool *skoraktif) {

    bak->konum = (Vector2) {50.0f, 310.0f};
    bak->hiz = (Vector2) {0.0f, 0.0f};
    for (int i = 0; i < ENGEL_SAYISI; i++) {

        EngelSifirla(engel, i, &(*skoraktif));

    }

}

void Aktiflik(float *suan, float *engelt, bool *engelaktif) {

    if (*suan - 0.8f >= *engelt) {

        *engelaktif = false;
        
    }

}

void BaklavaGuncelle(NesneVeri *bak, float yercekimi, float ziplama) {

    bak->konum.y += bak->hiz.y;
    if (bak->konum.y != 310.0f) {

        bak->hiz.y += yercekimi;
    } else {

        bak->hiz.y = 0.0f;

    }
    if (IsKeyPressed(KEY_SPACE) && bak->konum.y == 310.0f) {

        bak->hiz.y = -ziplama;

    }

}

void EngelGuncelle(NesneVeri engel[], int *skor, float *engelt, bool *engelaktif, bool *skoraktif) {

    for (int i = 0; i < ENGEL_SAYISI; i++) {

        engel[i].konum.x -= engel[i].hiz.x;
        if (engel[i].konum.x <= 50 - engel[i].boyut.x && !(*skoraktif)) {

            (*skor)++;
            *skoraktif = true;

        }
        if (engel[i].konum.x <= -engel[i].boyut.x) {

            EngelSifirla(engel, i, &(*skoraktif));

        }
        if (!(*engelaktif) && GetRandomValue(1, 100) == 1 && engel[i].konum.x == 1045.0f) {

            float oran = 5.0f;
            switch (GetRandomValue(1, 3)) {

                case 1: break;
                case 2: engel[i].boyut.x += oran; engel[i].boyut.y += oran * 3; 
                        engel[i].konum.y -= oran * 3; break;
                case 3: engel[i].boyut.x += oran * 2; engel[i].boyut.y += oran * 6; 
                        engel[i].konum.y -= oran * 6; break;

            }
            engel[i].hiz.x = 6.0f + 1.0f * (*skor / 15);
            *engelt = GetTime();
            *engelaktif = true;

        }

    }

}

bool Carpisma(NesneVeri *bak, NesneVeri engel[]) {

    Rectangle oyuncu = {bak->konum.x, bak->konum.y, bak->boyut.x, bak->boyut.y};
    for (int i = 0; i < ENGEL_SAYISI; i++) {

        Rectangle e = {engel[i].konum.x, engel[i].konum.y, engel[i].boyut.x, engel[i].boyut.y};
        if (CheckCollisionRecs(oyuncu, e)) {

            return true;

        }

    }
    return false;

}

void SkorKontrol(int *skor, int *maxskor) {

    if (*maxskor == 0) {

        FILE *dosya = fopen("MaxSkor.txt", "r");
        if (dosya != NULL) {

            fscanf(dosya, "%d", *&maxskor);
            fclose(dosya);

        }

    }

    if (*skor > *maxskor) {

        *maxskor = *skor;
        FILE *dosya = fopen("MaxSkor.txt", "w");
        fprintf(dosya, "%d", *maxskor);
        fclose(dosya);
    }
}

void Cizim(NesneVeri *bak, NesneVeri engel[], int *skor, int *maxskor, int kontrol, bool oyun_bitti, 
           const int ekran_genislik) {

    char metin[35];
    BeginDrawing();
        ClearBackground(GRAY);
        if (!oyun_bitti) {

            DrawRectangle(0, 350, 1000, 250, RAYWHITE);
            DrawRectangle(bak->konum.x, bak->konum.y, bak->boyut.x, bak->boyut.y, bak->renk);
            DrawText(TextFormat("Maks Skor: %d", *maxskor), 10, 10, 30, RAYWHITE);
            DrawText(TextFormat("Skor: %d", *skor), 10, 40, 30, RAYWHITE);
            for (int i = 0; i < ENGEL_SAYISI; i++) {

                DrawRectangle(engel[i].konum.x, engel[i].konum.y, engel[i].boyut.x, 
                              engel[i].boyut.y, engel[i].renk);

            }
            if (kontrol == 1) {

                strcpy(metin, "Baslamak icin SPACE'e basin.");
                DrawText(metin, MetinOrtala(metin, 30, ekran_genislik), 250, 30, RAYWHITE);

            }

        } else {

            strcpy(metin, "KAYBETTINIZ!");
            DrawText(metin, MetinOrtala(metin, 50, ekran_genislik), 200, 50, RED);
            strcpy(metin, TextFormat("Skorunuz: %d", *skor));
            DrawText(metin, MetinOrtala(metin, 30, ekran_genislik), 250, 30, RAYWHITE);
            strcpy(metin, TextFormat("Maksimum Skorunuz: %d", *maxskor));
            DrawText(metin, MetinOrtala(metin, 30, ekran_genislik), 280, 30, RAYWHITE);
            strcpy(metin, "Tekrar baslamak icin R'ye basin.");
            DrawText(metin, MetinOrtala(metin, 30, ekran_genislik), 310, 30, RAYWHITE);

        }
    EndDrawing();

}

int main(void) {

    const int ekran_genislik = 1000;
    const int ekran_yukseklik = 600;

    InitWindow(ekran_genislik, ekran_yukseklik, "Kosan Baklava");
    SetTargetFPS(60);

    const float yercekimi = 0.5f;
    const float ziplama = 10.0f;

    int skor = 0, maxskor = 0;
    int kontrol = 1;
    bool oyun_bitti = false;
    float suan, engelt;
    bool engelaktif = false, skoraktif = false;

    SkorKontrol(&skor, &maxskor);

    NesneVeri bak;
    bak.konum = (Vector2) {50.0f, 310.0f};
    bak.hiz = (Vector2) {0.0f, 0.0f};
    bak.boyut = (Vector2) {40.0f, 40.0f};
    bak.renk = BLACK;

    NesneVeri engel[ENGEL_SAYISI];
    for (int i = 0; i < ENGEL_SAYISI; i++) {

        EngelSifirla(engel, i, &skoraktif);
        engel[i].renk = BLACK;

    }

    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_R)) {

            OyunuSifirla(&bak, engel, &skoraktif);
            skor = 0;
            oyun_bitti = false;

        }

        if (!oyun_bitti && IsKeyPressed(KEY_SPACE) && kontrol == 1) {

            kontrol = 0;

        }

        if (!oyun_bitti && kontrol == 0) {

            suan = GetTime();
            BaklavaGuncelle(&bak, yercekimi, ziplama);
            EngelGuncelle(engel, &skor, &engelt, &engelaktif, &skoraktif);
            Aktiflik(&suan, &engelt, &engelaktif);
            if (Carpisma(&bak, engel)) {

                SkorKontrol(&skor, &maxskor);
                kontrol = 1;
                oyun_bitti = true;

            }

        }

        Cizim(&bak, engel, &skor, &maxskor, kontrol, oyun_bitti, ekran_genislik);

    }

    CloseWindow();

    return 0;
}