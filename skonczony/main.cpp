

//Projekt gry Mastermind na zaliczenie - 1 semetr Informatyka WSTI

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <string.h>
//#include <dos.h>
#include <conio.h>
#include <windows.h>
#include <limits>
#include <stdexcept>
//#include <stdio.h>
#include <fstream>
#include <sstream>
#include <cstring>

using namespace std;

int ilosc_kolorow=0;
int ilosc_prob=0;
int dlugosc_planszy=20;
int szerokosc_planszy=9;
int plansza[9][16];
int punktacja [9][16];
char punktacja_zrandomizowana [9][16];
int rzad=0;
int kolumna=0;
int zakryte_liczby[9];
int zwyciestwo=0;
char tabela_ksywek[12][11];
char* wskaznik_na_ksywki[12][11];
int tabela_wynikow[4][11];
int trudnosc=0;
string do_zapisu;
string finalny_zapis;
int wybor=0;

//----------------------------------------------------------------------------
struct console
  {
  console( unsigned width, unsigned height )
    {
    SMALL_RECT r;
    COORD      c;
     hConOut = GetStdHandle( STD_OUTPUT_HANDLE );
    if (!GetConsoleScreenBufferInfo( hConOut, &csbi ))
      throw runtime_error( "You must be attached to a human." );

    r.Left   =
    r.Top    = 0;
    r.Right  = width -1;
    r.Bottom = height -1;
    SetConsoleWindowInfo( hConOut, TRUE, &r );

    c.X = width;
    c.Y = height;
    SetConsoleScreenBufferSize( hConOut, c );
    }

  ~console()
    {
    SetConsoleTextAttribute(    hConOut,        csbi.wAttributes );
    SetConsoleScreenBufferSize( hConOut,        csbi.dwSize      );
    SetConsoleWindowInfo(       hConOut, TRUE, &csbi.srWindow    );
    }

  void color( WORD color = 0x07 )
    {
    SetConsoleTextAttribute( hConOut, color );
    }

  HANDLE                     hConOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  };

//----------------------------------------------------------------------------
console con( 62, 21 );

//**************************************************************
void zerowanie_zmiennych()
{
    int zerowanie1=0;
    int zerowanie2=0;
    for (zerowanie1=0;zerowanie1<=16;zerowanie1++)
    {
        for (zerowanie2=0;zerowanie2<=9;zerowanie2++)
        {
            plansza[zerowanie2][zerowanie1]=0;
            punktacja[zerowanie2][zerowanie1]=0;
            punktacja_zrandomizowana[zerowanie2][zerowanie1]=0;
        }
    }

    zwyciestwo=0;
}


//**************************************************************
void zerowanie_tabeli_wynikow_i_ksywek()
{
    int zerowanie1=0;
    int zerowanie2=0;
    for (zerowanie2=0;zerowanie2<11;zerowanie2++)
    {
        for (zerowanie1=0;zerowanie1<=11;zerowanie1++)
        {
            tabela_ksywek[zerowanie1][zerowanie2]=' ';
            if (zerowanie1<=3)
            tabela_wynikow[zerowanie1][zerowanie2]=0;
        }
    }
}


//**************************************************************
void brzeg_ramki ()
{
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
}


//**************************************************************
void wyswietlanie_planszy ()
{
int dopelnienie_ramki=((16-dlugosc_planszy)/2);
    system("CLS");
    brzeg_ramki();
    for (dopelnienie_ramki;dopelnienie_ramki>0;dopelnienie_ramki--)
        cout<<"::                                                         ::"<<endl;
    for (int wys=0;wys<=dlugosc_planszy;wys++)
        {
            cout<<"::  "; //ozdobnik tabelki
            for(int spacje=0;spacje<20-(szerokosc_planszy*2);spacje++)
            {
                cout<<" ";
            }

            cout<<wys+1<<"  ";
            if (wys+1<=9)
                cout<<" ";

            for (int szer=0;szer<=szerokosc_planszy;szer++)
            {
                cout << plansza[szer][wys] << " ";
                if (szer==szerokosc_planszy)
                    cout <<"   ";

            }
            for (int szer=0;szer<=szerokosc_planszy;szer++)
            {

                cout << punktacja_zrandomizowana[szer][wys] << " ";
                if (szer==szerokosc_planszy)
                {
                    for(int spacje=0;spacje<=20-(szerokosc_planszy*2);spacje++)
                    {
                        cout<<" ";
                    }
                    if ((szerokosc_planszy % 2)==1)
                        cout<<" ";
                    cout <<"  ::"<<endl; //dolozony ozdobnik tabelki
                }

            }
        }
dopelnienie_ramki=((16-dlugosc_planszy)/2);
    for (dopelnienie_ramki;dopelnienie_ramki>0;dopelnienie_ramki--)
        cout<<"::                                                         ::"<<endl;
    brzeg_ramki();
}


//**************************************************************
void wprowadzanie_strzalu ()
{
    for(int kolumna=0;kolumna<=szerokosc_planszy;kolumna++)
    {
        cout<<"wprowadz cyfre z zakresu 1-"<<ilosc_kolorow<<endl;
        plansza[kolumna][rzad]=(getch()-48);

        for (int ogranicznik=1;ogranicznik<=ilosc_kolorow;ogranicznik++)
        {
            if (plansza[kolumna][rzad] == ogranicznik)
                break;

            if (ilosc_kolorow==ogranicznik)
            {
                plansza[kolumna][rzad]=0;
                kolumna--;
                cout<<"Nieprawidlowe wprowadzenie! mialo byc z zakresu 1-"<<ilosc_kolorow<<"!"<<endl;
                _sleep(400);
            }

        }
    if (kolumna<szerokosc_planszy)
    wyswietlanie_planszy();
    }
}


