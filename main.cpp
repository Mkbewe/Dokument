#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <ctime>
#include <algorithm>
#include <fstream>

//zmienne sterujace opcjami gry

int speed_movment =100;
int bonus =300;       // czestotliwosc wystepowania bonusu
int dificult = 2;
bool goodmode = false;
int timeOver =120;    // czas gry na czas
int time_delay = 10; //opoznienie znikania bonusu

struct Ranking
{
    int score = 0;
    char nick [10];
    int length = 0;


    bool operator < (const Ranking &x)const //to umzliwa sortowanie ni mom pojecia jak dziala
    {
        return score>x.score;
    }

};

enum color {red =12,yellow =14};

using namespace std;

HANDLE uchwyt = GetStdHandle(STD_OUTPUT_HANDLE);

void cls(); //czyszczenie ekranu
int menu_main (Ranking []);
int gamemode ();
void gotoxy(int x, int y); //do ustawiania kursora w pozycji x,y
void show_screan ();        //pokazuje plansze
void move_sanke (int,int&,int&);
void draw_sanke (const int&,const int&);
int change_direction (int);
int apple (int &,int &, color);
void show_score (int);
int show_time (clock_t, int x=0);
void draw_sanke_body (int[],int[],int);
void add_body (const int&,const int&,int[],int[],int);
int options ();
bool collision (int [],int[],int,int,int);
void add_player (Ranking[],int);
void ranking (Ranking player[]);
void ranking_save (Ranking []);
void ranking_load (Ranking []);

int main()
{
    srand( time( NULL ) );
    Ranking player[50];


    while (true)
    {

        menu_main(player);
        bool game_mode = gamemode ();

        int direction=3;                                        // wybor kierunku poruszania
        int position_x=50,position_y=15;                        // pozycja glowy weza
        int tab_position_x[1000] {0},tab_position_y[1000] {0};  // pozycja ciala weza
        int apple_x,apple_y;                                    // pozycja czerwonego jabłka
        int apple1_x,apple1_y;                                  // pozycja zoltego jablka
        int score=2;                                            // punkty
        int random;                                             // zmienna do ustawiania prawdopodobienstwa zoltego jakbka
        bool randimize = true;                                 // warunek do ponownego losowania zoltego jablka
        int apple_time_live=5000;

        apple_x =(rand() % 74 ) + 26;
        apple_y =(rand() % 20 ) + 4;



        clock_t start;
        start=clock();


        show_screan();

        while (true)
        {

          int game_time = show_time (start);

           if (game_mode)
           {
               int game_time = show_time (start,timeOver);
               if (timeOver==game_time)
                {
                Sleep(1000);
                cls();
                gotoxy(35,10);
                cout<<"Koniec gry, twoj wynik to: "<<(score-2)*dificult;

                gotoxy(35,12);
                add_player (player, score);
                ranking_save(player);
                getch();
                break;
                }

           }


            if (score==2)
                show_score(score);

            direction=change_direction (direction);
            if (direction==-1)
                break;

            //while (kbhit())
            //   getch();

            add_body(position_x,position_y,tab_position_x,tab_position_y,score);

            move_sanke (direction,position_x,position_y);

            apple(apple_x,apple_y,red);

            if (apple_x==position_x&&apple_y==position_y)
            {
                apple_x =(rand() % 74 ) + 26;
                apple_y =(rand() % 20 ) + 4;

                /* petla sprawdza czy jabłko nie pojawilo sie
                w ciele wezam jesli tak to losuje je ponownie*/

                for (int i=0;i<score;i++)
                {
                  if (apple_x==tab_position_x[i]&&apple_y==tab_position_y[i])
                  {
                      apple_x =(rand() % 74 ) + 26;
                      apple_y =(rand() % 20 ) + 4;
                      i =0;
                  }
                }

                score++;
                show_score(score);
            }

            //ify do losowania zlotego jakbłka oraz do usuwania go po okrslonym czasie
            // strasznie zamotane

                if (apple_time_live+time_delay==game_time)
                {
                  randimize=true;

                  char apple_clear=255;
                  gotoxy(apple1_x,apple1_y);
                  cout<<apple_clear;
                  apple1_y=28;
                  gotoxy(1,25);
                }

               if (game_time>10)
                    random =(rand() % bonus ) + 1;
               else
                random =75;

            if (random==100&&randimize==true)
            {
                randimize = false;

                apple1_x =(rand() % 74 ) + 26;
                apple1_y =(rand() % 20 ) + 4;

                apple(apple1_x,apple1_y,yellow);
                apple_time_live = game_time;



            }
                if (apple1_x==position_x&&apple1_y==position_y)
          {
                randimize=true;

                apple1_y=28;
                score=score+5;
                show_score(score);

          }

            draw_sanke(position_x,position_y);
            draw_sanke_body (tab_position_x,tab_position_y,score);

            if (!goodmode)
            if (collision (tab_position_x,tab_position_y,position_x,position_y,score))
            {
                Sleep(1000);
                cls();
                gotoxy(35,10);
                cout<<"Przegrales twoj wynik to: "<<(score-2)*dificult;

                gotoxy(35,12);
                add_player (player, score);
                ranking_save(player);
                getch();
                break;
            }


            /* cout<<endl<<"x "<<position_x<<" y "<<position_y;
             cout<<endl<<"x tab0 "<<tab_position_x[0]<<" y tab0 "<<tab_position_y[0];
             cout<<endl<<"x tab1 "<<tab_position_x[1]<<" y tab1 "<<tab_position_y[1];
             cout<<endl<<"x tab2 "<<tab_position_x[2]<<" y tab2 "<<tab_position_y[2];
             cout<<endl<<"x tab3 "<<tab_position_x[3]<<" y tab3 "<<tab_position_y[3];

             */
            Sleep(speed_movment);
        }

    }
    return 0;
}

