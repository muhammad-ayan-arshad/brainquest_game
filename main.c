#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// I did this part with Ai cuz gui isnt in our course so apart from c0loring whole logic is ours
#ifdef _WIN32
#include <windows.h>
#endif

#define C_RST   "\033[0m"
#define C_CYAN  "\033[1;36m"
#define C_GOLD  "\033[1;33m"
#define C_WHITE "\033[1;37m"
#define C_LGRN  "\033[1;32m"
#define C_RED   "\033[1;31m"
#define C_MAG   "\033[1;35m"
#define C_BLUE  "\033[1;34m"

struct User {
    int id;
    char name[50];
    char pass[50];
    int coins;
    int hints;
    int prog[3][5];                 
    char curr[50];
    char hidden[50];
    int wrongs;
    int guesses[26];
};

struct User users[100];                 
int total=0;
int i,j;
int currUser=-1; 

char words[3][5][50] = {
    {"ZEBRA", "PIXEL", "WRITTER", "WITCH", "FLAME"},
    {"GALAXY", "JUNGLE", "PLANET", "CASTLE", "SPIDER"},
    {"ADVENTURE", "NIGHTMARE", "DARKLIGHT", "STARLIGHT", "DREAMLAND"}
};

void initcolor() { // This fun is also a part we did with ai and is only for gui and styling 
    #ifdef _WIN32
        HANDLE hOut=GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode=0;
        GetConsoleMode(hOut,&dwMode);
        dwMode|=0x0004;
        SetConsoleMode(hOut,dwMode);
    #endif
}

void cls() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void save() {
    FILE *fp=fopen("players.txt","w");
    if(fp==NULL)return;

    for(i=0; i<total; i++) {
        fprintf(fp,"%d %s %s %d %d",users[i].id,users[i].name,users[i].pass,users[i].coins,users[i].hints);
        for(j=0; j<15; j++) {
            fprintf(fp," %d",users[i].prog[j/5][j%5]);
        }
        fprintf(fp,"\n");
    }
    fclose(fp);
}

void load() {
    FILE *fp=fopen("players.txt","r");
    if(fp==NULL)return; 

    total=0;
    while(fscanf(fp,"%d %s %s %d %d",&users[total].id,users[total].name,users[total].pass,&users[total].coins,&users[total].hints)==5) {
        for(j=0; j<15; j++) {
            fscanf(fp,"%d",&users[total].prog[j/5][j%5]);
        }
        total++;
        if(total>=100)break;
    }
    fclose(fp);
}

void header(const char *title) {
    cls();
    printf(C_BLUE "\n  ======================================================\n" C_RST);
    printf(C_CYAN "    [ %s ]\n" C_RST,title);
    printf(C_BLUE "  ======================================================\n" C_RST);
}

void registration() {
    header("PLAYER REGISTRATION :: NEW PROFILE");
    
    if(total<100) {
       users[total].id=total+1;
       printf(C_LGRN "   Enter Player Name : " C_CYAN);
       scanf(" %49[^\n]",users[total].name);
       printf(C_LGRN "   Enter Secret Pass : " C_WHITE);
       scanf("%49s",users[total].pass);
       
       users[total].coins=100;
       users[total].hints=5;
       
       for(i=0; i<3; i++) {
            for(j=0; j<5; j++) {
                users[total].prog[i][j]=0;
            }
       }
       
       total++;
       save();
       printf(C_BLUE "\n  ------------------------------------------------------\n" C_RST);
       printf(C_LGRN "   >>> Profile Created! Your Player ID is: " C_WHITE "%d" C_LGRN " <<<\n" C_RST,users[total-1].id);
       printf(C_LGRN "   >>> Starter Pack: " C_GOLD "100 Coins" C_LGRN " & " C_WHITE "5 Hints" C_LGRN " awarded! <<<\n" C_RST);
       printf(C_BLUE "  ------------------------------------------------------\n" C_RST);
    }
    else {
        printf(C_RED "\n   [!] Server Player limit reached!\n" C_RST);
    }
    printf(C_LGRN "\n   Press Enter to continue..." C_RST);
    while(getchar()!='\n');
    getchar();
}

