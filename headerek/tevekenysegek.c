#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "debugmalloc.h" 
#include "adatkezeles.h"
#include "tevekenysegek.h"


char *szak_kereso(szak szak) {
    // ? Ez a fuggveny visszaadja a megkapott szak teljes nevet egy stringben.
    // ? printf-eknel hasznos, hiszen az adatotbazisban nincsen teljesen kiirva senkinek sem a szakja, csak annak kezdobetuje. 
    if (szak ==  villany) {
        return "Villamosmernok";
    } else if (szak == uzinfo) {
        return "Uzemmernok-Informatikus";
    }  else if (szak == info) {
        return "Mernokinformatikus";
    } else {
        return "-";
    }
}

void lako_print(szoba_struct *szoba_parameter, int szobaszam, bool szinthez) {
    // ? Kiirja a a megkapott szobaszamban lakok nevet, kartyaszamat es buntetopontjat es szakjat.
    // ? A szinthez parameter ha igaz akkor nem ir ki par szoveget, hogy egybefuggo szoveg legyen a szintprint fuggvenynek.

    szoba_struct *jelenlegi = szoba_parameter; 
    
    if(!szinthez && szobaszam != 0) {
        printf("\nA %d. szoba lakoi:\n", szobaszam);
    }
    bool megvan = false;

    while (jelenlegi != NULL)
    {
        if (jelenlegi -> szobaszam == szobaszam) {
            for (int i =0; i < LAKOSZAM; i++) {
                printf("\t%s, SCH kartya: %d, Buntetopont: %d Szak: %s \n ", jelenlegi -> lakok[i].nev, jelenlegi->lakok[i].kartyaszam, jelenlegi->lakok[i].buntipont, szak_kereso(jelenlegi->lakok[i].szak));
            }
            megvan = true;
            break;
        }
        jelenlegi = jelenlegi -> kov;
    }
    if (!megvan) {
        error_kezeles(nem_letezo_szoba);
    }
}

void szintprint(szoba_struct *szoba_parameter, int szint) { 
    // ? Kiirja a megkapott szint osszes lakojat, a lakop_print iteralt elohivasaval.
    if (szint < 1 || szint > 18) {
        error_kezeles(nem_letezo_szint);
        return;
    }
    int elsoszoba = szint*100 + 1; 
    for (int i = elsoszoba; i <= elsoszoba + 15; i++) {
        lako_print(szoba_parameter, i, true);
    }
}

int osszes_szabad_hely(szoba_struct *szoba_parameter) { 
    // ? Ez a fuggveny azokat a szobakat irja ki amelyek rendelkeznek szabad hellyel.
    szoba_struct *jelenlegi = szoba_parameter;
    bool volt_e = false; // * Ez azert van, hogy ha nem volt szabad szoba, akkor mast tudjon kiirni
    
    printf("\nSzobak szabad hellyel:");
    
    while (jelenlegi != NULL) {
        for (int i = 0; i < LAKOSZAM; i++) {
            if (jelenlegi -> lakok[i].nev[0] == '-') {
                printf(" %d", jelenlegi -> szobaszam);
                volt_e = true; 
                break;
            }
        }
        jelenlegi = jelenlegi -> kov;
    }
    printf("\n");
    if (!volt_e) {      
        error_kezeles(nincs_szabad_hely);  // * ez nem feltetlen egy error, inkabb csak ide van gyujtve  
        return nincs_szabad_hely;
    } else {
        return sikeres;
    }
}

int szabad_hely(szoba_struct * szoba_parameter, int szoba, int* kartya) { 
    // ? Ez a fuggveny azt nezi meg, hogy egy adott szobaban van-e szabad hely.
    // ? Ha van, akkor visszaadja az ures hely kartyaszamat a parameterlistan
    szoba_struct* jelenlegi = szoba_parameter;

    while (jelenlegi != NULL)
    {
        if (szoba == jelenlegi -> szobaszam) {
            for (int i = 0; i < LAKOSZAM; i++)
            {
                if (jelenlegi->lakok[i].szak ==ures) {
                    *kartya = jelenlegi->lakok[i].kartyaszam;
                    return sikeres;
                }
            }
            break;
        }
        jelenlegi = jelenlegi -> kov;
    }
    error_kezeles(szoba_tele);
    return szoba_tele;
}