int menu_main (Ranking player[])
{
    int wybranaopcja =1; // wyswietla opcje
    char znak; // zczytuje znak z klawiatury
    int wybor=0; // zatwierdza opcje

    while( wybranaopcja != 5 )
    {
        cls();

        SetConsoleTextAttribute(uchwyt, 15);
        cout<<"==================================================\n";
        cout<<"                      Snake\n";
        cout<<"==================================================\n";
        SetConsoleTextAttribute(uchwyt, 7);

        if( wybranaopcja == 0 )
        {
            cout.width(30);     // niezbyt eleganckie ustawianie pozycji tekstu
            SetConsoleTextAttribute(uchwyt, 15);
            cout << "New  Game\n";
            SetConsoleTextAttribute(uchwyt, 7);
            wybor=0;
        }
        else
        {
            cout.width(30);
            cout << "New  Game\n";
        }
        if( wybranaopcja == 1 )
        {
            SetConsoleTextAttribute(uchwyt, 15);
            cout.width(29);
            cout << " Ranking\n";
            SetConsoleTextAttribute(uchwyt, 7);
            wybor=1;
        }
        else
        {
            cout.width(29);
            cout << "Ranking\n";
        }

        if( wybranaopcja == 2 )
        {
            SetConsoleTextAttribute(uchwyt, 15);
            cout.width(29);
            cout << "Options\n";
            SetConsoleTextAttribute(uchwyt, 7);
            wybor=2;
        }
        else
        {
            cout.width(29);
            cout << "Options\n";
        }

        if( wybranaopcja == 3 )
        {
            SetConsoleTextAttribute(uchwyt, 15);
            cout.width(28);
            cout << "Exit\n";
            SetConsoleTextAttribute(uchwyt, 7);
            wybor=3;
        }
        else
        {
            cout.width(28);
            cout << "Exit\n";
        }
        znak = getch();

        switch( znak )
        {
        case 72: //strzalka w gore
        case 'w':
            wybranaopcja--;
            break;
        case 80:
        case 's':
            wybranaopcja++;
            break;
        case 13:
        case 32:
            if (wybor==0)
            {
                cls();
                return 0;
            }
            if (wybor==1)
            {
                cls();
                ranking (player);
                continue;
            }

            if (wybor==2)
            {
                cls();
                options ();
                continue;
            }

            if (wybor==3)
            {
                cls();
                cout<<"The End \n";
                Sleep(700);
                exit(0);
            }
        case 27:
            cls();
            cout<<"The End \n";
            Sleep(700);
            exit(0);
        }
        if (wybranaopcja!=5)
        {
            if( wybranaopcja < 0 ) wybranaopcja++;
            if( wybranaopcja > 3 ) wybranaopcja--;
        }

    }

}

int gamemode ()