int login() {
    char name[50],pass[50];
    header("PLAYER AUTHENTICATION :: LOGIN");

    printf(C_LGRN "   Enter Player Name : " C_CYAN);
    scanf(" %49[^\n]",name);
    printf(C_LGRN "   Enter Secret Pass : " C_WHITE);
    scanf("%49s",pass);

    for(i=0; i<total; i++) {
        if(strcmp(users[i].name,name)==0&&strcmp(users[i].pass,pass)==0) {
            printf(C_BLUE "\n  ------------------------------------------------------\n" C_RST);
            printf(C_LGRN "   >>> Welcome back, " C_CYAN "%s" C_LGRN " (ID: " C_WHITE "%d" C_LGRN ")! <<<\n" C_RST,users[i].name,users[i].id);
            printf(C_BLUE "  ------------------------------------------------------\n" C_RST);
            printf(C_LGRN "\n   Press Enter to enter the Game Lobby..." C_RST);
            while(getchar()!='\n');
            getchar();
            return i; 
        }
    }

    printf(C_RED "\n   [!] Access Denied: Wrong name or password!\n" C_RST);
    printf(C_LGRN "   Press Enter to continue..." C_RST);
    while(getchar()!='\n');
    getchar();
    return -1; 
}

void shop() {
    int choice;
    do {
        header("ROYAL MARKETPLACE :: ITEM EXCHANGE");
        printf(C_LGRN "   Player: " C_CYAN "%s" C_LGRN " (ID: " C_WHITE "%d" C_LGRN ") | Coins: " C_GOLD "%d" C_LGRN " | Hints: " C_WHITE "%d\n" C_RST,users[currUser].name,users[currUser].id,users[currUser].coins,users[currUser].hints);
        printf(C_BLUE "  ------------------------------------------------------\n" C_RST);
        printf(C_LGRN "   Exchange Rate: " C_GOLD "20 Coins" C_LGRN " = " C_WHITE "+1 Hint\n" C_RST);
        printf(C_BLUE "  ------------------------------------------------------\n" C_RST);
        printf(C_WHITE "   [1] " C_LGRN "Buy " C_WHITE "1" C_LGRN " Hint (" C_GOLD "20 Coins" C_LGRN ")\n" C_RST);
        printf(C_WHITE "   [2] " C_LGRN "Return to Game Lobby\n" C_RST);
        printf(C_BLUE "  ======================================================\n" C_RST);
        printf(C_LGRN "   Select Option: " C_WHITE);

        if(scanf("%d",&choice)!=1) {
            while(getchar()!='\n');
            continue;
        }
        while(getchar()!='\n');

        if(choice==1) {
            if(users[currUser].coins>=20) {
                users[currUser].coins-=20;
                users[currUser].hints+=1;
                printf(C_LGRN "\n   >>> Purchased " C_WHITE "1" C_LGRN " Hint! (" C_GOLD "-20 Coins" C_LGRN ") <<<\n" C_RST);
            } else {
                printf(C_RED "\n   [!] Insufficient Funds! Need " C_GOLD "20 Coins" C_RED ".\n" C_RST);
            }
            printf(C_LGRN "   Press Enter to continue..." C_RST);
            getchar();
        } else if(choice==2) {
            return;
        } else {
            printf(C_RED "\n   [!] Invalid option! Press Enter..." C_RST);
            getchar();
        }
    } while(choice!=2);
}

void ruleskcs() {
    header("KCS ARCHIVES :: RULES & MECHANICS");
    printf(C_CYAN "   1. THE DECK SETUP:\n" C_RST);
    printf(C_LGRN "      - Each side receives " C_WHITE "5" C_LGRN " cards: " C_WHITE "4" C_LGRN " Citizens & " C_WHITE "1" C_LGRN " Special.\n" C_RST);
    printf(C_LGRN "      - " C_GOLD "KING Side" C_LGRN " vs " C_GOLD "SLAVE Side" C_LGRN " assigned at random.\n\n" C_RST);
    printf(C_CYAN "   2. CLASH HIERARCHY:\n" C_RST);
    printf(C_GOLD "      - KING" C_LGRN " defeats " C_WHITE "CITIZEN\n" C_RST);
    printf(C_WHITE "      - CITIZEN" C_LGRN " defeats " C_GOLD "SLAVE\n" C_RST);
    printf(C_GOLD "      - SLAVE" C_LGRN " assassinates " C_GOLD "KING\n" C_RST);
    printf(C_LGRN "      - Citizen vs Citizen: Tie! Both cards discarded.\n\n" C_RST);
    printf(C_CYAN "   3. TURNS & SURRENDER:\n" C_RST);
    printf(C_LGRN "      - Match spans up to " C_WHITE "5" C_LGRN " turns. Used slots lock.\n" C_RST);
    printf(C_LGRN "      - Enter " C_WHITE "0" C_LGRN " during your turn to " C_RED "Surrender" C_LGRN ".\n\n" C_RST);
    printf(C_CYAN "   4. STAKES & PAYOUTS:\n" C_RST);
    printf(C_LGRN "      - Victory:   " C_GOLD "+8 Coins\n" C_RST);
    printf(C_LGRN "      - Defeat:    " C_RED "-5 Coins\n" C_RST);
    printf(C_LGRN "      - Surrender: " C_RED "-7 Coins\n" C_RST);
    printf(C_BLUE "  ======================================================\n" C_RST);
    printf(C_LGRN "   Press Enter to return..." C_RST);
    getchar();
}