//**************************************************************
//funkcja ktora liczy czesc odpowiedzialna za punktacje i warunki zakonczenia gry
void obliczanie_punktacji ()
{
int suma_punktow=0;
int zakryte_liczby_tymczasowe[9]; //wpisywana jest tutaj wartosc zmiennej liczby_zakryte, bo w petli sa w niej zerowane te liczby, a dzieki temu nie tracimy wygenerowanej na poczatku zmeinnej juz po pierwszym przelocie. Zeminna ma szanse sie regenerowac dzieki temu.

    //petla do przyporzadkowania liczb zakrytych do tymczasowej tabeli
    for(kolumna=0;kolumna<=szerokosc_planszy;kolumna++)
    {
        zakryte_liczby_tymczasowe[kolumna]=zakryte_liczby[kolumna];
    }

//petla na sprawdzenie trafien dokladnych sposrod liczb ukrytych (wynik 2)
    for(kolumna=0;kolumna<=szerokosc_planszy;kolumna++)
    {
        if ((plansza[kolumna][rzad]) == (zakryte_liczby_tymczasowe[kolumna]))
        {
            punktacja[kolumna][rzad]=2;
            zakryte_liczby_tymczasowe[kolumna]=0;

//fragment do liczenia czy aby nie skonczylismy gry z sukcesem
    suma_punktow++;
        if (suma_punktow==szerokosc_planszy+1)
            zwyciestwo=1;
//jezeli koniec nie zostal osiagniety, to wracamy do wprowadzania:
        }
    }

//petla na sprawdzenie i odfiltrowanie wynikow niedokladnych trafien sposrod ukrytych (wynik 1)
    for(kolumna=0;kolumna<=szerokosc_planszy;kolumna++)
    {
        for (int sprawdzacz=0;sprawdzacz<=szerokosc_planszy;sprawdzacz++)
        {
            if ((plansza[kolumna][rzad]) == (zakryte_liczby_tymczasowe[sprawdzacz]))
            {
                if (punktacja[kolumna][rzad]!= 2)
                {
                    punktacja[kolumna][rzad]=1;
                    zakryte_liczby_tymczasowe[sprawdzacz]=0;
                    break;
                }
            }
        }
    }
//czesc ktora konwertuje tabele wynikow wskazujaca pola oceny w kolejnosci pol planszy gracza
//na dane uszeregowane od X jako trafienie, przez / jako zle pole po 0 jako nietrafienie.
        int znacznik = 0;
            for (int randomizer=0;randomizer<=szerokosc_planszy;randomizer++)
            {
                    if (punktacja[randomizer][rzad] == 2)
                    {
                        punktacja_zrandomizowana[znacznik][rzad] = 'X';
                        znacznik++;
                    }

            }
            for (int randomizer=0;randomizer<=szerokosc_planszy;randomizer++)
            {
                    if (punktacja[randomizer][rzad] == 1)
                    {
                        punktacja_zrandomizowana[znacznik][rzad] = '/';
                        znacznik++;
                    }

            }
            for (int randomizer=0;randomizer<=szerokosc_planszy;randomizer++)
            {
                    if (punktacja[randomizer][rzad] == 0)
                    {
                        punktacja_zrandomizowana[znacznik][rzad] = '0';
                        znacznik++;
                    }
            }

        wyswietlanie_planszy();
//fragment do liczenia, czy aby nie ponieslismy porazki - skonczyly nam sie pola do strzelania
        if ((rzad==dlugosc_planszy) && (zwyciestwo != 1))
        zwyciestwo=2;

        rzad++;
}


//**************************************************************
void generowanie_liczb_zakrytych ()
{
    for (int i=0;i<=szerokosc_planszy;i++)
    {
        zakryte_liczby[i] = 1+(rand()%ilosc_kolorow);

    }
}


//**************************************************************
void przerwa_w_fajerwerkach ()
{
_sleep(200);
system("CLS");
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
cout<<"::                                                         ::"<<endl;
cout<<"::                                                         ::"<<endl;
cout<<"::             ~~~ GRATULACJE! ZWYCIESTWO! ~~~             ::"<<endl;
cout<<"::                                                         ::"<<endl;
}