{
    int wybranaopcja =0; // wyswietla opcje
    char znak; // zczytuje znak z klawiatury
    int wybor=0; // zatwierdza opcje

    while( wybranaopcja != 2 )
    {
        cls();

        SetConsoleTextAttribute(uchwyt, 15);
        cout<<"==================================================\n";
        cout<<"                   Game Mode\n";
        cout<<"==================================================\n";
        SetConsoleTextAttribute(uchwyt, 7);

        if( wybranaopcja == 0 )
        {
            cout.width(31);
            SetConsoleTextAttribute(uchwyt, 15);
            cout << "Standard game\n";
            SetConsoleTextAttribute(uchwyt, 7);
            wybor=0;
        }
        else
        {
            cout.width(31);
            cout << "Standard game\n";ww
        }
        if( wybranaopcja == 1 )
        {
            SetConsoleTextAttribute(uchwyt, 15);
            cout.width(29);
            cout << " Time game\n";
            SetConsoleTextAttribute(uchwyt, 7);
            wybor=1;
        }
        else
        {
            cout.width(29);
            cout << "Time game\n";
        }


        znak = getch();

        switch( znak )
        {
        case 72: //strzalka w gore
        case 'w':
            wybranaopcja--;
            break;
        case 80:
        case 's':
            wybranaopcja++;
            break;
        case 13:
        case 32:
            if (wybor==0)
            {
                cls();
                cout<<"Loading...";
                Sleep(500);
                cls();
                return 0;
            }
            if (wybor==1)
            {
                cls();
                cout<<"Loading...";
                Sleep(500);
                cls();
                return 1;
            }


        }
        if (wybranaopcja!=1)
        {
            if( wybranaopcja < 0 ) wybranaopcja++;
            if( wybranaopcja > 1 ) wybranaopcja--;
        }

    }

}


void cls()
{
    HANDLE hStdout;
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = { 0, 0 };     // pozycja pocz¹tkowa kursora
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    // Pobranie iloœci znaków w buforze konsoli
    if( !GetConsoleScreenBufferInfo( hStdout, &csbi ))
        return;
    // Okreœlenie, z ilu znaków sk³ada siê ekran konsoli (szerokoœæ * wysokoœæ)
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    // Zapisanie ca³ego ekranu spacjami
    if( !FillConsoleOutputCharacter( hStdout, (TCHAR) ' ',
                                     dwConSize, coordScreen, &cCharsWritten ))
        return;

    // Pobranie atrybutów (ustawieñ) tekstu (czyli np kolor itd)
    if( !GetConsoleScreenBufferInfo( hStdout, &csbi ))
        return;

    // Ustawienie w konsoli odczytanych wczeœniej atrybutów
    if( !FillConsoleOutputAttribute( hStdout, csbi.wAttributes,
                                     dwConSize, coordScreen, &cCharsWritten ))
        return;

    // Ustawienie kursora na pozycji startowej
    SetConsoleCursorPosition( hStdout, coordScreen );
}

void gotoxy(int x, int y)
{
    COORD c;
    c.X = x - 1;
    c.Y = y - 1;
    SetConsoleCursorPosition (GetStdHandle (STD_OUTPUT_HANDLE), c);
}

void show_screan ()
{
    cls();
    SetConsoleTextAttribute(uchwyt, 15);
    cout<<"================================================================================"<<
        "====================\n"; //100

     gotoxy(58,2);
    cout<<"<SNAKE>";

    gotoxy(4,5);
    cout<<"Press \"P\" to Pause";
    gotoxy(4,7);
    cout<<"Press ESC to return\n      to mine menu";
    gotoxy(7,10);
    cout<<"Control WSAD";


    gotoxy(1,3);
    cout<<"================================================================================"<<
        "====================\n"; //100
    gotoxy(1,2);

    cout<<"|\n\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n"; //24
    cout<<"================================================================================"<<
        "====================\n"; //100
    for (int i=2; i<24; i++)
    {
        gotoxy(25,i);
        if (i==3)
            continue;
        cout<<"|";
    }
    for (int i=2; i<24; i++)
    {
        gotoxy(100,i);
        if (i==3)
            continue;
        cout<<"|";
    }
    SetConsoleTextAttribute(uchwyt, 7);
    gotoxy(1,25);
}

int change_direction (int last)
{
    char direction;

    if (kbhit())
    {
        direction = getch();
        if (direction==27)
        {
            cls();
            return -1;
        }
        else if (direction==112)
        {
            getchar();
            return last;
        }

        else if (direction=='w'||direction=='s'||direction=='a'||direction=='d')
        {
            switch( direction )
            {
            case 'w': //strzalka w gore
                if (last==1)
                    return 1;
                return 0;
            case 's': //strzalka w dol
                if (last==0)
                    return 0;
                return 1;
            case 'a': //strzalka w lewo
                if (last==3)
                    return 3;
                return 2;
            case 'd': //strzalka w prawo
                if (last==2)
                    return 2;
                return 3;
            }
        }
        else return last;
    }
    else return last;
}

