#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "debugmalloc.h"
#include "adatkezeles.h"

const lako_struct ureslako = {  // * ures/kirugott/elkoltozott lako structja, kirugasnal egyenesen erre valtozik meg. Fontos hogy az ureslakonak a kartyaszama megegyezik az elozo lakonak a kartyaszamaval.
    .nev= "-",
    .kartyaszam = 0, // * Ez rogton megvaltozik, ha kirugjak az eredeti lako kartyaszamara
    .szak = '-',
    .meres = 0.0,
    .atlag = 0.0,
    .buntipont = 0
};

int error_kezeles(int be_error) {
    // ? a be_error mindig egy adott fuggveny visszateresi ertekere utal. (sokszor akar magan a fuggvenyen belul kapja meg ezt a szamot)
    // ? Az errorok enum felhasznalasaval kiirja a megefelelo uzenetet a felhasznalonak.
    // ? Csak az errorok enumban felsorolt visszateresi ertekek vannak a programban. 
    printf("\e[1;31m"); // * Piros szinezes az erroroknak.
    switch (be_error)
    {
    case sikeres:
        break;

    case nem_letezo_file:
        printf("A keresett file nem letezik, vagy nincs ebben a mappaban. A program most leall.\n");
        break;
    
    case nem_letezo_szoba:
        printf("Az adott szoba nem talalhato a kollegium adatbazisaban.\n");
        break;
    
    case nem_letezo_szint:
        printf("Az adott szint nem talalhato a kollegium adatbazisaban.\n");
        break;
    
    case nem_letezo_ember:
        printf("Nem lakik a kollegiumban ilyen nevu, vagy kartyaszamu ember.\n");
        break;
    
    case nincs_szabad_hely:
        printf("Sajnos jelen pillanatban nincs szabad hely a kollegiumban.\n");  
        break;
    
    case szoba_tele:
        printf("Sajnos ez a szoba tele van.\n");  
        break;
    
    case nem_egyezo_adat:
        printf(" Sajnos a megadott adatok hibasak, vagy nem megfelelo tipusuak, kerlek probald ujra.\n");  
        break;
    
    case nem_sikeres_memoriaf:
        printf("Sajnos nem sikerult memoriat foglalni az adatok beolvasasahoz.\n");
        break;
    
    default:
        break;
    }
    printf("\e[0;37m"); // * Itt visszaallitja feherre a betuszint.

    return be_error;
}