void playkcs() {
    int pHand[5];
    int cHand[5];
    int pUsed[5]={0,0,0,0,0};
    int cUsed[5]={0,0,0,0,0};
    int pSide=rand()%2;

    for(i=0; i<5; i++) {
       pHand[i]=1;
       cHand[i]=1;
    }

    int pSpecial=rand()%5;
    int cSpecial=rand()%5;

    if(pSide==0) {
        pHand[pSpecial]=2;
        cHand[cSpecial]=3;
    }
    else {
       pHand[pSpecial]=3;
       cHand[cSpecial]=2;
    }

    header("KCS ARENA :: DEALING HANDS");
    if(pSide==0) {
        printf(C_LGRN "   >>> You hold the " C_GOLD "KING SIDE" C_LGRN " (" C_GOLD "1 King" C_LGRN ", " C_WHITE "4 Citizens" C_LGRN ")\n" C_RST);
        printf(C_LGRN "   >>> Computer holds the " C_GOLD "SLAVE SIDE" C_LGRN " (" C_GOLD "1 Slave" C_LGRN ", " C_WHITE "4 Citizens" C_LGRN ")\n" C_RST);
    } else {
        printf(C_LGRN "   >>> You hold the " C_GOLD "SLAVE SIDE" C_LGRN " (" C_GOLD "1 Slave" C_LGRN ", " C_WHITE "4 Citizens" C_LGRN ")\n" C_RST);
        printf(C_LGRN "   >>> Computer holds the " C_GOLD "KING SIDE" C_LGRN " (" C_GOLD "1 King" C_LGRN ", " C_WHITE "4 Citizens" C_LGRN ")\n" C_RST);
    }
    printf(C_BLUE "  ======================================================\n" C_RST);
    printf(C_LGRN "   Press Enter to enter Round " C_WHITE "1" C_LGRN "..." C_RST);
    getchar();

    int turn,choice,pCard,cSlot,cCard;

    for(turn=1; turn<=5; turn++) {
       cls();
       printf(C_BLUE "\n  ======================================================\n" C_RST);
       printf(C_CYAN "    [ KCS ARENA :: ROUND " C_WHITE "%d" C_CYAN " OF " C_WHITE "5" C_CYAN " ]\n" C_RST,turn);
       printf(C_BLUE "  ======================================================\n" C_RST);
       printf(C_LGRN "   Player: " C_CYAN "%s" C_LGRN " | Coins: " C_GOLD "%d" C_LGRN " | Role: " C_GOLD "%s\n" C_RST,users[currUser].name,users[currUser].coins,(pSide==0?"KING":"SLAVE"));
       printf(C_BLUE "  ------------------------------------------------------\n" C_RST);

       printf(C_CYAN "\n   YOUR HAND:\n" C_RST);
       for(i=0; i<5; i++) {
           if(pUsed[i]==0) {
               if(pHand[i]==1)printf(C_WHITE "    Slot [%d] : " C_LGRN "[ Citizen ]\n" C_RST,i+1);
               else if(pHand[i]==2)printf(C_WHITE "    Slot [%d] : " C_GOLD "[ *** KING *** ]\n" C_RST,i+1);
               else if(pHand[i]==3)printf(C_WHITE "    Slot [%d] : " C_GOLD "[ *** SLAVE *** ]\n" C_RST,i+1);
           }
           else {
               printf(C_WHITE "    Slot [%d] : " C_RED "[ Discarded ]\n" C_RST,i+1);
           }
       }
       printf(C_BLUE "  ------------------------------------------------------\n" C_RST);

       while(1) {
           printf(C_LGRN "   Select Card Slot (" C_WHITE "1-5" C_LGRN ", or " C_WHITE "0" C_LGRN " to Surrender): " C_WHITE);
           if(scanf("%d",&choice)!=1) {
               while(getchar()!='\n');
               printf(C_RED "   [!] Invalid input! Enter a number (" C_WHITE "0-5" C_RED ").\n" C_RST);
               continue;
           }
           while(getchar()!='\n');

           if(choice==0) {
               header("KCS ARENA :: SURRENDERED");
               printf(C_LGRN "   You forfeited the match.\n" C_RST);
               printf(C_RED "   >>> DEFEAT! Penalty applied: " C_GOLD "-7 Coins" C_RED " <<<\n" C_RST);
               users[currUser].coins-=7;
               if(users[currUser].coins<0) {
                   users[currUser].coins=0;
               }
               printf(C_BLUE "  ======================================================\n" C_RST);
               printf(C_LGRN "   Press Enter to return..." C_RST);
               getchar();
               return;
           }

           if(choice<1||choice>5) {
               printf(C_RED "   [!] Out of bounds! Pick slot " C_WHITE "1-5" C_RED " (or " C_WHITE "0" C_RED ").\n" C_RST);
           } 
           else if(pUsed[choice-1]==1) {
               printf(C_RED "   [!] Slot " C_WHITE "%d" C_RED " already played! Choose another.\n" C_RST,choice);
           } 
           else {
               break;
           }
       }

       pCard=pHand[choice-1];
       pUsed[choice-1]=1;

       do {
           cSlot=rand()%5;
       } while(cUsed[cSlot]==1);

       cCard=cHand[cSlot];
       cUsed[cSlot]=1;

       printf(C_BLUE "\n  ------------------------------------------------------\n" C_RST);
       printf(C_CYAN "    ROUND " C_WHITE "%d" C_CYAN " CLASH REVEAL\n" C_RST,turn);
       printf(C_BLUE "  ------------------------------------------------------\n" C_RST);
       
       printf(C_LGRN "   You played      : " C_RST);
       if(pCard==1)printf(C_LGRN "[ Citizen ]\n" C_RST);
       else if(pCard==2)printf(C_GOLD "[ KING ]\n" C_RST);
       else if(pCard==3)printf(C_GOLD "[ SLAVE ]\n" C_RST);

       printf(C_LGRN "   Computer played : " C_RST);
       if(cCard==1)printf(C_LGRN "[ Citizen ]\n" C_RST);
       else if(cCard==2)printf(C_GOLD "[ KING ]\n" C_RST);
       else if(cCard==3)printf(C_GOLD "[ SLAVE ]\n" C_RST);
       printf(C_BLUE "  ------------------------------------------------------\n" C_RST);

       if(pCard==1&&cCard==1) {
           printf(C_LGRN "   Result: Both played Citizen! Tie card discarded.\n" C_RST);
           printf(C_LGRN "   Press Enter for next round..." C_RST);
           getchar();
       }
       else if((pCard==2&&cCard==1)||(pCard==3&&cCard==2)||(pCard==1&&cCard==3)) {
           printf(C_LGRN "\n   >>> CLASH VICTORY! You won " C_GOLD "+8 Coins" C_LGRN "! <<<\n" C_RST);
           users[currUser].coins+=8;
           printf(C_LGRN "   Press Enter to return..." C_RST);
           getchar();
           return;
       }
       else if((cCard==2&&pCard==1)||(cCard==3&&pCard==2)||(cCard==1&&pCard==3)) {
           printf(C_RED "\n   >>> CLASH DEFEAT! You lost " C_GOLD "-5 Coins" C_RED "! <<<\n" C_RST);
           users[currUser].coins-=5;
           if(users[currUser].coins<0) {
               users[currUser].coins=0;
           }
           printf(C_LGRN "   Press Enter to return..." C_RST);
           getchar();
           return;
       }
    }
}