//**************************************************************
//ekran ktory bedzie sie wyswietlal po zakonczeniu gry:
void ekran_zwyciestwa ()
{
_sleep(1800);
przerwa_w_fajerwerkach();
cout<<"::                                                         ::"<<endl;
cout<<"::                                                         ::"<<endl;
cout<<"::                                                         ::"<<endl;
cout<<"::                       .|                                ::"<<endl;
cout<<"::                       | |                               ::"<<endl;
cout<<"::                       |'|            ._____             ::"<<endl;
cout<<"::               ___    |  |            |.   |' .---\'|     ::"<<endl;
cout<<"::       _    .-'   '-. |  |     .--'|  ||   | _|    |     ::"<<endl;
cout<<"::    .-'|  _.|  |    ||   '-__  |   |  |    ||      |     ::"<<endl;
cout<<"::    |' | |.    |    ||       | |   |  |    ||      |     ::"<<endl;
cout<<":: ___|  '-'     '    \'\'       '-'   '-.'    '`      |____ ::"<<endl;
cout<<"::~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~::"<<endl;
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
przerwa_w_fajerwerkach ();
cout<<"::                                                         ::"<<endl;
cout<<"::                                                         ::"<<endl;
cout<<"::                                                         ::"<<endl;
cout<<"::                       .|                                ::"<<endl;
cout<<"::                       | |                               ::"<<endl;
cout<<"::                       |'|  '         ._____             ::"<<endl;
cout<<"::               ___    |  | .          |.   |' .---\'|     ::"<<endl;
cout<<"::       _    .-'   '-. |  | .   .--'|  ||   | _|    |     ::"<<endl;
cout<<"::    .-'|  _.|  |    ||   '-__  |   |  |    ||      |     ::"<<endl;
cout<<"::    |' | |.    |    ||       | |   |  |    ||      |     ::"<<endl;
cout<<":: ___|  '-'     '    \'\'       '-'   '-.'    '`      |____ ::"<<endl;
cout<<"::~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~::"<<endl;
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
przerwa_w_fajerwerkach ();
cout<<"::                                                         ::"<<endl;
cout<<"::                                                         ::"<<endl;
cout<<"::                                                         ::"<<endl;
cout<<"::                       .|   _\\/_                         ::"<<endl;
cout<<"::                       | |   /\\                          ::"<<endl;
cout<<"::                       |'|  '         ._____             ::"<<endl;
cout<<"::               ___    |  | .          |.   |' .---\'|     ::"<<endl;
cout<<"::       _    .-'   '-. |  | .   .--'|  ||   | _|    |     ::"<<endl;
cout<<"::    .-'|  _.|  |    ||   '-__  |   |  |    ||      |     ::"<<endl;
cout<<"::    |' | |.    |    ||       | |   |  |    ||      |     ::"<<endl;
cout<<":: ___|  '-'     '    \'\'       '-'   '-.'    '`      |____ ::"<<endl;
cout<<"::~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~::"<<endl;
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
przerwa_w_fajerwerkach ();
cout<<"::                                                         ::"<<endl;
cout<<"::                                                         ::"<<endl;
cout<<"::                            *  *                         ::"<<endl;
cout<<"::                       .|  *_\\/_*                        ::"<<endl;
cout<<"::                       | | * /\\ *                        ::"<<endl;
cout<<"::                       |'|  *  *      ._____             ::"<<endl;
cout<<"::               ___    |  |            |.   |' .---\'|     ::"<<endl;
cout<<"::       _    .-'   '-. |  |     .--'|  ||   | _|    |     ::"<<endl;
cout<<"::    .-'|  _.|  |    ||   '-__  |   |  |    ||      |     ::"<<endl;
cout<<"::    |' | |.    |    ||       | |   |  |    ||      |     ::"<<endl;
cout<<":: ___|  '-'     '    \'\'       '-'   '-.'    '`      |____ ::"<<endl;
cout<<"::~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~::"<<endl;
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
przerwa_w_fajerwerkach ();
cout<<"::                                                         ::"<<endl;
cout<<"::                                                         ::"<<endl;
cout<<"::                            *  *                         ::"<<endl;
cout<<"::                       .|  *    *                        ::"<<endl;
cout<<"::                       | | *    * _\\/_                   ::"<<endl;
cout<<"::          _\\/_         |'|  *  *   /\\ ._____             ::"<<endl;
cout<<"::           /\\  ___    |  |            |.   |' .---\'|     ::"<<endl;
cout<<"::       _    .-'   '-. |  |     .--'|  ||   | _|    |     ::"<<endl;
cout<<"::    .-'|  _.|  |    ||   '-__  |   |  |    ||      |     ::"<<endl;
cout<<"::    |' | |.    |    ||       | |   |  |    ||      |     ::"<<endl;
cout<<":: ___|  '-'     '    \'\'       '-'   '-.'    '`      |____ ::"<<endl;
cout<<"::~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~::"<<endl;
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
przerwa_w_fajerwerkach ();
cout<<"::                                                         ::"<<endl;
cout<<"::                                                         ::"<<endl;
cout<<"::                            *  *                         ::"<<endl;
cout<<"::               _\\/_    .|  *    * .::.                   ::"<<endl;
cout<<"::          .''.  /\\     | | *     :_\\/_:                  ::"<<endl;
cout<<"::         :_\\/_:        |'|  *  * : /\\ :_____             ::"<<endl;
cout<<"::         : /\\ :___    |  |   o    '::'|.   |' .---\'|     ::"<<endl;
cout<<"::       _  '..-'   '-. |  |     .--'|  ||   | _|    |     ::"<<endl;
cout<<"::    .-'|  _.|  |    ||   '-__  |   |  |    ||      |     ::"<<endl;
cout<<"::    |' | |.    |    ||       | |   |  |    ||      |     ::"<<endl;
cout<<":: ___|  '-'     '    \'\'       '-'   '-.'    '`      |____ ::"<<endl;
cout<<"::~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~::"<<endl;
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
przerwa_w_fajerwerkach ();
cout<<"::                                                         ::"<<endl;
cout<<"::                                                         ::"<<endl;
cout<<"::               .''.          *                           ::"<<endl;
cout<<"::              :_\\/_:   .|         .::.                   ::"<<endl;
cout<<"::          .''.: /\\ :   | |       :    :                  ::"<<endl;
cout<<"::         :    :'..'    |'|  \\'/  :    :_____             ::"<<endl;
cout<<"::         :    :___    |  | = o =  '::'|.   |' .---\'|     ::"<<endl;
cout<<"::       _  '..-'   '-. |  |  /.\\.--'|  ||   | _|    |     ::"<<endl;
cout<<"::    .-'|  _.|  |    ||   '-__  |   |  |    ||      |     ::"<<endl;
cout<<"::    |' | |.    |    ||       | |   |  |    ||      |     ::"<<endl;
cout<<":: ___|  '-'     '    \'\'       '-'   '-.'    '`      |____ ::"<<endl;
cout<<"::~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~::"<<endl;
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
przerwa_w_fajerwerkach ();
cout<<"::                                                         ::"<<endl;
cout<<"::                           _\\)/_                         ::"<<endl;
cout<<"::               .''.         /(\\                          ::"<<endl;
cout<<"::              :    :   .|         _\\/_                   ::"<<endl;
cout<<"::          .''.:    :   | |   :     /\\                    ::"<<endl;
cout<<"::         :    :'..'    |'|'.\\'/.'     ._____             ::"<<endl;
cout<<"::         :    :___    |  |-= o =-     |.   |' .---\'|     ::"<<endl;
cout<<"::       _  '..-'   '-. |  |.'/.\\:--'|  ||   | _|    |     ::"<<endl;
cout<<"::    .-'|  _.|  |    ||   '-__: |   |  |    ||      |     ::"<<endl;
cout<<"::    |' | |.    |    ||       | |   |  |    ||      |     ::"<<endl;
cout<<":: ___|  '-'     '    \'\'       '-'   '-.'    '`      |____ ::"<<endl;
cout<<"::~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~::"<<endl;
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
przerwa_w_fajerwerkach ();
cout<<"::                             .                           ::"<<endl;
cout<<"::                           _\\)/_                         ::"<<endl;
cout<<"::               .''.         /(\\   .''.                   ::"<<endl;
cout<<"::              :    :   .|    '   :_\\/_:                  ::"<<endl;
cout<<"::              :    :   | |   :   : /\\ :                  ::"<<endl;
cout<<"::               '..'    |'|'. ' .' '..'._____             ::"<<endl;
cout<<"::               ___    |  |-=   =-     |.   |' .---\'|     ::"<<endl;
cout<<"::       _    .-'   '-. |  |.' . :--'|  ||   | _|    |     ::"<<endl;
cout<<"::    .-'|  _.|  |    ||   '-__: |   |  |    ||      |     ::"<<endl;
cout<<"::    |' | |.    |    ||       | |   |  |    ||      |     ::"<<endl;
cout<<":: ___|  '-'     '    \'\'       '-'   '-.'    '`      |____ ::"<<endl;
cout<<"::~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~::"<<endl;
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
przerwa_w_fajerwerkach ();
cout<<"::                                                         ::"<<endl;
cout<<"::                                                         ::"<<endl;
cout<<"::                _\\/_              .''.                   ::"<<endl;
cout<<"::                 /\\    .|        :    :                  ::"<<endl;
cout<<"::                       | |       :    :                  ::"<<endl;
cout<<"::                       |'|        '..'._____             ::"<<endl;
cout<<"::               ___    |  |            |.   |' .---\'|     ::"<<endl;
cout<<"::       _    .-'   '-. |  |     :--'|  ||   | _|    |     ::"<<endl;
cout<<"::    .-'|  _.|  |    ||   '-__: |   |  |    ||      |     ::"<<endl;
cout<<"::    |' | |.    |    ||       | |   |  |    ||      |     ::"<<endl;
cout<<":: ___|  '-'     '    \'\'       '-'   '-.'    '`      |____ ::"<<endl;
cout<<"::~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~::"<<endl;
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
przerwa_w_fajerwerkach ();
cout<<"::                              \\'/                        ::"<<endl;
cout<<"::                *  *         = o =                       ::"<<endl;
cout<<"::               *_\\/_*         /.\\ .''.                   ::"<<endl;
cout<<"::               * /\\ *  .|        :    :                  ::"<<endl;
cout<<"::                *  *   | |       :    :                  ::"<<endl;
cout<<"::                       |'|        '..'._____             ::"<<endl;
cout<<"::               ___    |  |            |.   |' .---\'|     ::"<<endl;
cout<<"::       _    .-'   '-. |  |     :--'|  ||   | _|    |     ::"<<endl;
cout<<"::    .-'|  _.|  |    ||   '-__: |   |  |    ||      |     ::"<<endl;
cout<<"::    |' | |.    |    ||       | |   |  |    ||      |     ::"<<endl;
cout<<":: ___|  '-'     '    \'\'       '-'   '-.'    '`      |____ ::"<<endl;
cout<<"::~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~::"<<endl;
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
przerwa_w_fajerwerkach ();
cout<<"::                            '.\\'/.'                      ::"<<endl;
cout<<"::                *  *        -= o =-                      ::"<<endl;
cout<<"::               *    *       .'/.\\'.                      ::"<<endl;
cout<<"::               *    *  .|      :                         ::"<<endl;
cout<<"::                *  *   | |                               ::"<<endl;
cout<<"::                       |'|            ._____             ::"<<endl;
cout<<"::               ___    |  |            |.   |' .---\'|     ::"<<endl;
cout<<"::       _    .-'   '-. |  |     :--'|  ||   | _|    |     ::"<<endl;
cout<<"::    .-'|  _.|  |    ||   '-__: |   |  |    ||      |     ::"<<endl;
cout<<"::    |' | |.    |    ||       | |   |  |    ||      |     ::"<<endl;
cout<<":: ___|  '-'     '    \'\'       '-'   '-.'    '`      |____ ::"<<endl;
cout<<"::~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~::"<<endl;
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
przerwa_w_fajerwerkach ();
cout<<"::                            '.\\'/.'                      ::"<<endl;
cout<<"::                            -=   =-                      ::"<<endl;
cout<<"::                   o        .'/.\\'.                      ::"<<endl;
cout<<"::            o          .|      :                         ::"<<endl;
cout<<"::                       | |        .:.                    ::"<<endl;
cout<<"::                       |'|        ':' ._____             ::"<<endl;
cout<<"::               ___    |  |            |.   |' .---\'|     ::"<<endl;
cout<<"::       _    .-'   '-. |  |     :--'|  ||   | _|    |     ::"<<endl;
cout<<"::    .-'|  _.|  |    ||   '-__: |   |  |    ||      |     ::"<<endl;
cout<<"::    |' | |.    |    ||       | |   |  |    ||      |     ::"<<endl;
cout<<":: ___|  '-'     '    \'\'       '-'   '-.'    '`      |____ ::"<<endl;
cout<<"::~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~::"<<endl;
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
przerwa_w_fajerwerkach ();
cout<<"::                            '. ' .'                      ::"<<endl;
cout<<"::                  \\'/       -     -                      ::"<<endl;
cout<<"::           \\'/   = o =      .' . '.                      ::"<<endl;
cout<<"::          = o =   /.\\  .|      :  .:::.                  ::"<<endl;
cout<<"::           /.\\         | |       :::::::                 ::"<<endl;
cout<<"::                       |'|        ':::'_____             ::"<<endl;
cout<<"::               ___    |  |            |.   |' .---\'|     ::"<<endl;
cout<<"::       _    .-'   '-. |  |     :--'|  ||   | _|    |     ::"<<endl;
cout<<"::    .-'|  _.|  |    ||   '-__: |   |  |    ||      |     ::"<<endl;
cout<<"::    |' | |.    |    ||       | |   |  |    ||      |     ::"<<endl;
cout<<":: ___|  '-'     '    \'\'       '-'   '-.'    '`      |____ ::"<<endl;
cout<<"::~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~::"<<endl;
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
przerwa_w_fajerwerkach ();
cout<<"::                   :                                     ::"<<endl;
cout<<"::            :   '.\\'/.'                                  ::"<<endl;
cout<<"::         '.\\'/.'-= o =-           .:::.                  ::"<<endl;
cout<<"::         -= o =-.'/.\\'..|        :::::::                 ::"<<endl;
cout<<"::         .'/.\\'.   :   | |       :::::::                 ::"<<endl;
cout<<"::            :          |'|        ':::'_____             ::"<<endl;
cout<<"::               ___    |  |            |.   |' .---\'|     ::"<<endl;
cout<<"::       _    .-'   '-. |  |     :--'|  ||   | _|    |     ::"<<endl;
cout<<"::    .-'|  _.|  |    ||   '-__: |   |  |    ||      |     ::"<<endl;
cout<<"::    |' | |.    |    ||       | |   |  |    ||      |     ::"<<endl;
cout<<":: ___|  '-'     '    \'\'       '-'   '-.'    '`      |____ ::"<<endl;
cout<<"::~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~::"<<endl;
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
przerwa_w_fajerwerkach ();
cout<<"::                   :                                     ::"<<endl;
cout<<"::            :   '.\\'/.'                                  ::"<<endl;
cout<<"::         '.\\'/.'-=   =-       *   .:::.                  ::"<<endl;
cout<<"::         -=   =-.'/.\\'..|        ::' '::                 ::"<<endl;
cout<<"::         .'/.\\'.   :   | |       ::. .::                 ::"<<endl;
cout<<"::            :          |'|        ':::'_____             ::"<<endl;
cout<<"::               ___    |  |            |.   |' .---\'|     ::"<<endl;
cout<<"::       _    .-'   '-. |  |     :--'|  ||   | _|    |     ::"<<endl;
cout<<"::    .-'|  _.|  |    ||   '-__: |   |  |    ||      |     ::"<<endl;
cout<<"::    |' | |.    |    ||       | |   |  |    ||      |     ::"<<endl;
cout<<":: ___|  '-'     '    \'\'       '-'   '-.'    '`      |____ ::"<<endl;
cout<<"::~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~::"<<endl;
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
przerwa_w_fajerwerkach ();
cout<<"::                   :          .                          ::"<<endl;
cout<<"::            :   '. ' .'     _\\)/_                        ::"<<endl;
cout<<"::         '. ' .'-     -      /(\\  .'''.                  ::"<<endl;
cout<<"::         -     -.' . '..|     '  :     :                 ::"<<endl;
cout<<"::         .' . '.   :   | |       :     :                 ::"<<endl;
cout<<"::            :          |'|        '...'_____             ::"<<endl;
cout<<"::               ___    |  |            |.   |' .---\'|     ::"<<endl;
cout<<"::       _    .-'   '-. |  |     :--'|  ||   | _|    |     ::"<<endl;
cout<<"::    .-'|  _.|  |    ||   '-__: |   |  |    ||      |     ::"<<endl;
cout<<"::    |' | |.    |    ||       | |   |  |    ||      |     ::"<<endl;
cout<<":: ___|  '-'     '    \'\'       '-'   '-.'    '`      |____ ::"<<endl;
cout<<"::~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~::"<<endl;
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
przerwa_w_fajerwerkach ();
cout<<"::                              .                          ::"<<endl;
cout<<"::                            _\\)/_        _\\/_            ::"<<endl;
cout<<"::                             /(\\   _\\/_   /\\             ::"<<endl;
cout<<"::                       .|     '     /\\                   ::"<<endl;
cout<<"::                       | |                               ::"<<endl;
cout<<"::                       |'|            ._____             ::"<<endl;
cout<<"::               ___    |  |            |.   |' .---\'|     ::"<<endl;
cout<<"::       _    .-'   '-. |  |     :--'|  ||   | _|    |     ::"<<endl;
cout<<"::    .-'|  _.|  |    ||   '-__: |   |  |    ||      |     ::"<<endl;
cout<<"::    |' | |.    |    ||       | |   |  |    ||      |     ::"<<endl;
cout<<":: ___|  '-'     '    \'\'       '-'   '-.'    '`      |____ ::"<<endl;
cout<<"::~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~::"<<endl;
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
przerwa_w_fajerwerkach ();
cout<<"::                              .          .''.            ::"<<endl;
cout<<"::                            _\\)/_  .''. :_\\/_:           ::"<<endl;
cout<<"::                             /(\\  :_\\/_:: /\\ :           ::"<<endl;
cout<<"::                       .|     '   : /\\ : '..'            ::"<<endl;
cout<<"::               o       | |         '..'                  ::"<<endl;
cout<<"::                       |'|            ._____             ::"<<endl;
cout<<"::               ___    |  |            |.   |' .---\'|     ::"<<endl;
cout<<"::       _    .-'   '-. |  |     :--'|  ||   | _|    |     ::"<<endl;
cout<<"::    .-'|  _.|  |    ||   '-__: |   |  |    ||      |     ::"<<endl;
cout<<"::    |' | |.    |    ||       | |   |  |    ||      |     ::"<<endl;
cout<<":: ___|  '-'     '    \'\'       '-'   '-.'    '`      |____ ::"<<endl;
cout<<"::~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~::"<<endl;
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
przerwa_w_fajerwerkach ();
cout<<"::                                         .''.            ::"<<endl;
cout<<"::                                   .''. :    :           ::"<<endl;
cout<<"::                           _\\/_   :    ::    :           ::"<<endl;
cout<<"::              \\'/      .|   /\\    :    : '..'            ::"<<endl;
cout<<"::             = o =     | |    _\\/_ '..'                  ::"<<endl;
cout<<"::              /.\\      |'|     /\\     ._____             ::"<<endl;
cout<<"::               ___    |  |            |.   |' .---\'|     ::"<<endl;
cout<<"::       _    .-'   '-. |  |     :--'|  ||   | _|    |     ::"<<endl;
cout<<"::    .-'|  _.|  |    ||   '-__: |   |  |    ||      |     ::"<<endl;
cout<<"::    |' | |.    |    ||       | |   |  |    ||      |     ::"<<endl;
cout<<":: ___|  '-'     '    \'\'       '-'   '-.'    '`      |____ ::"<<endl;
cout<<"::~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~::"<<endl;
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
przerwa_w_fajerwerkach ();
cout<<"::                                                         ::"<<endl;
cout<<"::                           .''.                          ::"<<endl;
cout<<"::               :          :_\\/_:                         ::"<<endl;
cout<<"::            '.\\'/.'    .| : /\\.:'.                       ::"<<endl;
cout<<"::            -= o =-    | | '.:_\\/_:                      ::"<<endl;
cout<<"::            .'/.\\'.    |'|   : /\\ :   ._____             ::"<<endl;
cout<<"::               :__    |  |    '..'    |.   |' .---\'|     ::"<<endl;
cout<<"::       _    .-'   '-. |  |     :--'|  ||   | _|    |     ::"<<endl;
cout<<"::    .-'|  _.|  |    ||   '-__: |   |  |    ||      |     ::"<<endl;
cout<<"::    |' | |.    |    ||       | |   |  |    ||      |     ::"<<endl;
cout<<":: ___|  '-'     '    \'\'       '-'   '-.'    '`      |____ ::"<<endl;
cout<<"::~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~::"<<endl;
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
przerwa_w_fajerwerkach ();
cout<<"::                                                         ::"<<endl;
cout<<"::                           .''.                          ::"<<endl;
cout<<"::               :          :    :                         ::"<<endl;
cout<<"::            '.\\'/.'    .| :   .:'.                       ::"<<endl;
cout<<"::            -=   =-    | | '.:    :                      ::"<<endl;
cout<<"::            .'/.\\'.    |'|   :    :   ._____             ::"<<endl;
cout<<"::               :__    |  |    '..'    |.   |' .---\'|     ::"<<endl;
cout<<"::       _    .-'   '-. |  |     :--'|  ||   | _|    |     ::"<<endl;
cout<<"::    .-'|  _.|  |    ||   '-__: |   |  |    ||      |     ::"<<endl;
cout<<"::    |' | |.    |    ||       | |   |  |    ||      |     ::"<<endl;
cout<<":: ___|  '-'     '    \'\'       '-'   '-.'    '`      |____ ::"<<endl;
cout<<"::~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~::"<<endl;
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
przerwa_w_fajerwerkach ();
cout<<"::                                                         ::"<<endl;
cout<<"::                           .''.                          ::"<<endl;
cout<<"::               :          :    :                         ::"<<endl;
cout<<"::            '. ' .'    .| :   .:'.                       ::"<<endl;
cout<<"::            -     -    | | '.:    :                      ::"<<endl;
cout<<"::            .' . '.    |'|   :    :   ._____             ::"<<endl;
cout<<"::               :__    |  |    '..'    |.   |' .---\'|     ::"<<endl;
cout<<"::       _    .-'   '-. |  |     :--'|  ||   | _|    |     ::"<<endl;
cout<<"::    .-'|  _.|  |    ||   '-__: |   |  |    ||      |     ::"<<endl;
cout<<"::    |' | |.    |    ||       | |   |  |    ||      |     ::"<<endl;
cout<<":: ___|  '-'     '    \'\'       '-'   '-.'    '`      |____ ::"<<endl;
cout<<"::~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~::"<<endl;
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
_sleep(200);
system("CLS");
}


