// ! README !
// A kommentelesek egy kulso VsCode extensionnel szineket kapnak es atlathatobbak lesznek (Better Comments, Aaron Bond)
// ! Ezek a kommentek (pirosak) Fontos infot tartalmaznak, figyelmeztetesre vagy esetleges hibara utalnak.
// TODO Ezek a kommentek (narancs) teendoket jelolnek a kodban, altalaban befejezetlen fuggvenynel, vagy modositando dolognal szerepelnek.
// * Ezek a kommentek (zoldek) Adatok, strukturak, es lepesek jelolesere szolgalnak.
// ? Ezek a kommentek (kek) altalaban fuggvenyek / nagyobb lepesek vazlatos leirasa. Egy fuggvenynel vagy egy hosszabb ciklusnal talalhatoak altalaban. 

// ! HA uj adatfile-ra van szukseg, akkor a txtgen_V2.py python kodot kell lefuttatni. Ez egy teljesen uj adatsort general nekunk.

// ! A a program harom faljbol all, az adatkezeles.h-bol, a tevekenysegek.h-bol (ezek a "headerek" mappaban talalhatoak), es a main.c-bol.

#include "./headerek/adatkezeles.h"
#include "./headerek/tevekenysegek.h"
#include <time.h>

typedef enum felhasznalo { // ? Ezek a menu elemei, ezekbol tudja a fuggveny_kereso() switch-je kivalasztani a megfelelo fuggvenyt, itt talalhato, hogy mi mire szolgal.
    rossz_valasz, // * Ha nem letezo menupontot hivott elo (pl 25-ot irt ami nem letezik) Fontos, hogy ez azert nulla, mert az atoi() sikertelen esetben 0-t ad vissza.
    szobakiiras, // * Kiirja egy kert szoba lakoit. (lako_print fuggveny)
    szintkiiras, // * Kiirja a kert szint lakoit (szintprint fuggveny)
    helycsere,  // * Ketto kert embert embert koltoztet el (koltoztetes fuggveny)
    kirug,  // * Egy kert lakot eltavolit a kollegiumbol (kirugas fuggveny)
    melyikszoba,    // * Visszaadja egy adott kartyaszamu ember szobaszamat.
    szabadhelyek,   // * Kiirja a kollegium azon szobait, melyekben vannak ures helyek (osszes_szabad_hely fuggveny)
    buntipont_noveles, // * Egy adott lakonak a buntetopontjait megnoveli egy kert ertekkel (buntetopont_adas fuggveny [es reszben a buntetopont_ellenorzes fuggveny is])
    ujlako, // * Felvesz egy uj lakot a kollegiumba, a megadott adatokkal (ujlako_felvetel es a ujlako_adatfelvetel fuggveny)
    meres, // * Kiirja a legjobb sormerest a kollegiumban (legjobb fuggvenyen osztozik az atlaggal)
    atlag, // * Kiirja a legjobb atlagot a kollegiumban (legjobb fuggvenyen osztozik az meressel)
    kupa, // * Meghivja a qpa fuggvenyt, es egy random esemenyt general, ami lehet pl robbanas, vagy sormeres is
    mentes, // * Kiirja a beolvasott faljba a modositott adatokat. (kiiras fuggveny)
    cls,    // * Letisztitja a kepernyot, hasonloan mint a cls parancs PowerShell-ben (nem hiv elo fuggvenyt)
    kilepes // * Kilep a programbol. Csak ezzel erdemes kilepni, igy biztositott a hasznalt memoria felszabaditasa! (nem hiv elo fuggvenyt)
} felhasznalo;

// ! Fuggvenyek