void kcs() {
    int choice;
    do {
        header("KING - CITIZEN - SLAVE :: GAME PAGE");
        printf(C_LGRN "   Player: " C_CYAN "%s" C_LGRN " | Coins: " C_GOLD "%d\n" C_RST,users[currUser].name,users[currUser].coins);
        printf(C_BLUE "  ------------------------------------------------------\n" C_RST);
        printf(C_WHITE "   [1] " C_LGRN "Enter Arena (vs Computer)\n" C_RST);
        printf(C_WHITE "   [2] " C_LGRN "View Game Rules\n" C_RST);
        printf(C_WHITE "   [3] " C_LGRN "Back to Game Lobby\n" C_RST);
        printf(C_BLUE "  ======================================================\n" C_RST);
        printf(C_LGRN "   Select Option: " C_WHITE);

        if(scanf("%d",&choice)!=1) {
            while(getchar()!='\n');
            continue;
        }
        while(getchar()!='\n');

        switch(choice) {
            case 1:
                playkcs();
                break;
            case 2:
                ruleskcs();
                break;
            case 3:
                return;
            default:
                printf(C_RED "\n   [!] Invalid option! Press Enter..." C_RST);
                getchar();
        }
    } while(choice!=3);
}

void draw(int wrongs) {
    printf(C_BLUE "\n");
    printf("     +-------+     \n");
    printf("     |       |     \n");
    printf("     |      " C_RED "%c%c%c" C_BLUE "   \n",(wrongs>=1?'(':' '),(wrongs>=1?'_':' '),(wrongs>=1?')':' '));
    printf("     |      " C_RED "%c%c%c" C_BLUE "   \n",(wrongs>=3?'/':' '),(wrongs>=2?'|':' '),(wrongs>=4?'\\':' '));
    printf("     |      " C_RED "%c %c" C_BLUE "   \n",(wrongs>=5?'/':' '),(wrongs>=6?'\\':' '));
    printf("     |             \n");
    printf("   ==+==           \n" C_RST);
}