//**************************************************************
//ekran ktory bedzie sie wyswietlal, kiedy nie uda nam sie zgadnac w przewidzianych powtorzeniach
void ekran_porazki ()
{
    system("CLS");
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
cout<<"::                                                         ::"<<endl;
cout<<"::                                                         ::"<<endl;
cout<<"::                   NIESTETY! PORAZKA!                    ::"<<endl;
cout<<"::                                                         ::"<<endl;
cout<<"::                          .-~~-.                         ::"<<endl;
cout<<"::                         (_~..~_)                        ::"<<endl;
cout<<"::                           ||||                          ::"<<endl;
cout<<"::                  __________________                     ::"<<endl;
cout<<"::                 (____    _||||_    )                    ::"<<endl;
cout<<"::                   ___)  ( _''_ )  (___                  ::"<<endl;
cout<<"::                  (_      ~-..-~      _)                 ::"<<endl;
cout<<"::                 ___)                (___                ::"<<endl;
cout<<"::                (______          ________)               ::"<<endl;
cout<<"::                    ___)        (                        ::"<<endl;
cout<<"::                   (_____________)                       ::"<<endl;
cout<<"::                                                         ::"<<endl;
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;

_sleep(2000);
}


//**************************************************************
void ekran_startowy ()
{
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
cout<<"::                                                         ::"<<endl;
cout<<"::                                                         ::"<<endl;
cout<<"::               ~~~ M A S T E R M I N D ~~~               ::"<<endl;
cout<<"::                                                         ::"<<endl;
cout<<"::                                                         ::"<<endl;
cout<<"::                       _---~~(~~-_.                      ::"<<endl;
cout<<"::                     _{        )   )                     ::"<<endl;
cout<<"::                   ,   ) -~~- ( ,-' )_                   ::"<<endl;
cout<<"::                  (  `-,_..`., )-- '_,)                  ::"<<endl;
cout<<"::                 ( ` _)  (  -~( -_ `,  }                 ::"<<endl;
cout<<"::                 (_-  _  ~_-~~~~`,  ,' )                 ::"<<endl;
cout<<"::                   `~ -^(    __;-,((()))                 ::"<<endl;
cout<<"::                         ~~~~ {_ -_(())                  ::"<<endl;
cout<<"::                                `  }                     ::"<<endl;
cout<<"::                                  { }                    ::"<<endl;
cout<<"::                                                         ::"<<endl;
cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
_sleep(2000);
}