void fuggveny_kereso(szoba_struct *szoba_parameter, char* filenev, char valasz[], bool* runtime) {
    // ? Megkapja a felhasznalo valaszat, a kert tevekenyseghez felveszi a az adatokat, es atadja a megfelelo fuggvenynek.
    // ? Megkapja a fo adatstrukturat, hogy tovabbadja a tobbi fuggvenynek. (A filenevet a mentes miatt kapja meg.)
    // ? A runtime azert kell, mert igy tudja a fomenu fuggveny ciklusat innen leallitani, ha a felhasznalo ki akar lepni.

        char ujvalasz[NEVMAX]; // * Ebben fogja eltarolni a felvett adatokat.

        switch (atoi(valasz)) {
            case szobakiiras:
                printf("\nSzobaszam: ");
                fgets(ujvalasz, VALASZHOSSZ, stdin);
                enterleszedo(ujvalasz);
                lako_print(szoba_parameter, atoi(ujvalasz), false);
                break;

            case szintkiiras:
                printf("\n Szint: ");
                fgets(ujvalasz, VALASZHOSSZ, stdin);
                enterleszedo(ujvalasz);
                szintprint(szoba_parameter, atoi(ujvalasz));
                break;

            case helycsere:
                printf("\nElso lako kartyaszama: ");
                fgets(ujvalasz, VALASZHOSSZ, stdin);
                enterleszedo(ujvalasz);
                int kartya1 = atoi(ujvalasz);

                printf("\nMasik lako kartyaszama: ");
                fgets(ujvalasz, VALASZHOSSZ, stdin);
                enterleszedo(ujvalasz);
                int kartya2 = atoi(ujvalasz);

                koltoztetes(szoba_parameter, kartya1, kartya2);
                break;

            case kirug:
                printf("\nKartyaszam: ");
                fgets(ujvalasz, VALASZHOSSZ, stdin);
                enterleszedo(ujvalasz);
                kirugas(szoba_parameter, atoi(ujvalasz));
                break;

            case melyikszoba:
                printf("\nKartyaszam: ");
                fgets(ujvalasz, VALASZHOSSZ, stdin);
                enterleszedo(ujvalasz);                
                hol_lakik_kiiras(szoba_parameter, atoi(ujvalasz));
                break;

            case szabadhelyek:
                osszes_szabad_hely(szoba_parameter);
                break;

            case buntipont_noveles:
                printf("\n Kartyaszam: ");
                fgets(ujvalasz, VALASZHOSSZ, stdin);
                enterleszedo(ujvalasz);
                
                printf("\n Mennyi buntetopont: ");
                char be_pont[VALASZHOSSZ];
                fgets(be_pont, VALASZHOSSZ, stdin);
                
                buntetopont_adas(szoba_parameter, atoi(ujvalasz), atoi(be_pont));
                break;

            case ujlako:
                ujlako_adatfelvetel(szoba_parameter);
                break;

            case meres:
                legjobb(szoba_parameter, true);
                break;

            case atlag:
                legjobb(szoba_parameter, false);
                break;

            case kupa:
            printf("\e[1;36m");
            printf("\nA Kollegiumban a lakok QPA hetet szerveztek, es egy heten keresztul megallas nelkul buliztak.\n");
            printf("\e[0;37m");
                qpa(szoba_parameter);
                break;

            case mentes:
                kiiras(filenev, szoba_parameter);
                printf("\nModositasok sikeresen elmentve.");
                break;

            case cls:
                printf("\033[2J\x1b[1;1H"); // * ANSI kod a kepernyo letisztitasahoz.
                printf("Mit szeretnel ma csinalni? (help megmutatja az osszes lehetseges tevekenyseget): ");
                break;

            case kilepes:
                kiiras(filenev, szoba_parameter);
                printf("\nModositasok sikeresen elmentve. ");
                *runtime = false; // * Amint a runtime false lesz, es kilep ebbol a fuggvenybol, es a ciklus is veget er.
                break;

            case rossz_valasz:
                printf("\nSajnos lehetseges hogy el lett utve valami.\n");
                break;

            default:
                printf("\nSajnos lehetseges hogy el lett utve valami.\n");
        }
}