void play(int diff, int lvl) {
    int len,randIdx,revealed,idx,found;
    char freeChar,guess,ch;

    strcpy(users[currUser].curr,words[diff][lvl]);
    len=strlen(users[currUser].curr);
    
    for(i=0; i<len; i++)users[currUser].hidden[i]='_';
    users[currUser].hidden[len]='\0';
    
    randIdx=rand()%len;
    freeChar=users[currUser].curr[randIdx];
    for(i=0; i<len; i++) {
        if(users[currUser].curr[i]==freeChar) {
            users[currUser].hidden[i]=freeChar;
        }
    }
    users[currUser].guesses[freeChar-'A']=1;

    users[currUser].wrongs=0;
    for(i=0; i<26; i++) {
        if(i!=(freeChar-'A')) {
            users[currUser].guesses[i]=0;
        }
    }

    while(users[currUser].wrongs<6) {
        cls();
        printf(C_BLUE "\n  ======================================================\n" C_RST);
        printf(C_CYAN "    [ HANGMAN ARENA :: DIFF " C_WHITE "%d" C_CYAN " - LEVEL " C_WHITE "%d" C_CYAN " ]\n" C_RST,diff+1,lvl+1);
        printf(C_BLUE "  ======================================================\n" C_RST);
        printf(C_LGRN "   Player: " C_CYAN "%s" C_LGRN " | Hints: " C_WHITE "%d" C_LGRN " | Wrongs: " C_RED "%d" C_LGRN "/" C_WHITE "6\n" C_RST,users[currUser].name,users[currUser].hints,users[currUser].wrongs);
        printf(C_BLUE "  ------------------------------------------------------\n" C_RST);

        draw(users[currUser].wrongs);

        printf(C_CYAN "\n   SECRET WORD: " C_GOLD);
        for(i=0; i<len; i++)printf("%c ",users[currUser].hidden[i]);
        printf("\n" C_RST);
        printf(C_BLUE "  ------------------------------------------------------\n" C_RST);

        if(strcmp(users[currUser].curr,users[currUser].hidden)==0) {
            if(users[currUser].prog[diff][lvl]==0) {
                printf(C_LGRN "\n   >>> WORD SOLVED! First Clear Bonus: " C_GOLD "+10 Coins" C_LGRN " <<<\n" C_RST);
                users[currUser].coins+=10;
            } else {
                printf(C_LGRN "\n   >>> WORD SOLVED! Replay Bonus: " C_GOLD "+1 Coin" C_LGRN " <<<\n" C_RST);
                users[currUser].coins+=1;
            }
            
            users[currUser].prog[diff][lvl]=1; 
            printf(C_LGRN "   Press Enter to return..." C_RST);
            while(getchar()!='\n');
            getchar();
            return;
        }

        printf(C_LGRN "   Guess Letter (or '" C_WHITE "0" C_LGRN "' for Hint, '" C_WHITE "Q" C_LGRN "' to Quit): " C_WHITE);
        if(scanf(" %c",&guess)!=1) {
            while(getchar()!='\n');
            continue;
        }
        while(getchar()!='\n');

        if(guess=='Q'||guess=='q')return;

        /* Flattened student-style hint handler avoiding deep nesting */
        if(guess=='0') {
            if(users[currUser].hints<=0) {
                printf(C_RED "\n   [!] Out of Hints! Visit Marketplace to buy more.\n" C_RST);
            } else {
                revealed=0;
                i=0;
                while(i<len&&revealed==0) {
                    if(users[currUser].hidden[i]=='_') {
                        ch=users[currUser].curr[i];
                        users[currUser].hidden[i]=ch;
                        users[currUser].guesses[ch-'A']=1;
                        users[currUser].hints--;
                        revealed=1;
                        printf(C_LGRN "\n   >>> Hint Used! Revealed Letter: " C_GOLD "%c" C_LGRN " <<<\n" C_RST,ch);
                    }
                    i++;
                }
            }
            printf(C_LGRN "   Press Enter..." C_RST);
            getchar();
            continue;
        }

        if(guess>='a'&&guess<='z')guess-=32;

        idx=guess-'A';
        if(idx<0||idx>25) {
            printf(C_RED "   [!] Invalid character! Press Enter..." C_RST);
            getchar();
            continue;
        }
        if(users[currUser].guesses[idx]==1) {
            printf(C_RED "   [!] Letter already guessed! Press Enter..." C_RST);
            getchar();
            continue;
        }
        users[currUser].guesses[idx]=1;

        found=0;
        for(i=0; i<len; i++) {
            if(users[currUser].curr[i]==guess) {
                users[currUser].hidden[i]=guess;
                found=1;
            }
        }

        if(!found)users[currUser].wrongs++;
    }

    header("HANGMAN ARENA :: GAME OVER");
    draw(users[currUser].wrongs);
    printf(C_RED "\n   [!] Out of guesses! The secret word was: " C_GOLD "%s\n" C_RST,users[currUser].curr);
    printf(C_BLUE "  ======================================================\n" C_RST);
    printf(C_LGRN "   Press Enter to return..." C_RST);
    getchar();
}