//**************************************************************
void wstep_i_menu_1()
{
    //wstep i menu
    /*cout<<"prosze okreslic szerokosc planszy (4-9) (ilosc wybieranych za kazdym razem pionkow)"<<endl;
    cin>>szerokosc_planszy;
    //poniewaz zmienna szerokosc_planszy bedzie sie odnosic do tabel, to konwertuje ja od razu na format "tabelowy"
    szerokosc_planszy--;
    cout<<"prosze okreslic dlugosc planszy (1-20) (ilosc dopuszczalnych prob zgadywania)"<<endl;
    //poniewaz zmienna dlugosc_planszy bedzie sie odnosic do tabel, to konwertuje ja od razu na format "tabelowy"
    cin>>dlugosc_planszy;
    dlugosc_planszy--;
    cout<<"prosze okreslic ilosc kolorow, ktore zostana uzyte w grze (ze wzgledu na brak kolorow, pionki zastepowane sa przez cyfry od jeden do ilosci wybranej - maksymalnie 9 \"kolorow\")"<<endl;
    cin>>ilosc_kolorow;*/
    system("CLS");
    brzeg_ramki();
    cout<<"::                                                         ::"<<endl;
    cout<<"::                  Witaj w grze Mastermind!               ::"<<endl;
    cout<<"::                                                         ::"<<endl;
    cout<<"::                                                         ::"<<endl;
    cout<<"::                                                         ::"<<endl;
    cout<<":: Celem gry jest odgadniecie kilku cyfr ukrytych          ::"<<endl;
    cout<<":: przed graczem na dole planszy mozesz wpisywac           ::"<<endl;
    cout<<":: liczby, ktore zgadujesz, ze sa tymi ukrytymi.           ::"<<endl;
    cout<<":: Kazda cyfre ktora chcesz obstawic wpisz                 ::"<<endl;
    cout<<":: na klawiaturze - automatycznie zostanie                 ::"<<endl;
    cout<<":: ona wpisana do najwyzszego niewypelnionego rzedu.       ::"<<endl;
    cout<<":: Po wypelnieniu danego rzedu pojawi sie obok niego       ::"<<endl;
    cout<<":: podpowiedz, jak duza skutecznosc masz w zgadywaniu.     ::"<<endl;
    cout<<"::                                                         ::"<<endl;
    cout<<"::                                                         ::"<<endl;
    cout<<"::                                                         ::"<<endl;
    brzeg_ramki();
system("PAUSE");
system("CLS");
}


