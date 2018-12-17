#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

/* STALE */
#define MILLISEC 1000
#define BOARD_WIDTH 70
#define BOARD_HEIGHT 20

/* STRUKTURY */
struct snakePart {
    int x,y; /* polozenie weza w przestrzeni 2D */
    struct snakePart *next; /* polaczenie z nastepnym elementem listy */
};

struct cherryXY {
    int x,y; /* polozenie 'wisienki' w przestrzeni 2D */
    int exist; /* stan wisienki 0-nie istnieje 1-istnieje */
};

/* TYPY */
typedef struct snakePart segment;
typedef struct cherryXY cherry;

/* ZMIENNE GLOBALNE */
int defDel=500; /* CZAS POMIEDZY ODSWIERZENIAMI */
int points=0; /* PUNKTY */
int dir=1; /* KIERUNEK 1-lewo, 2-dol, 3-prawo, 5-gora */
int znacznik=0; /* Znacznik zycia/smierci */
cherry *addPlus; /* WISIENKA */

/* PROTOTYPY FUNKCJI */
void clrScr();
void startGame();
void addElement (segment *start);
int gamePlay (segment *waz);
int printBoard (segment *start);
void wisienki (int tab[BOARD_HEIGHT][BOARD_WIDTH]);
int moveSnake (segment *glowa);
void sterowanie ();
int kolizje (segment *tmp);
void delay (int timeDel);
void destroySnake (segment *snake);


main () {
    int decision=0;
    
    srand(time(NULL));
    addPlus=(cherry *)malloc(sizeof(cherry)); /* INICJALIZACJA WISIENKI */
    
    printf("Witaj w mojej grze typu \"SNAKE\"\n\n----------------------------------------------\n\nSterowanie odbywa sie za pomoca klawiszy\nW (gora) S (dol) A (lewo) D (prawo)\n\n----------------------------------------------\n\nn - zaczyna nowa gre :)\nk - zakancza gre :(\nCo wybierasz?");
    
    while (decision!=107) {
        if (decision==110) {
            startGame();
            system("cls");
            if (points!=BOARD_WIDTH*BOARD_HEIGHT-4) {
                printf("\n\nZdazyles/as zjesc:");
            }
            else {
                printf("\n\nGRATULACJE!!\n\n\nPrzeszedles/as CALA gre\n\nZjadles/as:"); 
            }
            printf(" %d wisienek\n\n----------------------------------------------\n\nn - zaczyna gre od nowa :)\nk - koniec gry :(\n",points);
        }
        decision=getch();
    }
    free(addPlus);
    printf("\nDziekuje za gre :)\nGrzegorz Krzeminski\n\nENTER aby zamknac");
    
    getchar();
}

void clrScr() {
   COORD kordy = {0,0};
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), kordy);
}
void startGame() {
    segment *waz;
    int i;
    
    /* WYZEROWANIE ZMIENNYCH */
    points=0;
    defDel=500;
    dir=1;
    znacznik=0;
    addPlus->x=0;
    addPlus->y=0;
    addPlus->exist=0;
    
    /* ZAINICJOWANIE WEZA */
    waz=(segment *)malloc(sizeof(segment));
    waz->x=(int)BOARD_WIDTH/2;
    waz->y=(int)BOARD_HEIGHT/2;
    waz->next=NULL;
    for (i=0;i<3;i++) {
        addElement(waz);
    }
    while (znacznik==0) {
        clrScr();
        znacznik=gamePlay(waz);
        if ((defDel-points*40)>0) {
            delay(defDel-points*10);
        }
        else {
            delay(300);
        }
    }
    destroySnake (waz);
}
void addElement (segment *start) { /* DODAJE NOWY SEGMENT WEZA */
    segment *tmp, *newest;
    int x,y;
    
    tmp=start;
    
    while (tmp->next!=NULL) {
          tmp=tmp->next;
    }
    
    newest=(segment *)malloc(sizeof(segment));
    newest->x=tmp->x+1;
    newest->y=tmp->y;
    newest->next=NULL;
    tmp->next=newest;
}
int gamePlay (segment *waz) {
    int pom;
    znacznik=moveSnake(waz);
    if (znacznik<1 && points+4!=BOARD_WIDTH*BOARD_HEIGHT) {
        pom=printBoard(waz);
        return 0;
    }
    else {
        return 1;
}
         
}
int printBoard (segment *start) {
    int i,j,board[BOARD_HEIGHT][BOARD_WIDTH],x,y;
    segment *tmp;
    
    /* ZERUJE PLANSZE */
    for (i=0;i<BOARD_HEIGHT;i++) {
        for (j=0;j<BOARD_WIDTH;j++) {
            board[i][j]=0;
        }
    }
    /* RYSUJE WEZA */
    tmp=start;
    while (tmp->next!=NULL) {
          board[tmp->y][tmp->x]=1;
          tmp=tmp->next;
    }
    board[tmp->y][tmp->x]=1;
    
    /* RYSUJE WISIENKE */
    wisienki(board); /* JESLI NIE MA WISIENKI TO DODAJE NOWA */
    if (addPlus->exist==1) {
       board[addPlus->y][addPlus->x]=2;
    }
    
    /* RYSUJE GORE PLANSZY START */
    printf("/");
    for (i=0;i<(BOARD_WIDTH);i++) {
        printf("-");
    }
    printf("\\\n");
    /* RYSUJE GORE PLANSZY STOP */
    
    for (i=0;i<BOARD_HEIGHT;i++) {
        printf("|"); /* RYSUJE LEWY BOK PLANSZY */
        for (j=0;j<BOARD_WIDTH;j++) {
            switch (board[i][j]) {
                case 1:
                    printf("*");
                    break;
                case 2:
                    printf("#");
                    break;
                default:
                    printf(" ");
                    break;
            }
        }
        printf("|\n"); /* RYSUJE PRAWY BOK PLANSZY */
    }
    
    /* RYSUJE DOL PLANSZY START */
    printf("\\");
    for (i=0;i<BOARD_WIDTH;i++) {
        printf("-");
    }
    printf("/");
    /* RYSUJE DOL PLANSZY STOP */
    
    printf("\nZebrano wisienek: %d",points);
}