void levels(int diff) {
    int choice;
    do {
        cls();
        printf(C_BLUE "\n  ======================================================\n" C_RST);
        printf(C_CYAN "    [ HANGMAN :: DIFFICULTY " C_WHITE "%d" C_CYAN " STAGE SELECT ]\n" C_RST,diff+1);
        printf(C_BLUE "  ======================================================\n" C_RST);
        printf(C_LGRN "   Choose a Level (" C_WHITE "1-5" C_LGRN "):\n\n" C_RST);
        
        for(i=0; i<5; i++) {
            if(users[currUser].prog[diff][i]==1) {
                printf(C_WHITE "   [%d] " C_LGRN "Level " C_WHITE "%d  " C_GOLD "[ COMPLETED ]\n" C_RST,i+1,i+1);
            } else {
                printf(C_WHITE "   [%d] " C_LGRN "Level " C_WHITE "%d  " C_CYAN "[ OPEN ]\n" C_RST,i+1,i+1);
            }
        }
        printf(C_WHITE "\n   [6] " C_LGRN "Back to Difficulty Menu\n" C_RST);
        printf(C_BLUE "  ======================================================\n" C_RST);
        printf(C_LGRN "   Select Option: " C_WHITE);

        if(scanf("%d",&choice)!=1) {
            while(getchar()!='\n');
            continue;
        }
        while(getchar()!='\n');

        if(choice>=1&&choice<=5) {
            play(diff,choice-1);
        } else if(choice==6) {
            return;
        } else {
            printf(C_RED "   [!] Invalid option! Press Enter..." C_RST);
            getchar();
        }
    } while(choice!=6);
}