void ujlako_listaba_felvetel(szoba_struct * szoba_parameter, int szoba, char nev[], double sormer, double atlag, szak szak) {
    // ? Ez a fuggveny illeszti be az uj lakot a lancolt listaba. A az adatokat megkapja parameterlistan
    // ? ujlako_adatfelvetel fuggveny intezi az interakciot a felhasznaloval, ez pedig az adatokat tarolja csak el.
       int kartya;
        if (szabad_hely(szoba_parameter, szoba, &kartya) != sikeres) { //* Ha nincs szabad hely a szobaban nem probal felulirni
            return; //* mas adatot
        }

        szoba_struct *koltozo_szobaja;
        int hanyadik;

        hol_lakik(szoba_parameter, kartya, &koltozo_szobaja, &hanyadik);

        koltozo_szobaja -> lakok[hanyadik].atlag = atlag;
        strcpy(koltozo_szobaja -> lakok[hanyadik].nev, nev);
        koltozo_szobaja -> lakok[hanyadik].buntipont = 0;
        koltozo_szobaja -> lakok[hanyadik].meres = sormer;
        koltozo_szobaja -> lakok[hanyadik].szak = szak;

        printf("\n Sikeresen fel lett veve %s, a %d szobaba, matol a kartyaszama: %d", nev, szoba, kartya);
}
void ujlako_adatfelvetel(szoba_struct *szoba_parameter) {
    // ? Ha uj lakot felvetelet szeretnenk vegezni, akkor ez a fuggveny fogja az adatokat bekerni.
    // ? Itt tortenik az adatok validalasa is.
        
        char be_adat[NEVMAX]; // * Ez a felhasznalo beirt adata lesz.
        
        // * Ezeket az adatokat keri be a program
        double sormeres, atlag;
        char nev[NEVMAX];
        int szobaszam;
        szak be_szak;

        osszes_szabad_hely(szoba_parameter); // * Kiirja a szabad szobakat a felhasznalonak.

        printf("\n Szobaszam: ");
        fgets(be_adat, NEVMAX, stdin);
        enterleszedo(be_adat);
        szobaszam = atoi(be_adat);

        // * Itt tudva, hogy milyen szamozast kaphatnak a szobak, leellenorzi, hogy a szoba szama valoban helyesen lett megadva.
        if(szobaszam > 1816 || szobaszam <= 100 || szobaszam % 100 > 16 || szobaszam % 100 == 0) {
            error_kezeles(nem_letezo_szoba);
            return;
        } 

        // * Nevnek igazabol barmit meg lehet adni az mar teljesen a felhasznalotol fugg

        printf("\n Nev: ");
        fgets(be_adat, NEVMAX, stdin);
        enterleszedo(be_adat);
        strcpy(nev, be_adat);

        // * Itt a szakot keri be , es ellenorzi le.

        printf("\n Szak (v/m/u): ");
        fgets(be_adat, NEVMAX, stdin);
        enterleszedo(be_adat);
        be_szak = be_adat[0];

        if (be_szak != villany && be_szak != info && be_szak != uzinfo) {
            error_kezeles(nem_egyezo_adat);
            return;
        }

        // * A sormeres bekerese itt tortenik, 40 masodperc folotti sormerest mar errornak vesz be a program, afolott irrealis mar

        printf("\n Sormeres (2 tizedesjegy pontossagu): ");
        fgets(be_adat, NEVMAX, stdin);
        enterleszedo(be_adat);
        sormeres = strtod(be_adat, NULL);

        if (sormeres < 0.0 || sormeres > 40.0) {
            error_kezeles(nem_egyezo_adat);
            return;
        }
        
        // * Tanulmanyi atlagot keri be, a felso also hatart ellenorizve.

        printf("\n Tanulmanyi atlag (2 tizedesjegy pontossagu): ");
        fgets(be_adat, NEVMAX, stdin);
        enterleszedo(be_adat);
        atlag = strtod(be_adat, NULL);
        
        if (atlag < 1.0 || atlag > 5.0) {
            error_kezeles(nem_egyezo_adat);
            return;
        }

        ujlako_listaba_felvetel(szoba_parameter, szobaszam, nev, sormeres, atlag, be_szak); // * Atadja a fuggvenynek, ami betolti a listaba
}