//**************************************************************
void wstep_i_menu_2()
{
    brzeg_ramki();
    cout<<"::                                                         ::"<<endl;
    cout<<"::                                                         ::"<<endl;
    cout<<"::                                                         ::"<<endl;
    cout<<":: Kazdy X oznacza, ze ktoras z cyfr ktore obstawiasz      ::"<<endl;
    cout<<":: znajduje sie na tym samym miejscu co ukryta i ma        ::"<<endl;
    cout<<":: ta sama co ona wartosc.                                 ::"<<endl;
    cout<<":: Kazdy / oznacza, ze ktoras z cyfr owszem,               ::"<<endl;
    cout<<":: znajduje sie wsrod ukrytych, ale na innym               ::"<<endl;
    cout<<":: miejscu niz obstawiasz.                                 ::"<<endl;
    cout<<":: Z kolei kazde 0 oznacza, ze ktorejs z obstawianych      ::"<<endl;
    cout<<":: cyfr w ogole nie ma wsrod ukrytych...                   ::"<<endl;
    cout<<"::                                                         ::"<<endl;
    cout<<"::                                                         ::"<<endl;
    cout<<"::                                                         ::"<<endl;
    cout<<"::                                                         ::"<<endl;
    cout<<"::                                                         ::"<<endl;
    brzeg_ramki();
system("PAUSE");
}


//**************************************************************
void wstep_i_menu_3()
{
system("CLS");
    brzeg_ramki();
    cout<<"::                                                         ::"<<endl;
    cout<<"::                                                         ::"<<endl;
    cout<<"::                                                         ::"<<endl;
    cout<<":: Masz do wyboru 4 tryby trudnosci, od najlatwiejszego:   ::"<<endl;
    cout<<":: 1. Piece of cake:                                       ::"<<endl;
    cout<<"::          4 ukryte, cyfry dostepne 1-4, 10 prob          ::"<<endl;
    cout<<":: 2. Let's rock:                                          ::"<<endl;
    cout<<"::          4 ukryte, cyfry dostepne 1-6, 10 prob          ::"<<endl;
    cout<<":: 3. Come get some:                                       ::"<<endl;
    cout<<"::          6 ukrytych, cyfry dostepne 1-6, 12 prob        ::"<<endl;
    cout<<":: 4. Damn I'm good:                                       ::"<<endl;
    cout<<"::          9 ukrytych, cyfry dostepne 1-9, 14 prob        ::"<<endl;
    cout<<":: Wybierz pozadany poziom (1-4):                          ::"<<endl;
    cout<<"::                                                         ::"<<endl;
    cout<<"::                                                         ::"<<endl;
    cout<<"::                                                         ::"<<endl;
    brzeg_ramki();

//trudnosc jest ustawiona jako zmienna char, zeby sie petla nie zawisila po wprowadzeniu jakiegos innego znaku
    trudnosc=getch();

switch (trudnosc)
{
case 49:
    cout<<"Piece of cake"<<endl;
    system("PAUSE");
    szerokosc_planszy=4;
    ilosc_kolorow=4;
    dlugosc_planszy=10;
    break;
case 50:
        cout<<"Let's rock"<<endl;
    system("PAUSE");
    szerokosc_planszy=4;
    ilosc_kolorow=6;
    dlugosc_planszy=10;
    break;
case 51:
    cout<<"Come get some"<<endl;
    system("PAUSE");
    szerokosc_planszy=6;
    ilosc_kolorow=6;
    dlugosc_planszy=12;
    break;
case 52:
    cout<<"Damn I'm good"<<endl;
    system("PAUSE");
    szerokosc_planszy=9;
    ilosc_kolorow=9;
    dlugosc_planszy=16;
    break;
default:
    cout<<"Zle wprowadzenie, pamietaj ze masz wybrac 1, 2, 3 lub 4..."<<endl;
    system("PAUSE");
    system("CLS");
    wstep_i_menu_3();
    break;
}
}