/* DODAWANIE WISIENEK */
void wisienki (int tab[BOARD_HEIGHT][BOARD_WIDTH]) {
     int x,y;
     
     if (addPlus->exist==0) {
         x=rand()%BOARD_WIDTH;
         y=rand()%BOARD_HEIGHT;
         
         while (tab[y][x]!=0) {
             x=rand()%BOARD_WIDTH;
             y=rand()%BOARD_HEIGHT;
         }
         
         addPlus->x=x;
         addPlus->y=y;
         addPlus->exist=1;
     }
     
}

/* PORUSZANIE WEZA */
int moveSnake (segment *glowa) {
     segment *tmp, *tmpNext;
     int tmpX1, tmpX2, tmpY1, tmpY2,control;
     
     tmp=glowa;
     
     tmpX1=tmp->x;
     tmpY1=tmp->y;
     
     switch (dir) {
            case 1:
                 tmp->x-=1;
                 break;
            case 2:
                 tmp->y+=1;
                 break;
            case 3:
                 tmp->x+=1;
                 break;
            case 5:
                 tmp->y-=1;
                 break;
     }
     control=kolizje(tmp);
     if (control==1) {
         return 1;
     }
     if (control==2) {
        points+=1;
        addPlus->exist=0;
        addElement(tmp);
     }
     tmp=tmp->next;
     
     while (tmp->next!=NULL) {
           tmpX2=tmp->x;
           tmpY2=tmp->y;
           tmp->x=tmpX1;
           tmp->y=tmpY1;
           tmpX1=tmpX2;
           tmpY1=tmpY2;
           
           tmp=tmp->next;
     }
     tmpX2=tmp->x;
     tmpY2=tmp->y;
     tmp->x=tmpX1;
     tmp->y=tmpY1;
     tmpX1=tmpX2;
     tmpY1=tmpY2;
     return 0;
}

void sterowanie () {
     int przycisk;
     if (kbhit()) {
        przycisk=getch();
        switch (przycisk) {
            case 119:
                if(dir!=2) {
                    dir=5;
                }
                break;
            case 97:
                if(dir!=3) {
                    dir=1;
                }
                break;
            case 115:
                if(dir!=5) {
                    dir=2;
                }
                break;
            case 100:
                if(dir!=1) {
                    dir=3;
                }
                break;
             }
    }
}
int kolizje (segment *waz) {
    segment *tmp;
    
    tmp=waz->next->next->next;
    
    if (waz->x<0 || waz->x>(BOARD_WIDTH-1) || waz->y<0 || waz->y>(BOARD_HEIGHT-1)) {
        return 1;
    }
    if (waz->x==addPlus->x && waz->y==addPlus->y) {
        return 2;
    }
    while (tmp->next!=NULL) {
        if (waz->x==tmp->x && waz->y==tmp->y) {
            return 1;
        }
        tmp=tmp->next;
    }
    if (waz->x==tmp->x && waz->y==tmp->y) {
        return 1;
    }
    return 0;
}

void delay (int timeDel) { /* PAUZA */
    sleep(timeDel);
    sterowanie ();
}
void destroySnake (segment *snake) {
     segment *tmp,*trash;
     
     tmp=snake;
     while (snake->next!=NULL) {
         trash=tmp->next;
         tmp->next=NULL;
         free(trash);
     }
     free(snake);
}