void hangman() {
    int choice;
    do {
        header("HANGMAN :: DIFFICULTY SELECTION");
        printf(C_LGRN "   Player: " C_CYAN "%s" C_LGRN " | Coins: " C_GOLD "%d" C_LGRN " | Hints: " C_WHITE "%d\n" C_RST,users[currUser].name,users[currUser].coins,users[currUser].hints);
        printf(C_BLUE "  ------------------------------------------------------\n" C_RST);
        printf(C_WHITE "   [1] " C_LGRN "Easy Mode   (5-6 Letter Words)\n" C_RST);
        printf(C_WHITE "   [2] " C_LGRN "Medium Mode (6-7 Letter Words)\n" C_RST);
        printf(C_WHITE "   [3] " C_LGRN "Hard Mode   (6-11 Letter Words)\n" C_RST);
        printf(C_WHITE "   [4] " C_LGRN "Back to Game Lobby\n" C_RST);
        printf(C_BLUE "  ======================================================\n" C_RST);
        printf(C_LGRN "   Select Option: " C_WHITE);

        if(scanf("%d",&choice)!=1) {
            while(getchar()!='\n');
            continue;
        }
        while(getchar()!='\n');

        switch(choice) {
            case 1: levels(0); break;
            case 2: levels(1); break;
            case 3: levels(2); break;
            case 4: return;
            default:
                printf(C_RED "   [!] Invalid option! Press Enter..." C_RST);
                getchar();
        }
    } while(choice!=4);
}

void games() {
    int choice;
    do {
        header("BRAINQUEST :: MAIN GAME LOBBY");
        printf(C_LGRN "   Player: " C_CYAN "%s" C_LGRN " (ID: " C_WHITE "%d" C_LGRN ") | Coins: " C_GOLD "%d" C_LGRN " | Hints: " C_WHITE "%d\n" C_RST,users[currUser].name,users[currUser].id,users[currUser].coins,users[currUser].hints);
        printf(C_BLUE "  ------------------------------------------------------\n" C_RST);
        printf(C_WHITE "   [1] " C_LGRN "Royal Marketplace (Buy Hints)\n" C_RST);
        printf(C_WHITE "   [2] " C_LGRN "King - Citizen - Slave (KCS Arena)\n" C_RST);
        printf(C_WHITE "   [3] " C_LGRN "Hangman Word Challenge\n" C_RST);
        printf(C_WHITE "   [4] " C_LGRN "Save & Logout (Return to Portal)\n" C_RST);
        printf(C_BLUE "  ======================================================\n" C_RST);
        printf(C_LGRN "   Select Option: " C_WHITE);
        
        if(scanf("%d",&choice)!=1) {
            while(getchar()!='\n');
            continue;
        }
        while(getchar()!='\n');

        switch(choice) {
            case 1: 
                shop(); 
                break;
            case 2: 
                kcs();
                break;
            case 3: 
                hangman(); 
                break;
            case 4:
                save();
                printf(C_LGRN "\n   >>> Progress saved! Returning to title portal... <<<\n" C_RST);
                return; 
            default:
                printf(C_RED "\n   [!] Invalid option! Press Enter..." C_RST);
                getchar();
        }
    } while(choice!=4);
}

int main() {
    int choice;

    initcolor();
    srand((unsigned int)time(NULL)); 
    load(); 

    do {
        cls();
        printf(C_BLUE "\n");
        printf("  +--------------------------------------------------------+\n");
        printf("  |                                                        |\n");
        printf("  |       " C_GOLD " B R A I N Q U E S T   P O R T A L " C_BLUE "              |\n");
        printf("  |                                                        |\n");
        printf("  +--------------------------------------------------------+\n");
        printf("  |                                                        |\n");
        printf("  |        " C_WHITE "[1]" C_LGRN " CREATE NEW PLAYER PROFILE                   " C_BLUE "|\n");
        printf("  |        " C_WHITE "[2]" C_LGRN " LOGIN TO EXISTING PROFILE                   " C_BLUE "|\n");
        printf("  |        " C_WHITE "[3]" C_RED " EXIT PORTAL                                 " C_BLUE "|\n");
        printf("  |                                                        |\n");
        printf("  +--------------------------------------------------------+\n" C_RST);
        
        printf(C_LGRN "\n   Select Option: " C_WHITE);
        if(scanf("%d",&choice)!=1) {
            while(getchar()!='\n');
            continue;
        }
        while(getchar()!='\n');

        if(choice==1) {
            registration();
        }
        else if(choice==2) {
            currUser=login();
            if(currUser!=-1) {
                games();
            }
        }
        else if(choice==3) {
            save();
            printf(C_LGRN "\n   >>> All data saved! Thanks for playing BrainQuest! <<<\n\n" C_RST);
            break;
        }
        else {
            printf(C_RED "\n   [!] Invalid option! Press Enter..." C_RST);
            getchar();
        }
    } while(choice!=3);

    return 0;
}