int hol_lakik(szoba_struct *szoba_parameter, int kartya, szoba_struct **szoba, int* hanyadik) { // * Visszaadja a lako szobajanak pointerjet
    // ? Egyik legfontosabb fuggveny, megkeresi egy adott kartyaszamu lako szobajat, es visszaadja a szobanak a pointerjet, es hogy hanyadik lako az szemely.
    // ? Ha csak a szobara van szuksegunk, akkor a hanyadikat megadhatjuk NULL pointerkent.(de a szoba pointerjenek mindig letezonek kell lennie)
    
    szoba_struct* jelenlegi = szoba_parameter;
    bool volt_e = false; // * Erre azert van szuksegunk, mert lehet, hogy olyan lakot adnak meg aki nem is lakik a kollegiumban.

    while (jelenlegi != NULL)
    {
        for (int i = 0; i < LAKOSZAM; i++)
        {
            if (kartya == jelenlegi -> lakok[i].kartyaszam) { // * egyszerubb kartyaszam alapjan pakolni az embereket
                *szoba = jelenlegi;
                if (hanyadik != NULL) {
                    *hanyadik = i;
                }
                volt_e = true;
                break;
            }
        }
        jelenlegi = jelenlegi -> kov;
    }
    if (!volt_e) {
        error_kezeles(nem_letezo_ember);
        return nem_letezo_ember;
    } else {
        return sikeres;
    }
}

void hol_lakik_kiiras(szoba_struct* szoba_parameter, int kartya) {
    // ? Mivel a hol_lakik fuggveny ugy lett megcsinalva hogy mas fuggvenyeknek legyen a segedfuggvenye, ezert
    // ? ha a felhasznalo keri a lako szobajat, akkor itt tortenik a kiiras
    szoba_struct * jelenlegi;
    int hanyadik;
    
    if(hol_lakik(szoba_parameter, kartya, &jelenlegi, &hanyadik) != sikeres) {
        return;
    }

    printf("\n %s, a %d szobanak, a(z) %d. lakoja", jelenlegi->lakok[hanyadik].nev, jelenlegi->szobaszam, hanyadik + 1);
}

void koltoztetes(szoba_struct *szoba_parameter, int kartya1, int kartya2) { 
    // ? A felhasznalo koltoztetni tud ketto kartyaszamot megadva.
    // ? A fuggveny hagyatkozik arra, hogy a kirugott, vagy be nem koltozott lako helyenek is megvan a kartyaszama,
    // ? Hiszen ezt a szobaban levo helyre adjak. Igy konnyen, a hol_lakik fuggvenyt felhasznalva koltoztet.
    
    szoba_struct* koltozo_1_szoba; // * Ez a ket koltozo szobajanak pointerje. Ebben fogjuk megkapni a szobajukat, a hol_lakik fuggvenybol.
    szoba_struct* koltozo_2_szoba;

    lako_struct koltozo_1; // * koltozo lako adatai ebben lesznek
    lako_struct koltozo_2; // * itt annak az adatai van, akinek a helyere koltozik, ez akar lehet egy ures hely is.
    int k1; // * A szobaban levo lakok intexei a tombben
    int k2;    
    
    // * Leellenorizzuk, hogy valos adatokat adott e meg a felhasznalo 
    if (hol_lakik(szoba_parameter, kartya1, &koltozo_1_szoba, &k1) != sikeres  || hol_lakik(szoba_parameter, kartya2, &koltozo_2_szoba, &k2) != sikeres ) { // * Fontos hogy a kollegiumban lakjon ez az ember. Es igy meg is kapjuk a szobaszamat.  
        return;
    }

    // * Itt kicsereljuk a ket lakot
    koltozo_1 = koltozo_1_szoba->lakok[k1];
    koltozo_2 = koltozo_2_szoba->lakok[k2];
    
    koltozo_1_szoba->lakok[k1] = koltozo_2;
    koltozo_2_szoba->lakok[k2] = koltozo_1;

    printf("\n\nA Koltoztetes sikeres volt.");
    
}

void kirugas(szoba_struct *szoba_parameter, int kartya) {
    // ? Ez a fuggveny eltavolit a kollegiumbol egy adott kartyaszamu szemelyt.
    
    szoba_struct *szoba;
    int hanyadik;

    // * megadott kartyaszam ellenorzese
    if (hol_lakik(szoba_parameter, kartya, &szoba, &hanyadik) != sikeres) {
        return;
    }

    // * ellenorizzuk azt hogy a kartyaszam van e valakinek kiosztva
    if (szoba -> lakok[hanyadik].kartyaszam == kartya && szoba -> lakok[hanyadik].szak == ures) {
        error_kezeles(nem_letezo_ember);
        return;
    }
    // * kicsereljuk a lakot ureslako-ra
    szoba -> lakok[hanyadik] = ureslako;
    szoba -> lakok[hanyadik].kartyaszam = kartya;

    printf("\nA lako sikeresen ki lett rugva.");
}

