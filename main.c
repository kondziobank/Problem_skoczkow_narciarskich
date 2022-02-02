#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// struktura reprezentujaca pojedynczego zawodnika
typedef struct Zawodnik {

    char imie[14];
    char nazwisko[14];
    int numer_startowy;
    int skok_1;
    int skok_2;
    int suma_skokow;
} Zawodnik;

// symulacja pojedynczego skoku - pobranie wyniku od uzytkownika
int skok(Zawodnik *zaw) {
    printf("%s %s (%d)\n", zaw->imie, zaw->nazwisko, zaw->numer_startowy);
    printf("Dlugosc skoku (m): ");
    int dlugosc;
    scanf("%d", &dlugosc);
    printf("\n");
    return dlugosc;
}

// ustawia zawodnika na odpowiednie miejsce w tabeli pod wzgledem dlugosci skoku
void aktualizujTabele(Zawodnik **tabela, int n, int seria) {

    int zamiana = 1;
    while (n != 0 && zamiana == 1) {

        // sprawdzenie czy nalezy zamienic zawodnikow
        zamiana = 0;
        if (seria == 1 && tabela[n - 1]->suma_skokow > tabela[n]->suma_skokow)
            zamiana = 1;
        else if (seria == 2 && tabela[n - 1]->suma_skokow < tabela[n]->suma_skokow)
            zamiana = 1;
        else if (tabela[n - 1]->suma_skokow == tabela[n]->suma_skokow) {
            if (tabela[n - 1]->numer_startowy > tabela[n]->numer_startowy)
                zamiana = 1;
        }

        // ewentualna zamiana zawodnikow
        if (zamiana == 1) {

            Zawodnik *z = tabela[n];
            tabela[n] = tabela[n - 1];
            tabela[n - 1] = z;
        }
        n--;
    }
}

int main() {

    // printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

    FILE *lista = fopen("ListaStartowa.txt", "r");
    if (lista == NULL) {
        printf("Nie mozna otworzyc pliku ListaStartowa.txt.\n");
        return 0;
    }

    char nowe_imie[16];
    char nowe_nazwisko[16];
    int nowy_numer_startowy;

    // I seria skokow
    Zawodnik *T1[50]; // wyniki po pierwszej serii
    printf("  I SERIA:\n\n");
    int n = 0; // licznik zawodnikow
    while (fscanf(lista, "%13s%13s%d", nowe_imie, nowe_nazwisko, &nowy_numer_startowy) &&
           !feof(lista)) {

        // zapisywanie nowego skoczka
        T1[n] = malloc(sizeof(Zawodnik));
        strcpy(T1[n]->imie, nowe_imie);
        strcpy(T1[n]->nazwisko, nowe_nazwisko);
        T1[n]->numer_startowy = nowy_numer_startowy;

        // skok
        T1[n]->skok_1 = skok(T1[n]);
        T1[n]->suma_skokow = T1[n]->skok_1;

        // aktualizacja tabeli zawodnikow
        aktualizujTabele(T1, n, 1);
        n++;
    }
    fclose(lista);

    // II seria skokow
    Zawodnik *T2[50];
    printf("  II SERIA\n\n");
    for (int i = 0; i < n; i++) {

        // skok
        T2[i] = T1[i];
        T2[i]->skok_2 = skok(T2[i]);
        T2[i]->suma_skokow += T2[i]->skok_2;

        // aktualizacja tabeli zawodnikow
        aktualizujTabele(T2, i, 2);
    }

    // wyswietlenie wynikow
    printf("\n   NAZWISKO      IMIE       [1 SERIA]  [2 SERIA]  [SUMA]    NR\n");
    printf("===============================================================\n");
    for (int i = 0; i < n; i++) {
        printf("%2d ", i + 1);
        printf("%-13s %-13s", T2[i]->nazwisko, T2[i]->imie);
        printf("%3d m      %3d m  ", T2[i]->skok_1, T2[i]->skok_2);
        printf("   %2d m    %d\n", T2[i]->suma_skokow, T2[i]->numer_startowy);
    }

    // zapis wynikow do pliku
    FILE *wyniki = fopen("Wyniki.txt", "w");
    if (wyniki != NULL) {
        fprintf(wyniki,
                "\n   NAZWISKO      IMIE       [1 SERIA]  [2 SERIA]  [SUMA]    NR\n");
        fprintf(wyniki,
                "===============================================================\n");
        for (int i = 0; i < n; i++) {
            fprintf(wyniki, "%2d ", i + 1);
            fprintf(wyniki, "%-13s %-13s", T2[i]->nazwisko, T2[i]->imie);
            fprintf(wyniki, "%3d m      %3d m  ", T2[i]->skok_1, T2[i]->skok_2);
            fprintf(wyniki, "   %2d m    %d\n", T2[i]->suma_skokow, T2[i]->numer_startowy);
        }
        fclose(wyniki);

    } else
        printf("Nie mozna utworzyc pliku Wyniki.txt.\n");

    // zwalnianie pamieci
    for (int i = 0; i < n; i++)
        free(T1[i]);
    getchar();
    getchar();
    return 0;
}