void fomenu(szoba_struct *szoba_parameter, char* filenev) { 
    // ? Ez a fuggveny a fomenut tartalmazza, itt fut a ciklus amiben lehet elohivni tevekenysegeket, es a help parancsot tartalmazza.
    // ? Elohivja a fuggveny_kereso-t ami a megkapott valaszhoz kivalasztja a megfelelo fuggvenyt.
    // ? A szoba_parametert es a filenevet azert kapja meg, hogy kesobb a fuggveny_kereso-nek tovabbadja, az pedig a megfelelo fuggvenynek.
    printf("\n\tUdvozollek a Schonherz Kollegiumban, a lakok sikeresen bekoltoztek.\n");

    char valasz[VALASZHOSSZ]; // * Ebbe keri be a felhasznalo valasztott tevekenyseget.

    printf("\nMit szeretnel ma csinalni? (help megmutatja az osszes lehetseges tevekenyseget):\n");

    bool runtime = true;

    while (runtime)
    // ? Ez a fociklus. A runtime a kilepeskor valik hamissa, addig fut a program (a fuggveny_kereso-ben tortenik ez meg). 
    {
        printf("\n>");
        fgets(valasz, VALASZHOSSZ, stdin);
        enterleszedo(valasz);

        if (strcmp(valasz, "help") == 0) { // * Help funkcio
            // * Ez irja ki a parancsokat. 
            
            printf("\nLehetseges tevekenysegek (csak a szamat kell beirni):\n");
            printf("\t1. Szoba Lakoinak megjelenitese \t2.Szint lakoinak megjelenitese\n");
            printf("\t3.Lakok Koltoztetese \t4.Lako kirugas\n");
            printf("\t5. Lako szobaszama \t6. Szobak ures helyekkel\n"); 
            printf("\t7. Buntetopont adas \t8. Uj lako felvetele  \n");
            printf("\t9. Legjobb sormeres \t10. Legjobb atlag\n");
            printf("\t11.QPA het szervezes \t12. Mentes a beolvasott fileba \n");
            printf("\t13. Kepernyo tisztitas (cls)\t14. Kilepes\n");
        }
        else {
            fuggveny_kereso(szoba_parameter, filenev, valasz, &runtime);
        }
    }
    printf("Viszlat!\n\n");
}

void filenev_olvasas(char* filenev) {
    printf("\nNem sikerult a command line-rol beolvasni az adatfile nevet.\n");
    printf("Adj meg egy filenevet >");
    fgets(filenev, NEVMAX, stdin);
    printf("\n");
    enterleszedo(filenev);
}

void argumentum_kezeles(int argc, char * argv[], char* filenev) {
    // ? Kielemzi a main kapott command-line argumentumait.
    // ? Az elso a debug_log, ilyenkor, csinal log filet, es abba teszi a debugmalloc dump-jat es nem a command line-ba, a masodik a file neve. 

    if (argc > 1) {
        if (strcmp(argv[1], "debug_log") == 0) { // * Letrehoz egy uj log filet. (Ha mar letezik, torol belole minden korabbi szoveget.)
            FILE * logptr;
            logptr = fopen("hazi.log", "w");
            fclose(logptr);
            debugmalloc_log_file("hazi.log");
        }
    }
    if (argc > 2) {
        strcpy(filenev, argv[2]);
    } else {
        filenev_olvasas(filenev);
    }
}

// ! MAIN fuggveny

int main(int argc, char * argv[]) {
    srand(time(NULL)); // * random generalas seedjet beallitjuk a jelenlegi idohoz.
    printf("\n\t\t\tSchonherz Kollegium, keszitette: Krusoczki Adam Ferenc.\n");

    szoba_struct *szoba_adatok; // * Ebben lesz eltarolva az osszes beolvasott adat.
    char *filenev = (char*) malloc((NEVMAX * sizeof(char)));
    argumentum_kezeles(argc, argv, filenev); // * Megkapja a a filenevet a command line-bol, vagy a felhasznalotol

    if (beolvas(filenev, &szoba_adatok) == sikeres) { // * A program beolvasott adat nelkul nem inditja el a fomenut.
        fomenu(szoba_adatok, filenev);
        kuka(&szoba_adatok); // ! Minden adatot kozvetlen a bezaras elott felszabadit.
    }
    free(filenev);
    return 0;    
}