void move_sanke (int direction,int &x,int &y)

{
    switch (direction)
    {
    case 0: // w gore
        if (y<5)
            y=24;
        y--;
        break;
    case 1: // w dol
        if (y>22)
            y=3;
        y++;
        break;
    case 2: //w lewo
        if (x<27)
            x=100;
        x--;
        break;
    case 3: // w prawo
        if (x>98)
            x=25;
        x++;
        break;
    }

}

void draw_sanke (const int &positionx,const int &positiony)

{

    gotoxy(positionx,positiony);
    char snake_head=254;
    SetConsoleTextAttribute(uchwyt, 10);
    cout<<snake_head;
    SetConsoleTextAttribute(uchwyt, 15);
    gotoxy(1,25);

}

int apple (int &apple_x,int &apple_y,color choose)

{


    char apple='o';
    SetConsoleTextAttribute(uchwyt, choose);
    gotoxy(apple_x,apple_y);
    cout<<apple;
    SetConsoleTextAttribute(uchwyt, 15);
    gotoxy(1,25);
}

void show_score (int score)
{
    gotoxy(80,2);
    cout<<"SCORE: "<<(score-2)*dificult;
    gotoxy(32,2);
    cout<<"Snake length: "<<score;
    gotoxy(1,25);

}

int show_time (clock_t start, int x)
{
    clock_t stop;
    stop=clock();
    int time;
    time=(int)(stop-start)/CLOCKS_PER_SEC;

    gotoxy(10,2);
    if (x==0)
    cout<<"Time: "<<time;
    else
        if (x-time<10)
        cout<<"Time: "<<0<<0<<x-time;

        else if (x-time<100)
        cout<<"Time: "<<0<<x-time;

        else
     cout<<"Time: "<<x-time;

    gotoxy(1,25);
    return time;
}

void add_body (const int &head_x,const &head_y,int body_x[],int body_y[],int length)
{
    if (length==1)
    {
        body_x[0]=head_x;
        body_y[0]=head_y;
    }

    else if (length>1)
    {
        int i=length;
        for (i; i>0; i--)
        {
            body_x[i]=body_x[i-1];
            body_y[i]=body_y[i-1];
        }
        body_x[0]=head_x;
        body_y[0]=head_y;
    }

}

void draw_sanke_body (int body_x[],int body_y[],int snake_length)

{
    char snake_head=254;
    char snake_clear=255;
    int sanke_clear_position=snake_length;
    while (snake_length)
    {

        gotoxy(body_x[snake_length-1],body_y[snake_length-1]);
        SetConsoleTextAttribute(uchwyt, 2);
        cout<<snake_head;
        SetConsoleTextAttribute(uchwyt, 15);
        gotoxy(1,25);
        snake_length--;
    }

    gotoxy(body_x[sanke_clear_position],body_y[sanke_clear_position]);

    cout<<snake_clear;

    gotoxy(1,25);

}