//**************************************************************
void wczytywanie_tabeli_wynikow_z_pliku()
{
//otwarcie pliku

for (int otwieranie=0;;otwieranie++)
{
    ifstream zpliku ("mastermind_wyniki.txt");
//wczytanie stringu z pliku i wpisywanie go w tabele zmiennych

//jezeli plik byl juz stworzony, to pieknie - odpalamy zczytywanie z niego. Jezeli nie, petla sprobuje go otworzyc, i jeszcze tu wrocimy.
    if (zpliku.is_open())
    {
        int zczytywanie1=0;
        int zczytywanie2=0;
        int zczytywanie3=0;
        char ksywka_temp[3];

        for (zczytywanie2=0;zczytywanie2<10;zczytywanie2++)
        {
            zczytywanie3=0;
            zczytywanie1=0;
//petla do przepisania wartosci z pliku (stream -> string -> wartosci w odpowiednich tabelach int i char)
            for (std::string linia;std::getline (zpliku,linia);)
            {
//jezeli mamy do czynienia z 4 znakiem stringu, to jest to wynik numeryczny, i musimy go wpisac do odpowiedniego miejsca w tabeli intow)
                if (zczytywanie1%4==3)
                {
                    istringstream iss (linia);
                    iss>>tabela_wynikow[zczytywanie3][zczytywanie2];
                    zczytywanie3++;
                }
//jesli nie, to znaczy, ze jest to fragment 3-literowego inicjalu grajacego, ktory trzeba wpisac do odpowiedniej(kolejnej) komorki tabeli ksywek. uzywamy przejsciowej tabeli znakowej, zeby dalo
//sie wpisac jednoliterowy string i jego koncowke \n, a potem wyciagamy juz tylko pierwszy argument tabeli, ktory bedzie porzadanym przez nas znakiem.
                else
                {
                    ksywka_temp[0]='a';
                    ksywka_temp[1]='a';
                    ksywka_temp[2]='a';
                    strcpy (ksywka_temp,linia.c_str());

                    tabela_ksywek[zczytywanie1-(zczytywanie1/4)][zczytywanie2]=ksywka_temp[0];
                }
//jezeli petla osiagnela 15, oznacza to, ze zczytalismy 16 arumentow (12 charow (czyli 4 ksywki) i 4 inty (wyniki dla tych ksywek)) - czyli caly rzad zostal przepisany. Wychodzimy z petli dla tego
//rzedu, i zaczynamy przepisywac kolejny rzad.
                if (zczytywanie1==15)
                    break;
                zczytywanie1++;
            }
        }
        zpliku.close();
        break;
    }
//jezeli wszystko zawiodlo, i w petli nie udalo sie ani zczytac wynikow, ani stworzyc pustego pliku i zczytac wynikow, to wychodzimy z petli, i wyniki beda dostepne tylko lokalnie.
//ta mozliwosc jest zarezerwowana na wypadek sytuacji, w ktorych plik .exe gry jest umieszczony w repertuarze, ktory odmowi mu praw do stworzenia sobie pliku. Czasem nie da sie walczyc z pradem :P
    if (otwieranie==1)
    {
        cout<<"Niestety, uzyskane wyniki beda widoczne tylko w czasie bierzacej sesji gier"<<endl;
        _sleep(1500);
        break;
    }

    {
//tworzenie wyzerowanego pliku

    ofstream dopliku ("mastermind_wyniki.txt", ios::trunc);
//zapisanie wyzerowanych danych

    if (dopliku.is_open())
    {

        for (int wpisywanie=0;wpisywanie<160;wpisywanie++)
        {
            if (wpisywanie%4<3)
            dopliku<<" "<<'\n';
            else
            dopliku<<0<<'\n';
        }
    dopliku.close();
    }
    }
}
}

//**************************************************************
void wyswietlanie_tabeli_wynikow()
{
int wyniki1=0;
int wyniki2=0;
int wyniki3=0;
int ksywki=0;

system("CLS");
brzeg_ramki();
cout<<"::                                                         ::"<<endl;
cout<<"::    Baked:    |   Rocked:   |    Came:    |    Owned:    ::"<<endl;
for (wyniki2=0;wyniki2<10;wyniki2++)
{
    wyniki3=0;
    cout<<"::    ";
    for (wyniki1=0;wyniki1<4;wyniki1++)
    {
        for (ksywki=0;ksywki<3;ksywki++)
        {
            cout<<tabela_ksywek[wyniki3][wyniki2];
            wyniki3++;
        }

        cout<<"  "<<tabela_wynikow[wyniki1][wyniki2];
        if (tabela_wynikow[wyniki1][wyniki2]<=9)
            cout<<" ";
        if (wyniki1==3)
            break;
        cout<<"   |   ";
    }
    cout<<"    ::"<<endl;
}
cout<<"::                                                         ::"<<endl;
cout<<"::                                                         ::"<<endl;
brzeg_ramki();
}


//**************************************************************
void zapisywanie_wyniku_do_tabeli()
{
int kolumna_wyniki=0;
int kolumna_ksywki=0;
int rzad_wyniki=0;
bool warunek_wpisanie=0;
int miejsce=0;

    if (trudnosc==49)
    {
        kolumna_wyniki=0;
        kolumna_ksywki=0;
    }
    if (trudnosc==50)
    {
        kolumna_wyniki=1;
        kolumna_ksywki=3;
    }
    if (trudnosc==51)
    {
        kolumna_wyniki=2;
        kolumna_ksywki=6;
    }
    if (trudnosc==52)
    {
        kolumna_wyniki=3;
        kolumna_ksywki=9;
    }

    for (rzad_wyniki=0;rzad_wyniki<=9;rzad_wyniki++)
    {
        if ((tabela_wynikow[kolumna_wyniki][rzad_wyniki]>=rzad)||(tabela_wynikow[kolumna_wyniki][rzad_wyniki]==0))
        {
            miejsce = rzad_wyniki;
            warunek_wpisanie=1;
            break;
        }

    }

    if (warunek_wpisanie==1)
    {
        for (rzad_wyniki=9;rzad_wyniki>=miejsce;rzad_wyniki--)
        {
            tabela_wynikow[kolumna_wyniki][rzad_wyniki+1]=tabela_wynikow[kolumna_wyniki][rzad_wyniki];
            tabela_ksywek[kolumna_ksywki][rzad_wyniki+1]=tabela_ksywek[kolumna_ksywki][rzad_wyniki];
            tabela_ksywek[kolumna_ksywki+1][rzad_wyniki+1]=tabela_ksywek[kolumna_ksywki+1][rzad_wyniki];
            tabela_ksywek[kolumna_ksywki+2][rzad_wyniki+1]=tabela_ksywek[kolumna_ksywki+2][rzad_wyniki];
        }

        tabela_wynikow[kolumna_wyniki][miejsce]=rzad;

        for(int pisownia=0;;pisownia++)
        {
            tabela_ksywek[kolumna_ksywki][miejsce]=' ';
            tabela_ksywek[kolumna_ksywki+1][miejsce]=' ';
            tabela_ksywek[kolumna_ksywki+2][miejsce]=' ';
            wyswietlanie_tabeli_wynikow();
            if(pisownia>0)
            cout<<"Zle wprowadzenie. ";
            cout<<"Wprowadz 3 litery jako swoj podpis"<<endl;

            tabela_ksywek[kolumna_ksywki][miejsce]=getch();
            if (tabela_ksywek[kolumna_ksywki][miejsce]<33 || tabela_ksywek[kolumna_ksywki][miejsce]>125)
                continue;
            cout<<tabela_ksywek[kolumna_ksywki][miejsce];
            tabela_ksywek[kolumna_ksywki+1][miejsce]=getch();
            if (tabela_ksywek[kolumna_ksywki+1][miejsce]<33 || tabela_ksywek[kolumna_ksywki][miejsce]>125)
                continue;
            cout<<tabela_ksywek[kolumna_ksywki+1][miejsce];
            tabela_ksywek[kolumna_ksywki+2][miejsce]=getch();
            if (tabela_ksywek[kolumna_ksywki+2][miejsce]<33 || tabela_ksywek[kolumna_ksywki][miejsce]>125)
                continue;
            cout<<tabela_ksywek[kolumna_ksywki+2][miejsce];
            break;
        }

    }
    else
    {
        cout<<"Niestety, nie udalo Ci sie pobic zadnego wyniku"<<endl;
        system("PAUSE");
    }
}