void buntetopont_ellenorzes(szoba_struct *szoba_pointer, int hanyadik) {
    //? Ez a fuggveny megnezi, hogy egy lakonak osszegyult-e mar a 20 buntetopont
    // ? Ez nem a lancolt listat kapja meg parameternek, hanem a lako szobajanak pointerjet es azon belul a lako indexet,
    // ? mivel ez altalaban a hol_lakik fuggvennyel van egyutt hasznalva 
    //? itt nincs szukseg arra, hogy megnezzuk, hogy itt lakik-e, mivel az ezt meghivo fuggvenyek mar megnezik ezt
    szoba_struct *jelenlegi = szoba_pointer;

    if (jelenlegi->lakok[hanyadik].buntipont >= 20) {
        printf("\n%s (%d) nevu lakonak osszegyultek a buntetopontjai, ezert kirugasra kerul.",jelenlegi->lakok[hanyadik].nev, jelenlegi->lakok[hanyadik].kartyaszam);
        kirugas(szoba_pointer, jelenlegi->lakok[hanyadik].kartyaszam);
    }

}

void buntetopont_adas(szoba_struct *szoba_parameter, int kartya, int pont) {
    // ? Ez a fuggveny a megkapott szamu buntetopontot kiosztja egy lakonak.
    szoba_struct *lako_szobaptr;
    int hanyadik;
    
    // * Megkeressuk, es egyben ellenorizzuk, hogy van-e ilyen lako
    if(hol_lakik(szoba_parameter, kartya, &lako_szobaptr, &hanyadik) != sikeres) {
        return;
    }
    // * Ne adjunk nem letezo embernek buntetopontot
    if (lako_szobaptr -> lakok[hanyadik].kartyaszam == kartya && lako_szobaptr -> lakok[hanyadik].szak == ures) {
        error_kezeles(nem_letezo_ember);
        return;
    }
    // * 20 felett mar mindegy mit ad a felhasznalo igyis ugyis kurgja a szemelyt
    if (pont >= 20) {
        lako_szobaptr->lakok[hanyadik].buntipont = 20;
    } else if (pont < 0) { // * viszont 0 pont alatti buntetopont nme letezik
        error_kezeles(nem_egyezo_adat);\
        return;
    } else { // * ez pedig a normal eset
        lako_szobaptr->lakok[hanyadik].buntipont += pont;
    }

    printf("\nSikeresen megkapta %s (%d) a buntetopontokat. (Jelenlegi pontjai: %d)", lako_szobaptr->lakok[hanyadik].nev, kartya, lako_szobaptr->lakok[hanyadik].buntipont);
    buntetopont_ellenorzes(lako_szobaptr, hanyadik);
}

void enterleszedo(char* szoveg) {
    // ? Megkap egy string-et es leszedi rola a '\n' karaktert, visszaternie nem kell hiszen parameterlistan visszaadja a stringet
    for (int i = 0; szoveg[i] != '\0'; i++)
    {
        if (szoveg[i] == '\n') {
            szoveg[i] = '\0';
            break;
        }
    }
}

void legjobb(szoba_struct *szoba_parameter, bool sormeres) {
    // ? Ez a fuggveny a legjobb sormerest vagy atlagot keresi meg (ha a sormeres hamis akkor keresi az atlagot, mas esetben a sormerest fogja)
    // ? Azert lett a ketto kereses egy fuggvenyben mert ugyanaz a folyamat csak mas adattal.
    
    szoba_struct* jelenlegi = szoba_parameter;
    double legjobb;
    sormeres ? legjobb = 40.0 : 1.0; //* Fuggoen attol hogy sormerest vagy atlagot keresunk, megadjuk a legrosszabb esetet
                                     //* es ugy keressuk meg a maximumot az adatszerkezetben

    char nev[NEVMAX];
    int kartya;
    while (jelenlegi != NULL)
    {
        for (int i = 0; i < LAKOSZAM; i++)
        {   // * Nem teljesen hatekony, hiszen sok embert kell megneznie a legrosszabb eset megadasa miatt de biztos mukodesu
            // * itt a sormerest nezzuk
            if (sormeres && jelenlegi->lakok[i].meres < legjobb && jelenlegi->lakok[i].szak != ures)
            {
                legjobb = jelenlegi->lakok[i].meres;
                strcpy(nev, jelenlegi->lakok[i].nev);
                kartya = jelenlegi->lakok[i].kartyaszam;
            }
            // * itt az atlagot nezzuk
            if (!sormeres && jelenlegi->lakok[i].atlag > legjobb && jelenlegi->lakok[i].szak != ures)
            {
                legjobb = jelenlegi->lakok[i].atlag;
                strcpy(nev, jelenlegi->lakok[i].nev);
                kartya = jelenlegi->lakok[i].kartyaszam;
            }
        }
        jelenlegi = jelenlegi->kov;
    }
    if (sormeres) {
        printf("A legjobb sormeres a kollegiumban : \t %s (%d): %.2lf \n", nev, kartya, legjobb);
    } else {
        printf("A legjobb atlag a kollegiumban : \t %s (%d): %.2lf \n", nev, kartya, legjobb);
    }
}