int beolvas(char nev[], szoba_struct **szoba_parameter) {
    // ? Itt tortenik a program osszes adatanak beolvasasa a megadott "nev[]" faljba
    // ? A szoba_parameter dupla pointeren keresztul adja vissza a lancolt lista fejenek a pointerjet. 

    FILE *fileptr; // * adatfile pointerje

    fileptr = fopen(nev, "r");

    if (fileptr == NULL) { // * Nem letezo file error
        fclose(fileptr);
        error_kezeles(nem_letezo_file);
        return nem_letezo_file;
    }

    szoba_struct *fej; // * Elso eleme a listanak
    szoba_struct *jelenlegi;  // * Utana mindig ebbe teszi bele az ertekeket
    
    fej = NULL; // * Innen tudjuk majd hogy az elso elemnel jarunk-e meg. 

    char sor[SORMAX]; // * A file egy egesz sorat olvassa be (ami egy teljes szoba adata)

    while (fgets(sor, SORMAX, fileptr) != NULL) // ? Ez a ciklus vezeti le a beolvasast es az adattarolast.
    {
        if (fej == NULL) { // * Ha a lancolt lista elso elemenel vagyunk
            fej = jelenlegi = (szoba_struct*) malloc(sizeof(szoba_struct));
            if (fej == NULL) { // * Ha meg mindig NULL pointer akkor nem sikerult a memoriafoglalas. 
                fclose(fileptr);
                *szoba_parameter = NULL;
                return nem_sikeres_memoriaf;
            }
            *szoba_parameter = fej; // * Itt adjuk vissza a fej pointerjet.
        } else { // * Ha nem a fejnel jarunk mar es itt ugrik at a kovetkezo elemre
            jelenlegi = jelenlegi -> kov = (szoba_struct*) malloc(sizeof(szoba_struct)); 
            jelenlegi -> kov = NULL; // * Itt mindig lenullazza a kovetkezo elem pointerjet, 
        }                            // * mert ha nem tud tovabb olvasni akkor az lesz az utolso elem.

        

        // * Innentol tortenik az adatok feldolgozasa es eltarolasa a jelenlegi szoba_struct es lako struct-ba.

        jelenlegi -> szobaszam = atoi(strtok(sor, LIM));

        for (int i = 0; i < LAKOSZAM; i++) // * itt egy szobahoz tartozo osszes lako adatanak eltarolasa tortenik
        {
            strcpy(jelenlegi -> lakok[i].nev, strtok(NULL, LIM));
            jelenlegi->lakok[i].kartyaszam = atoi(strtok(NULL, LIM));
            jelenlegi -> lakok[i].szak = strtok(NULL, LIM)[0];
            jelenlegi -> lakok[i].meres = strtod(strtok(NULL, LIM), NULL);
            jelenlegi -> lakok[i].atlag = strtod(strtok(NULL, LIM), NULL);
            jelenlegi -> lakok[i].buntipont = atoi(strtok(NULL, LIM));
        }
        jelenlegi -> golyaszoba = strtok(NULL, LIM) [0] == 'I' ? true : false;
    }
    fclose(fileptr);
    return sikeres; 
}

void kiiras(char nev[], szoba_struct *szoba_parameter) {
    // ? Ez a fuggveny felel az adatok kiirasaert ugyanabban a formatumban, ugyanabba a file-ba.
    // ? Fontos hogy a kiiras pontosan ugyanugy tortenjen mint a beolvasas, kulonben a program a kovetkezo beolvasasnal nem fog mukodni.
    
    szoba_struct *jelenlegi = szoba_parameter; // * A lancolt lista fej
    FILE *fileptr; // * A kiirando file pointerje

    fileptr = fopen(nev, "w");

    while (jelenlegi != NULL) { // * Fo ciklus ami vegigfut a SZOBAKON 

        fprintf(fileptr, "%d,", jelenlegi -> szobaszam);

        for (int i = 0; i< LAKOSZAM; i++) // * Masodlagos ciklus ami a LAKOKON fut vegig
        { 
            fprintf(fileptr, "%s,", jelenlegi->lakok[i].nev);
            fprintf(fileptr, "%d,", jelenlegi->lakok[i].kartyaszam);
            fprintf(fileptr, "%c,", jelenlegi->lakok[i].szak);
            fprintf(fileptr, "%.2f,", jelenlegi->lakok[i].meres);
            fprintf(fileptr, "%.2f,", jelenlegi->lakok[i].atlag);
            fprintf(fileptr, "%d,", jelenlegi->lakok[i].buntipont);
        }
        fprintf(fileptr, "%c", jelenlegi -> golyaszoba ? 'I':'N');
        fprintf(fileptr, "\n");
        jelenlegi = jelenlegi -> kov; // *Lepunk a fociklusban a kovetkezo szobara
    }

    fclose(fileptr);
}

void kuka(szoba_struct **szoba_parameter) {
    // ? Ez a fuggveny a beolvasott adatokat tudja a memoriabol felszabaditani.
    // ? Ha a debugmode-ban van a program, akkor ennek sikeresseget a log file mutatja, mas esetben a parancssorba irja ki, hogy volt-e szivargas.
    szoba_struct *jelenlegi = *szoba_parameter;
    szoba_struct *temp = NULL;

    while (jelenlegi != NULL)
    {
        temp = jelenlegi;
        jelenlegi = jelenlegi -> kov;
        free(temp);
    }
}


