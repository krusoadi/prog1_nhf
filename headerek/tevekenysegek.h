#ifndef TEVEKENYSEGEK_H
#define TEVEKENYSEGEK_H

#include "adatkezeles.h"

char *szak_kereso(szak szak);
void lako_print(szoba_struct *szoba_parameter, int szobaszam, bool szinthez);
void szintprint(szoba_struct *szoba_parameter, int szint);
int osszes_szabad_hely(szoba_struct *szoba_parameter);
int szabad_hely(szoba_struct * szoba_parameter, int szoba, int* kartya);
void ujlako_listaba_felvetel(szoba_struct * szoba_parameter, int szoba, char nev[], double sormer, double atlag, szak szak);
void ujlako_adatfelvetel(szoba_struct *szoba_parameter);
int hol_lakik(szoba_struct *szoba_parameter, int kartya, szoba_struct **szoba, int* hanyadik);
void hol_lakik_kiiras(szoba_struct* szoba_parameter, int kartya);
void koltoztetes(szoba_struct *szoba_parameter, int kartya1, int kartya2);
void kirugas(szoba_struct *szoba_parameter, int kartya);
void enterleszedo(char* szoveg);
void buntetopont_ellenorzes(szoba_struct *szoba_pointer, int hanyadik);
void buntetopont_adas(szoba_struct *szoba_parameter, int kartya, int pont);
void legjobb(szoba_struct *szoba_parameter, bool sormer);
int random_generator(int felso, int also);
void qpa(szoba_struct *szoba_parameter);

#endif // !TEVEKENYSEGEK_H