int random_generator(int felso, int also) {
    // ? Megkap egy also es egy felso hatart es akozott general egy random szamot majd visszater ezzel a szammal.
    // ? Azert van szukseg itt az idore, hiszen az lesz a random szam generator seedje.
    return (rand() % (felso - also + 1)) + also; // * visszaad egy also es felso korlat kozotti intet. (a kartyaszamok miatt fontos).
}

void qpa(szoba_struct *szoba_parameter) {
    // ? A qpa igazabol egy nagy random esemeny generator easter egg, nem feltetlen kupa alatt megortent dolgokat tartalmaz
    // ? inkabb viccesebb szituaciokat general
    // ? Ez egy hosszu fuggveny, mivel a random esemenyektol fuggoen vegzi a dolgokat.

    int esemeny = random_generator(sormeres, robbanas); // * itt kisorsolja az elkovetkezo esemenyt/katasztrofat

    int elso = szoba_parameter->lakok[0].kartyaszam; // * Itt a random generalt kartyaszamok also korlatjat taroljuk
    int utolso; // * ebben pedig a legnagyobb kartyaszamot keressuk meg, hogy felso korlatnak hasznaljuk

    szoba_struct *jelenlegi = szoba_parameter;

    while (jelenlegi != NULL)
    {
        if (jelenlegi->kov == NULL)
        {
            utolso = jelenlegi->lakok[3].kartyaszam; // * itt megtalaltuk az utolso embert
            break;
        }
        jelenlegi = jelenlegi->kov;
    }
    // ? Ha csak egy atlagos sormeres tortenik kupa alatt
    if (esemeny == sormeres) {
        int sormero1; //* elso sormeronk
        int sormero2; //* masodik sormeronk
        
        szoba_struct *sormero1_szoba; //* letrehozzuk a sormerok szobajanak a pointerjeit
        szoba_struct *sormero2_szoba;
        int si; // * Az elso sormero indexe (sorszama) a szobajaban [sormero_i-t rovidit]
        int sj; // * A masodik sormero indexe (sorszama) a szobajaban [sormero_j-t rovidit]
        
        int itt_lakik = ures; // * Ezt a valtozot arra hasznaljuk, hogy, ha a generalt kartyaszam egy kirugott embere,
        int itt_lakik2 = ures; // * akkor keressunk egy ujat (addig keresunk egy sormerot, amig annak a szakja helyen valodi szak nem lesz)


        while (itt_lakik == ures) //* generaljuk a random lakok kartyaszamat, amig egy valodit nem kapunk
        {
            sormero1 = random_generator(utolso, elso); 
            hol_lakik(szoba_parameter, sormero1, &sormero1_szoba, &si); //* keresunk egy elso lakot
            itt_lakik = sormero1_szoba->lakok[si].szak;
        }
        while (itt_lakik2 == ures)
        {
            sormero2 = random_generator(utolso, elso);
            hol_lakik(szoba_parameter, sormero2, &sormero2_szoba, &sj);
            itt_lakik2 = sormero2_szoba->lakok[sj].szak;
        }

        printf("\n%s (%d) kihivta sormeresre %s (%d) lakot!\n", sormero1_szoba->lakok[si].nev,sormero1_szoba->szobaszam, sormero2_szoba->lakok[sj].nev,sormero2_szoba->szobaszam);

        if (rand() % 2 == 0) { // * Itt random nyertesse tesszuk az egyikojuket, es csokkentjuk a sormereset, egy adott minimum pontig (1.5 mp)
            double ujmeres = 1.5 + (sormero1_szoba->lakok[si].meres - 1.5) * 0.25;
            printf("%s nyert, az sormerese %lf-rol javult %lf-re", sormero1_szoba->lakok[si].nev, sormero1_szoba->lakok[si].meres, ujmeres);
            sormero1_szoba->lakok[si].meres = ujmeres; 
        }
        else {
            double ujmeres = 1.5 + (sormero2_szoba->lakok[sj].meres - 1.5) * 0.25;
            printf("%s nyert, az sormerese %lf-rol javult %lf-re", sormero2_szoba->lakok[sj].nev, sormero2_szoba->lakok[sj].meres, ujmeres);
            sormero2_szoba->lakok[sj].meres = ujmeres;
        }
    }
    if (esemeny == robbanas) {
        // ? Ez az az eset, ha a kupa alatt valakik egy mikrot robbantanak fel, ittas allapotban
        szoba_struct* robbant_szoba;
        int megfelelt_e = false; // * A random generalas miatt megvan az esely arra, hogy olyan szoba lesz amit mar teljesen kirugtak.
        // ! itt nem random szobat generalunk, mivel a szobaknak a szamozasa nem 1-99-ig megy a szinteken hanem 1-16-ig ezert egy kartyaszamot general es annak a szobajat nezi meg
        
        while (!megfelelt_e) // * Erre a ciklusra azert van szukseg, mert generalhat olyan szobat is amiben nem lakik senki
        {                    // * Ami nem ad errort/faultot, lehetseges dolog, de szerintem nem nezne ki jol.
        int random_szemely = random_generator(utolso, elso);
         hol_lakik(szoba_parameter,random_szemely, &robbant_szoba, NULL); 
         int uresdb = 0;
         for (int i = 0; i < LAKOSZAM; i++)
         {
            if (robbant_szoba->lakok[i].szak == ures)
            {
                uresdb += 1; // * ebben taroljuk el hogy hany ures lako van, ha megvan a 4 ujat generalunk.
            }
         }
            if (uresdb != 4)
            {
                megfelelt_e = true;
            }
            
            uresdb = 0;
        } 

        printf("\nJaj ne! A QPA alatt eldurvult az %d szobaban a buli es felrobbantottak a mikrojukat! A szoba osszes lakoja kirugrasra kerult.", robbant_szoba->szobaszam);
        
        for (int i = 0; i < LAKOSZAM; i++)
        {  // * itt a szoba lakoit ureslakokra csereljuk
            int temp = robbant_szoba->lakok[i].kartyaszam;
            robbant_szoba->lakok[i] = ureslako;
            robbant_szoba->lakok[i].kartyaszam = temp;
        }
    }
   
    if (esemeny == huto) {
         // ? Ez az eset pedig az, amikor egy huto repul ki az ablakon      
        int kirugott_kartyaszam;
        szoba_struct *kirugott_szobaja;
        int hanyadik;

        int nem_kirugott_e = ures; // * ugyanugy itt is olyat genrealunk aki nincs kirugva 
        
        while (nem_kirugott_e == ures) // * Generalunk egy megfelelo kartyaszamot
        {
            kirugott_kartyaszam = random_generator(utolso, elso);
            hol_lakik(szoba_parameter, kirugott_kartyaszam, &kirugott_szobaja, &hanyadik); 
            nem_kirugott_e = kirugott_szobaja->lakok[hanyadik].szak;               
        }


        printf("\e[0;33m A QPA het utolso napjan rettenetesen meleg volt a varosban.\e[0;37m\n");
        printf("\n\nA %d szobabol %s veletlenul kiejtette a hutot az ablakon, mikozben legkondinak akarta hasznalni.",kirugott_szobaja->szobaszam, kirugott_szobaja->lakok[hanyadik].nev);
        kirugas(szoba_parameter, kirugott_kartyaszam);
    
        if (rand() % 2 == 0) // * a vicc kedveert megvan az esely, hogy kivancsisagbol valaki mas is megnezi hogy kifer-e a huto az ablakon.
        {
            int nem_kirugott_e2 = ures; 
            while (nem_kirugott_e2 == ures) // * Megfelelo lako generalasa
            {
                kirugott_kartyaszam = random_generator(utolso, elso);
                hol_lakik(szoba_parameter, kirugott_kartyaszam, &kirugott_szobaja, &hanyadik); 
                nem_kirugott_e2 = kirugott_szobaja->lakok[hanyadik].szak;               
            }
            
            printf("\n\n A %d szobaban lako %s nem hitte el, hogy kifer a huto az ablakon a korabbi esemenyek alapjan, de tenyleg kifert, kiesett neki is.",
            kirugott_szobaja->szobaszam, kirugott_szobaja->lakok[hanyadik].nev);
            kirugas(szoba_parameter, kirugott_kartyaszam);
        }
    }
}