//**************************************************************
void zapisywanie_tabeli_wynikow_do_pliku()
{
//otwarcie pliku z wymazaniem jego aktualnej tresci

ofstream dopliku ("mastermind_wyniki.txt", ios::trunc);
//dopliku.open ("mastermind_wyniki.txt", ios::trunc | ios::in);

//zapisanie danych

if (dopliku.is_open())
{
    int wpisywanie1=0;
    int wpisywanie2=0;
    int wpisywanie3=0;
    for (wpisywanie2=0;wpisywanie2<10;wpisywanie2++)
    {
        wpisywanie3=0;
        for (wpisywanie1=0;wpisywanie1<=11;wpisywanie1++)
        {
            dopliku<<tabela_ksywek[wpisywanie1][wpisywanie2]<<'\n';
            if (wpisywanie1%3==2)
            {
                dopliku<<tabela_wynikow[wpisywanie3][wpisywanie2]<<'\n';
                wpisywanie3++;
            }
        }
    }
    dopliku.close();
}


}


//**************************************************************
void warunki_zwyciestwa()
{
    if (zwyciestwo==1)
        {
        ekran_zwyciestwa();
//Po obejrzeniu animacji zwyciestwa pojawi sie tabela wynikow z mozliwoscia wpisywania do niej
//# tutaj zamiescic przejscie do funkcji z tabela wynikow
        wczytywanie_tabeli_wynikow_z_pliku();
        zapisywanie_wyniku_do_tabeli();
        zapisywanie_tabeli_wynikow_do_pliku();
        wyswietlanie_tabeli_wynikow();
        system("PAUSE");
        }
    if (zwyciestwo==2)
//jezeli przegralismy, wyswietli sie wlasciwa odpowiedz, po czym ekran porazki
    {
        cout<<"Wlasciwa odpowiedz to: "<<endl;
        for (int odpowiedz=0;odpowiedz<=szerokosc_planszy;odpowiedz++)
        {
            cout<<zakryte_liczby[odpowiedz]<<" ";
        }
        system("PAUSE");
        ekran_porazki();
    }
}


//**************************************************************
void zapytanie_o_powtorke()
{
    for(;;)
    {
           system("CLS");
        brzeg_ramki();
        for(int i=13;i>=0;i--)
        {
            cout<<"::                                                         ::"<<endl;
            if (i==6)
            {
                cout<<"::              Czy chcesz zagrac jeszcze raz?             ::"<<endl;
                cout<<"::                     1: tak  / 2: nie                    ::"<<endl;
            }
        }
        brzeg_ramki();
        wybor=getch();
        if (wybor==49 || wybor==50)
            break;
    }

}


//**************************************************************
void ekran_pozegnania()
{
    system("CLS");
    brzeg_ramki();
    for(int i=13;i>=0;i--)
    {
        cout<<"::                                                         ::"<<endl;
        if (i==6)
        {
            cout<<"::      Dziekuje za gre, mam nadzieje ze sie podobalo!     ::"<<endl;
            cout<<"::                                   Jakub Komacki         ::"<<endl;
        }
    }
    brzeg_ramki();
    _sleep(1500);
}


//**************************************************************
int main()
{
srand((unsigned)time(NULL));

    ekran_startowy();
    wstep_i_menu_1();
    wstep_i_menu_2();
//jezeli ladujemy wyniki z pliku, to ta funkcja nie jest potrzebna - zostawiam ja tylko na wypadek, gdyby pliku nie udalo sie stworzyc.
    zerowanie_tabeli_wynikow_i_ksywek();
for(;;)
{
    wstep_i_menu_3();
    zerowanie_zmiennych();
//poniewaz zmienna szerokosc_planszy bedzie sie odnosic do tabel, to konwertuje ja od razu na format "tabelowy"
    szerokosc_planszy--;
//poniewaz zmienna dlugosc_planszy bedzie sie odnosic do tabel, to konwertuje ja od razu na format "tabelowy"
    dlugosc_planszy--;

//Odpalenie generatora liczb ktore bedziemy zgadywac
    generowanie_liczb_zakrytych();

/*
//Petelka testowa na wyswietlenie, czy liczby sie dobrze generuja.
///////////////////////////////////////////////////////////////////////////////////
        for(int i=0; i<=szerokosc_planszy; i++)
        {
            cout<<zakryte_liczby[i]<<" ";
        }
        system("PAUSE");
///////////////////////////////////////////////////////////////////////////////////
*/

//##########################################################
//czesc glowna odpowiedzialna za mechanizm umieszczania strzalow na planszy -
    wyswietlanie_planszy();

    for(int rzad=0;rzad<=dlugosc_planszy;rzad++)
    {
        wprowadzanie_strzalu();
        obliczanie_punktacji();
        if (zwyciestwo>0)
            break;
    }
//w funkcji warunki zwyciestwa rozpisany jest caly mechanizm zwiazany z tabela wynikow, odczytywaniem jej z pliku, wypelnianiem i zapisywaniem do pliku.
    warunki_zwyciestwa();
    zapytanie_o_powtorke();

//W funkcji pytanie o powtorke okreslana jest wartosc zmiennej - main jest w petli, wiec odpowiedz na to pytanie pozwoli ewentualnie z niej wyjsc, zeby nie powatarzac gry.
    if (wybor==49)
        continue;
    if (wybor==50)
        break;

}
    ekran_pozegnania();
}
