#ifndef ADATKEZELES_H
#define ADATKEZELES_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "debugmalloc.h"

// ! Adatstrukturak, define-ok

#define LIM "," // * szovegelvalasztas a txt file-ban
#define SORMAX 250 // * randomizalt nevek es adatok kiszamithatatlanok inkabb masfelszerese a kello meretnek
#define NEVMAX 30 // * Itt is inkabb a masfelszerese
#define LAKOSZAM 4 // * Ennyi lako van a szobaban, ha valakit kirugnak, az lenullazott lesz, a neve helyett '-' lesz, tehat a lakok szama konstans
#define VALASZHOSSZ 30 // * A felhasznalo altal beirt szoveg maximum ekkora lehet (Mivel adhat hosszu nevu lakot is.) 


typedef enum szak { // * Ez adja meg egy adott lako szakjat. 
    villany = 'v', // * Villamosmernok
    info = 'm', // * Mernokinfos
    uzinfo = 'u', // * Uzemmernok Informatikus
    ures = '-' // * Ures lakonak ez a "szakja" 
} szak; // * szamnak az enum megkapja a char-ok ASCII kodjat ami egyszerubbe teszi a beolvasast es a kiirast

typedef enum errorok { // * Lehetseges visszateresi ertekek
    sikeres, // * Ezt akkor adja avissza a fuggveny ha sikeresen le tudott futni 
    nem_letezo_file, // * Ha nem letezik az adott file, amit kerestunk 
    nem_letezo_szoba, // * Ha egy tevekenysegnek nem letezo szobaszamot adunk meg akkor ilyen error-t adhat (pl 2350 szamu szoba)
    nem_letezo_ember, // * Ha egy adatbazisban nem letezo kartyaszamu / nevu embert keresunk (pl. el akarunk koltoztetni egy nem letezo embert). 
    nincs_szabad_hely, // * Pl. Ha ugy akarunk felvenni egy lakot, hogy nincs szabad hely a kollegiumban.
    szoba_tele, // * Az adott szobaban minden hely foglalt.
    nem_egyezo_adat, // * Ha olyan adatot probalunk megadni ami vagy nem helyes tipusu, vagy nem oda illo.
    nem_sikeres_memoriaf, // * Nem sikeres a memoriafoglalas, nincs eleg elerheto memoria. (altalaban 78336 bajt korul foglal a program a debugmalloc.h szerint) 
    nem_letezo_szint // * A megadott szint nem letezik a kollegiumban.
} errorok;

typedef enum qpa_esemenyek { 
    robbanas, // * Ez az esemeny egy mikrorobbanassal jar
    huto, // * Ilyenkor egy huto repul ki az ablakon
    sormeres // * Ilyenkor egy sima someres tortenik
} qpa_esemenyek;

typedef struct lako_struct // * Egy-egy lako adatait tartalmazo adatstruktura
{
    char nev [NEVMAX]; // * Lako neve
    int kartyaszam; // *Schonherz kartyaszam, legfokepp azert, mert nagyon sok hasonlo nevu ember lehet. Az emberek (szoba szerint) sorban kapjak meg. (0-9999-ig mehet)
    szak szak; // * Enummal a szakja
    double meres; // * Sormerese
    double atlag; // * Tanulmanyi atlag
    int buntipont; // * A buntetopontjai az adott lakonak. 20 buntetopont folott automatikusan kirugja a program.
} lako_struct;

typedef struct szoba_struct  // * Egy adott szobanak a struct-ja 
{
    lako_struct lakok[LAKOSZAM]; // * Ez a lakok structjanak a tombje.
    int szobaszam; 
    bool golyaszoba; // * Ez megmutatja hogy golyaszinten van e szoba
    struct szoba_struct * kov; // * Novekvo sorban a kovetkezo szoba 
} szoba_struct;

// ! Globalis valtozo

extern const lako_struct ureslako;

int error_kezeles(int be_error);
int beolvas(char nev[], szoba_struct **szoba_parameter);
void kiiras(char nev[], szoba_struct *szoba_parameter);
void kuka(szoba_struct **szoba_parameter);


#endif // !ADATKEZELES_H