int options ()
{
    int wybranaopcja =1; // wyswietla opcje
    char znak; // zczytuje znak z klawiatury
    int wybor=0; // zatwierdza opcje

    while( wybranaopcja != 6 )
    {
        cls();

        SetConsoleTextAttribute(uchwyt, 15);
        cout<<"==================================================\n";
        cout<<"                      Options\n";
        cout<<"==================================================\n";
        cout.width(37);
        cout<<"Poziom trudnosci: \n\n";
        SetConsoleTextAttribute(uchwyt, 7);

        if( wybranaopcja == 0 )
        {
            cout.width(27);
            SetConsoleTextAttribute(uchwyt, 15);
            cout << "Easy\n";
            SetConsoleTextAttribute(uchwyt, 7);
            wybor=0;
        }
        else
        {
            cout.width(27);
            cout << "Easy\n";
        }
        if( wybranaopcja == 1 )
        {
            SetConsoleTextAttribute(uchwyt, 15);
            cout.width(28);
            cout << "Medium\n";
            SetConsoleTextAttribute(uchwyt, 7);
            wybor=1;
        }
        else
        {
            cout.width(28);
            cout << "Medium\n";
        }

        if( wybranaopcja == 2 )
        {
            SetConsoleTextAttribute(uchwyt, 15);
            cout.width(28);
            cout << "Hard\n\n";
            SetConsoleTextAttribute(uchwyt, 7);
            wybor=2;
        }
        else
        {
            cout.width(28);
            cout << "Hard\n\n";
        }

        if( wybranaopcja == 3 )
        {
            SetConsoleTextAttribute(uchwyt, 15);
            cout.width(31);
            cout << "Good Mode On\n";
            SetConsoleTextAttribute(uchwyt, 7);
            wybor=3;
        }
        else
        {
            cout.width(31);
            cout << "Good Mode On\n";
        }

        if( wybranaopcja == 4 )
        {
            SetConsoleTextAttribute(uchwyt, 15);
            cout.width(32);
            cout << " Good Mode Off\n";
            SetConsoleTextAttribute(uchwyt, 7);
            wybor=4;
        }
        else
        {
            cout.width(32);
            cout << "Good Mode Off\n";
        }
        znak = getch();

        switch( znak )
        {
        case 72: //strzalka w gore
        case 'w':
            wybranaopcja--;
            break;
        case 80:
        case 's':
            wybranaopcja++;
            break;
        case 13:
        case 32:
            if (wybor==0)
            {
                cls();
                speed_movment=150;
                bonus = 200;
                dificult =1;
                time_delay =13;
                return 0;
            }
            if (wybor==1)
            {
                cls();
                speed_movment=100;
                bonus = 300;
                dificult =2;
                time_delay = 10;
                return 0;
            }

            if (wybor==2)
            {
                cls();
                speed_movment=50;
                bonus = 450;
                dificult =3;
                time_delay = 7;
                return 0;
            }
            if (wybor==3)
            {
                cls();
                goodmode = true;
                return 0;
            }
            if (wybor==4)
            {
                cls();
                 goodmode = false;
                return 0;
            }

        case 27:
            cls();
            return 0;
        }
        if (wybranaopcja!=6)
        {
            if( wybranaopcja < 0 ) wybranaopcja++;
            if( wybranaopcja > 4 ) wybranaopcja--;
        }

    }

}

bool collision (int body_x[],int body_y[],int head_x,int head_y,int length)

{
    for (int i = 0; i<=length; i++)
    {
        if (body_x[i]==head_x&&body_y[i]==head_y)
            return true;
    }
    return false;

}


void add_player (Ranking player[],int score)

{

    for (int i =0; i<50; i++)
    {
        if (player[i].score==0)
        {
            cout<<"Podaj swoj nick: ";
            while (kbhit())
                getch();

            player[i].score=(score-2)*dificult;
            player[i].length=score;
            cin.getline (player[i].nick, 10);
            break;
        }
    }
}

void ranking (Ranking player[])

{
    ranking_load (player);

    sort (player,player+50);
    cout<<"       Nick                                    Score       Length\n\n";

    for (int i=0; i<20; i++)
    {
        if (player[i].score==0)
        {
            cout.width( 1 );
            cout<<right<<"[";
            cout.width( 2 );
            cout<<right<<i+1;
            cout.width( 1 );
            cout<<right<<"] ";
            cout<<"           ..............................   0   .....    0\n";

        }
        else
        {
            cout.width( 1 );
            cout<<right<<"[";
            cout.width( 2 );
            cout<<right<<i+1;
            cout.width( 1 );
            cout<<right<<"] ";
            cout.width( 10 );
            cout<<left<<player[i].nick;

            cout<<" .............................. ";

            cout.width( 3 );
            cout<<internal<<player[i].score;
            cout<<"   .....    ";
            cout<<internal<<player[i].length<<endl;

        }
    }

    getch();
}

void ranking_save (Ranking player[])

{
    fstream plik;
    plik.open("ranking.txt",ios::out );
    int i=0;
    while (player[i].score!=0)
    {
        plik<<player[i].nick<<endl;
        plik<<player[i].score<<endl;
        plik<<player[i].length<<endl;
        i++;
    }
    plik.close();
}

void ranking_load (Ranking player[])

{

    fstream plik;
    plik.open("ranking.txt",ios::in);
    if(plik.good()==false)
    {
        cout<<"Plik nie istnieje";
    }

    string linia;
    string snick;
    int nr_lini=1,index=0;

    while (getline(plik,linia))
    {
        switch(nr_lini)
        {
        case 1:
            snick=linia;
            for (unsigned int i=0; i<snick.size(); i++)
            {
                player[index].nick[i]=snick[i];
            }
            player[index].nick[snick.size()]='\0';
            break;
        case 2:
            player[index].score=atoi(linia.c_str());
            break;
        case 3:
        player[index].length=atoi(linia.c_str());
        break;
        }

        nr_lini++;
        if (nr_lini==4)
        {
            nr_lini=1;
            index++;
        }
    }

    plik.close();
}
