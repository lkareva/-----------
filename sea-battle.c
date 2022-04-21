#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include <time.h> 

//�������� ��६���� � ����⠭�� 
struct Player{
    int field[10][10]; // ���� ��ப� � ���⠢����묨 �� ��� ��ࠡ�ﬨ
    int qt_ships[10]; // �㤥� ��᫥������ ���-�� ��ࠡ��� �� ���� � �� 楫��⭮���
    int ships; // ���-�� �� ��⮯������ ��ࠡ���  
};
struct H{ //�࠭�� ����� �� ���ਨ ����५�
    int p; //����� ��ப�
    int x; //���न��� �᫠
    int y; // ���न��� �㪢�
    int result; // १���� ����५�
};
struct H h[] = {{0, 0, 0 ,0},{0, 0, 0, 0},{0, 0 ,0 ,0},{0, 0, 0, 0},{0, 0, 0 ,0},{0, 0, 0, 0},{0, 0 ,0 ,0},{0, 0, 0, 0}, {0, 0 ,0 ,0},{0, 0, 0, 0}} ;
//��� ���ᠭ�� ��室�� ����५� ����� ����᫥���. 
const int Invalid = 1, // �� ������� ���⪨ � ������묨 ���न��⠬�; � ����� (x, y) 㦥 ��५﫨;
          Missed  = 2, // ���⪠ (x, y) ����: �஬�� 
          Hit     = 3, // ���⪠ (x, y) ����� ��ࠡ��: ������, �� �� 㡨�� 
          Killed  = 4; // ���⪠ (x, y) ����� ��ࠡ��: ������ � 㡨�� ��ࠡ�� 
const int  EmptyCell     =  -1, // ���⪠ ����� 
           MissedCell    =  -2, // 㦥 ��५﫨, �� ����५� �뫠 ���⮩ 
           DestroyedCell =  -3, // ������� ���㡠
           BanCell       =  -4; //�८� ��ࠡ�� 
const char EmptyChar     = '.',           
           MissedChar    = 'o',
           DestroyedChar = 'X';

//
int CoorX(int x){
    return x - 1;
}
int CoorY(char y){
    char coor_y[10] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};
    for(int i = 0; i < 10; i++){
        if(y == coor_y[i]) return i;
    }
    return -1;
}
int X(int x){
    return x + 1;
}
char Y(int y){
    return 97 + y;
}
char Cipher(int n){ //�८�ࠧ�� ���� � ᨬ����       
    if(n == EmptyCell || n == BanCell) return EmptyChar;
    if(n == MissedCell) return MissedChar;
    if(n == DestroyedCell) return DestroyedChar;
    return 254;
}
void dirs(int *x, int *y, int c){//�롮� ���ࠢ�����
    int a = *x, b = *y;
    if(c == 0) a--;
    if(c == 1) a++;
    if(c == 2) b++;
    if(c == 3) b--;
    *x = a;
    *y = b;
}
void History(int p, int x, int y, int result){//����� ��᫥���� 10 室��
    for(int i = 8; i >= 0; i--){
        h[i+1].p = h[i].p; 
        h[i+1].x = h[i].x;
        h[i+1].y = h[i].y; 
        h[i+1].result = h[i].result; 
    }
    h[0].p = p;
    h[0].x = x;
    h[0].y = y;
    h[0].result = result;
}
void InitPlayer(struct Player *player){// ���樠������ ����� �������� 
    int sh[10] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
    for(int i = 0; i < 10; i++){
        (*player).qt_ships[i] = sh[i];
        for(int j = 0; j < 10; j++){
            (*player).field[i][j] = EmptyCell;
        }
    }
    (*player).ships = 10;
}
int CheckExit(struct Player player, int x, int y){ // �㭪�� �஢�ન �� 㤠筮� ࠧ��饭�� ����� ����� (����, ���न���� �����)
    if( x < 0 || x > 9 ||y < 0 || y > 9) return -1; // ��室 �� �࠭��� 
    else if(player.field[x][y] == EmptyCell){
        return 0;// ����� ࠧ������ ��ࠡ��
    }
    else return -2; //� ���⪥ 㦥 ���� ���㡠 ��� ��ࠡ�� ᮯਪ������ 
}




//�㭪樨 ��� �ࠢ����� ������ன 
int PutHorizontal(struct Player *player, int ship, int x, int y){ //(����, ship, ���न���� ) ��� ship ������ ���ᨢ� player.qt_ships
    //ࠧ��頥� ��ࠡ�� ��ਧ��⠫쭮(������ ���न���� "������" � ࠧ��頥� ��⠫�� ���� ��ࠡ�� �ࠢ�� ��)
    int class_sh = (*player).qt_ships[ship];
    int check = 0;
    for(int i = 0; i < class_sh; i++){
        if(CheckExit(*player, x, y+i) == 0) check++;
    }
    if(check == class_sh){
        while(class_sh != 0){
            (*player).field[x][y] = ship; 
            class_sh--;
            for(int i = x-1; i <= x+1; i++){
                for(int j = y-1; j <= y+1; j++){
                    if(class_sh != 0 && j == y+1){}
                    else if((*player).field[i][j] == EmptyCell && (i != -1) && (j != -1)&& (i != 10) && (j != 10))  (*player).field[i][j] = BanCell;
                }
            }
            y++;

        }
    }
    else return -1;
    return 0;
}
int PutVertical(struct Player *player, int ship, int x, int y){ //(����, ship, ���न���� ) ��� ship ������ ���ᨢ� player.qt_ships
    //ࠧ��頥� ��ࠡ�� ���⨪��쭮(������ ���न���� "������" � ࠧ��頥� ��⠫�� ���� ��ࠡ�� ���� ��)
    int class_sh = (*player).qt_ships[ship];
    int check = 0;
    for(int i = 0; i < class_sh; i++){
        if(CheckExit(*player, x+i, y) == 0) check++;
    }
    if(check == class_sh){
        while(class_sh != 0){
            (*player).field[x][y] = ship;
            class_sh--;
            for(int i = x-1; i <= x+1; i++){
                for(int j = y-1; j <= y+1; j++){
                    if(class_sh != 0 && i == x+1){}
                    else if((*player).field[i][j] == EmptyCell  && (i != -1) && (j != -1) && (i != 10) && (j != 10)) (*player).field[i][j] = BanCell;
                }
            }
            x++;
        }
    }
    else return -1;
    return 0;
}

void AreaS(struct Player *player, int x, int y){ //�८� ��ࠡ�� ��᫥ ��⮯����� ��ࠡ��
    int  c = 0;
    int a, b;
    while(c != 4){
        a = x;
        b = y;
        while((*player).field[a][b] == DestroyedCell){
            for(int i = a-1; i <= a+1; i++){
                for(int j = b-1; j <= b+1; j++){
                    if((*player).field[i][j] == BanCell && (i != -1) && (j != -1) && (i != 10) && (j != 10)) (*player).field[i][j] = MissedCell;
                }
            }
            dirs( &a, &b, c);
        }
        c++;
    }
}
int Shoot (struct Player *player, int x, int y){ // ����५��� � ����� x, y �� ���� player.field[][]
    int ship;
    if( x < 0 || x > 9 ||y < 0 || y > 9) return Invalid; //��室 �� �࠭��� ����
    else if((*player).field[x][y] == MissedCell || (*player).field[x][y] == DestroyedCell) return Invalid; // � ��� ��� 㦥 ��������
    else if((*player).field[x][y] == EmptyCell || (*player).field[x][y] == BanCell){
            (*player).field[x][y] = MissedCell;
            return Missed;
    }
    else{
        ship = (*player).field[x][y];
        (*player).field[x][y] = DestroyedCell;
        (*player).qt_ships[ship]--;
        if((*player).qt_ships[ship] == 0){
            (*player).ships--;
            AreaS(player,x,y);
            return Killed;
        }
        return Hit;
    }
}






//�����
// �뢮� ������ ���� ��� ���⠭���� ��ࠡ��� 
void Draw1(struct Player player, int ship, int p, int PutS){
    printf("      P L A Y E R %d\n", p);
    printf("   a b c d e f g h i j\n");
    for(int i = 0; i < 10; i++){
        printf("%2d ",i + 1);
        for(int j = 0; j < 10; j++){
            printf("%c ", Cipher(player.field[i][j]));
        }
        printf("%2d ",i + 1);
        if(PutS == 1){
        if(i == 0)      printf("    ��ࠡ��: \n");
        else if(i == 1) printf("    1   ������ (\"4444\")\n");
        else if(i == 2) printf("    2 �३�� (\" 333\")\n");
        else if(i == 3) printf("    3  �ᬨ�� (\"  22\")\n");
        else if(i == 4) printf("    4   ���� (\"   1\")\n");
        else if(i == 5) printf("    ��⠫���: %d �� 10\n", 10 - ship );
        else if(i == 6) printf("    �ࠢ��� �����: ��������� ��஡�� ��।������ ���न��⠬�\n");
        else if(i == 7) printf("    �  �ਥ��樥� �����: 0 - ��ਧ��⠫쭮; 1 - ���⨪��쭮.\n");
        else if(i == 8) printf("    ��⠫�� ����� ���⠢������ �ࠢ�� ��� ���� ������ ᮮ⢥��⢥���.\n");
        else if(i == 9) printf("    ��� ���� �ਥ���� �� �����. �ਬ�� ����� ���न���: 1a\n");
        }
        else printf("\n");
    }
    printf("   a b c d e f g h i j\n");
    if(player.qt_ships[ship] == 4) printf("�㤥� ࠧ��饭 ������!\n");
    else if(player.qt_ships[ship] == 3) printf("�㤥� ࠧ��饭 �३��!\n");
    else if(player.qt_ships[ship] == 2) printf("�㤥� ࠧ��饭 �ᬨ���!\n");
    else if(player.qt_ships[ship] == 1) printf("�㤥� ࠧ��饭 ����!\n");

    

}
// �뢮� ����� �� �࠭
void Draw12(struct Player player1, int open1, struct Player player2, int open2, int p){ // open == 1, ���� player.field[][] �����뢠���� ����⮬ ०���, ���� � �����⮬
    printf("      P L A Y E R %d              P L A Y E R %d\n", p, 3 - p);
    printf("   a b c d e f g h i j        a b c d e f g h i j\n");
    for(int i = 0; i < 10; i++){
        printf("%2d ",i + 1);
        for(int j = 0; j < 10; j++){
            if(open1 == 1) printf("%c ", Cipher(player1.field[i][j]));
            else if( player1.field[i][j] == MissedCell || player1.field[i][j] == DestroyedCell ) printf("%c ", Cipher(player1.field[i][j]));
            else printf("%c ", EmptyChar);
        }
        printf("%2d  %2d ",i + 1, i + 1);
        for(int j = 0; j < 10; j++){
            if(open2 == 1) printf("%c ", Cipher(player2.field[i][j]));
            else if( player2.field[i][j] == MissedCell || player2.field[i][j] == DestroyedCell ) printf("%c ", Cipher(player2.field[i][j]));
            else printf("%c ", EmptyChar);
        }
        printf("%2d ",i + 1);
        if(i == 4)      printf("      PLAYER 1: %d ��ࠡ���\n", player1.ships);
        else if(i == 5) printf("      PLAYER 2: %d ��ࠡ���\n", player2.ships);
        else printf("\n");
    }
    printf("   a b c d e f g h i j        a b c d e f g h i j          �ਬ�� ����� ���न���: 1a\n");
    printf("____________________________________________________\n");
    for(int i = 9; i >= 0; i--){
        if(h[i].result == 0);
        if(h[i].result == Missed) printf("PLAYER %d: (%2d%c) - ������!\n", h[i].p, X(h[i].x), Y(h[i].y));
        if(h[i].result == Hit) printf("PLAYER %d: (%2d%c) - ������� �����!\n", h[i].p, X(h[i].x), Y(h[i].y));
        if(h[i].result == Killed) printf("PLAYER %d: (%2d%c) - ������� ����!\n", h[i].p, X(h[i].x), Y(h[i].y));
    }
    printf("____________________________________________________\n");
}





//�����饭�� ��ࠡ��� 
void UserPutShips(struct Player *player, int p){//����⠭���� ��ࠡ��� � �����
    int ship = 0, x, z;
    char y;
    while(ship < 10){
        fflush(stdin);
        Draw1(*player, ship, p, 1);
        printf("������ ���न����:");
        if(scanf("%d%c", &x, &y) == 2){
            printf("������ �ਥ����:");
            fflush(stdin);
            if(scanf("%d", &z) != 1){
                fflush(stdin);
                printf("�����४⭮ ������� �ਥ���� ��஡��! ���஡�� �� ࠧ!\n");
                system("pause");
                system("cls");
            }
            else{
                if( z == 0){
                    if(PutHorizontal(player, ship, CoorX(x), CoorY(y)) == 0) {
                        ship++;
                        system("cls");
                    }
                    else {
                        printf("��㤠筠� ����⪠ ���⠢��� ��ࠡ��! ���஡�� �� ࠧ!\n");
                        system("pause");
                        system("cls");
                    }
                }
                else if( z == 1){
                    if(PutVertical(player, ship, CoorX(x), CoorY(y)) == 0) {
                        ship++;
                        system("cls");
                    }
                    else {
                        printf("��㤠筠� ����⪠ ���⠢��� ��ࠡ��! ���஡�� �� ࠧ!\n");
                        system("pause");
                        system("cls");
                    }
                }
                else{
                printf("�����४⭮ ������� �ਥ���� ��஡��! ���஡�� �� ࠧ!\n");
                system("pause");
                system("cls");
                }
            }
        }
        else{
            fflush(stdin);
            printf("�����४⭮ ������� ���न���� ��஡��! ���஡�� �� ࠧ!\n");
            system("pause");
            system("cls");
        }
    }
    system("cls");
}
int RandomPutShips(struct Player *player){ // ����⠭���� ��ࠡ��� ��⮬���᪨
    int ship = 0;
    int try = 0;
    int x, y, z;
    while(ship < 10 && try < 200){
        try = 0;
        x = rand()%10;
        y = rand()%10;
        z = rand()%2;
        if(z == 0){
            if(PutHorizontal(player, ship, x, y) == 0) ship++;
            else try++;
        }
        if(z == 1){
            if(PutVertical(player, ship, x, y) == 0) ship++;
            else try++;
        }
    }
    if(ship == 10) return 0;
    else return -1;
}
void PutShips(struct Player *player, int p){
    char z;
    printf("PLAYER %d ���������� �������:\n  1. �������\n  2. ��������� �������\n��� �롮�:", p);
    z = getch();
        while( z != '1' && z!= '2'){
            printf("�����४�� ����!���஡�� �� ࠧ!\n");
            z = getch();
        }
    system("cls");
    if(z == '1'){
        UserPutShips(player, p);
    }
    else if(z == '2'){
        while(RandomPutShips(player) != 0) InitPlayer(player);
    }
}



//���� ��ப�� � AI
int UserMove(struct Player player1, struct Player *player2, int p){// 室 ���짮��⥫�
    int x;
    char y;
    int result = Invalid;
    while(result != Missed){
        Draw12(player1, 1, *player2, 0, p);
        fflush(stdin);
        printf("������ ���न����: ");
        if(scanf("%d%c", &x, &y) == 2){
            result = Shoot(player2,CoorX(x),CoorY(y));
            if(result == Invalid){
                printf(" �� ������ ���न��� 㦥 ��५﫨 ��� �� �� �������!\n");
                system("pause");
                system("cls");
            }
            else if(result == Missed){
                system("cls");

            }
            else if(result == Hit){
                system("cls");
            }
            else if(result == Killed){
                system("cls");
                if((*player2).ships == 0){
                    History(p, CoorX(x), CoorY(y), result);
                    return 1;
                }
            }
        }
        else{
            fflush(stdin);
            printf("�����४⭮ ������� ���न���� ��ࠡ��! ���஡�� �� ࠧ!\n");
            system("pause");
            system("cls");
        }
    History(p, CoorX(x), CoorY(y), result);
    Draw12(player1, 1, *player2, 0, p);
    system("pause");
    system("cls");
    }
    return 0;
}
int AimShootComputer(struct Player *player, int x, int y, int *d){//०�� ��楫쭮� ��५�� ��᫥ ��ࢮ�� ���������
    int result = Invalid;
    int a,b, nrand;
    while(result == Invalid){
        nrand = rand()%4;
        a = x;
        b = y;
        if(*d == -1) nrand = rand()%4;
        if(*d == 0) nrand = 1;
        if(*d == 1) nrand = 0;
        if(*d == 2) nrand = 3;
        if(*d == 3) nrand = 2;
        dirs(&a,&b, nrand);
        result = Shoot(player,a,b);
        if(result == Missed){
            *d = -1;
            History(2, a, b, Missed);
            return Missed;
        }
        while(result == Hit){
            History(2, a, b, Hit);
            dirs(&a,&b, nrand);
            result = Shoot(player,a,b);
            if(result == Missed){
                *d = nrand;
                History(2, a, b, Missed);
                return Missed;
            }
        }
    }
    History(2, a, b, Killed);
    return Killed;
}
int RandShootComputer(struct Player *player, int *x, int *y){// ��砩��� ��५졠
    int result = Invalid;
    while(result != Missed){
        *x = rand()%10; 
        *y = rand()%10;
        result = Shoot(player,*x,*y);
        if(result == Hit){
            History(2, *x, *y, Hit);
            return Hit;
        }
        if(result == Killed){
            History(2, *x, *y, Killed);
            return Killed;
        }
    }
    History(2, *x, *y, Missed);
    return Missed;
}
int ComputerMove(struct Player *player, int *x, int *y, int *d){ // 室 �������� 
    int result = Invalid;
    if((*player).field[*x][*y] == DestroyedCell){
        result = AimShootComputer(player,*x,*y, d);
        if((*player).ships == 0) return 1;
    }
    while(result != Missed){
        *d = -1;
        result = RandShootComputer(player,x,y);
        if(result == Hit){
            result = AimShootComputer(player,*x,*y, d);
            if((*player).ships == 0) return 1;
        }
        else if(result == Killed){
            if((*player).ships == 0) return 1;
        }
    }
    return 0;
}




//������ ���� 
int PlayGameUserUser(struct Player *player1, struct Player *player2){ //०�� ���� ���짮��⥫� - ���짮��⥫� 
    int p = 1;
    char z;
    printf("PLAYER 1 ������������ ���������� �������\n");
    system("pause");
    system("cls");
    z = '2';
    while(z == '2'){
        InitPlayer(player1);
        PutShips(player1, 1);
        Draw1(*player1, -1, 1, 0);
        printf("������� ������� �����������\n");
        printf("�த������?\n  1. ��\n  2. ���, ����⠢��� ��ࠡ��\n");
        z = getch();
        while( z != '1' && z!= '2'){
            printf("�����४�� ����!���஡�� �� ࠧ!\n");
            z = getch();
        }
        system("cls");
    }
    printf("PLAYER 2 ������������ ���������� �������\n");
    system("pause");
    system("cls");    
    z = '2';
    while(z == '2'){
        InitPlayer(player2);
        PutShips(player2, 2);
        Draw1(*player2, -1, 2, 0);
        printf("������� ������� �����������\n");
        printf("�த������?\n  1. ��\n  2. ���, ����⠢��� ��ࠡ��\n");
        z = getch();
        while(z != '1' && z!= '2'){
            printf("�����४�� ����!���஡�� �� ࠧ!\n");
            z = getch();
        }
        system("cls");
    }          
    printf("������ ������������ � ����\n");
    system("pause");
    system("cls");
    while( (*player1).ships != 0 || (*player2).ships != 0){
        if(p == 1){
            printf("��� PLAYER 1\n");
            system("pause");
            system("cls");
            if(UserMove(*player1, player2, p) == 1) {
                system("cls");
                return 0;
            }
            system("cls");
        }
        if(p == 2){
            printf("��� PLAYER 2\n");
            system("pause");
            system("cls");
            if(UserMove(*player2, player1, p) == 1) {
                system("cls");
                return 1;
            }
            system("cls");
        }
        p = 3 - p;
    }
}
int PlayGameUserComputer(struct Player *player1, struct Player *player2){// ०�� ���� ���짮��⥫� - ��������
    int x = 0, y = 0, d = -1;
    int p = 1;
    char z;
    z = '2';
    while(z == '2'){
        printf("���������� ��� �������\n");
        InitPlayer(player1);
        PutShips(player1, 1);
        Draw1(*player1, -1, 1, 0);
        printf("������� ������� �����������\n");
        printf("�த������?\n  1. ��\n  2. ���, ����⠢��� ��ࠡ��\n");
        z = getch();
        while(z != '1' && z!= '2'){
            printf("�����४�� ����!���஡�� �� ࠧ!\n");
            z = getch();
        }
        system("cls");
    }
    InitPlayer(player2);
    while(RandomPutShips(player2) != 0) InitPlayer(player2);;
    printf("����� ����������� � ����\n");
    system("pause");
    system("cls");
    while( (*player1).ships != 0 || (*player2).ships != 0){
        if(p == 1){
            if(UserMove(*player1, player2, p) == 1) {
                system("cls");
                return 0;
            }
        }
        if(p == 2){
            if(ComputerMove(player1, &x, &y, &d) == 1) {
                system("cls");
                return 1;
            }
        }
        p = 3 - p;
    }
}


void SeaBattle(){
    printf("                                            ***                     ***  \n");
    printf("  **   **  ****  ****   ****  ** **  ****  **  **      ****  ****  **  **\n");
    printf("  **   ** **  ** ** ** **  ** ** ** **  ** ** ***      **   **  ** ** ***\n");
    printf("  *** *** **  ** ** ** **     ****  **  ** ** ***      **   **  ** ** ***\n");
    printf("  *** *** **  ** ** ** **     ***   **  ** ** ***      **** **  ** ** ***\n");
    printf("  *** *** **  ** ****  **     ****  **  ** *** **      ** ****  ** *** **\n");
    printf("  ** * ** **  ** **    **  ** ** ** **  ** *** **      ** ****  ** *** **\n");
    printf("  ** * ** **  ** **    **  ** ** ** **  ** *** **      ** ****  ** *** **\n");
    printf("  ** * **  ****  **     ****  ** **  ****  **  **      ****  ****  **  **\n");
    printf("\n");
    printf("                                                <c> ������ ��ॢ�, 2020\n");
    printf("����� ���� \n");
    printf("  1. ���᪮� ��� ���� �� ����\n");
    printf("  2. ��� ��⨢ ���\n");
    printf("  3. �ࠢ��� ����\n");
    printf("  4. ��室\n");
    printf("��� �롮�: ");

}
//�᭮���� �㭪�� 
int Menu(){
    srand(time(NULL));//���樠����஢��� ������� 
    struct Player player1, player2;
    InitPlayer(&player1);
    InitPlayer(&player2);
    char z = '0';
    while (z != '4'){
        SeaBattle();
        z = getch();
        while(z != '1' && z!= '2' && z != '3' && z!= '4'){
            printf("�����४�� ����!���஡�� �� ࠧ!\n");
            z = getch();
        }
        system("cls");
        if(z == '1'){
            if(PlayGameUserUser(&player1, &player2) == 0){
                Draw12(player1, 1, player2, 1, 1);
                printf("PLAYER 1 ������� ������!\n");
                system("pause");
                system("cls");
            }
            else{
                Draw12(player1, 1, player2, 1, 1);
                printf("PLAYER 2 ������� ������!\n");
                system("pause");
                system("cls");
            }
        }
        else if( z == '2'){
            if(PlayGameUserComputer(&player1, &player2) == 0){
                Draw12(player1, 1, player2, 1, 1);
                printf("�� �������� � �������������� ����������!\n");
                printf("�⮡� �������� � ���� ������ 1..\n");
                z = getch();
                while(z != '1'){
                    printf("�����४�� ����!���஡�� �� ࠧ!\n");
                    z = getch();
                }
                if(z == '1'){
                    system("cls");
                    Menu();
                }
            }
            else{
                Draw12(player1, 1, player2, 1, 1);
                printf("�� ���������  �������������� ����������!\n");
                printf("�⮡� �������� � ���� ������ 1..\n");
                z = getch();
                while(z != '1'){
                    printf("�����४�� ����!���஡�� �� ࠧ!\n");
                    z = getch();
                }
                if(z == '1'){
                    system("cls");
                    Menu();
                }
            }
        }
        else if( z == '3'){
            printf("��� �ਢ������ ��� ���᪮� ���. ���� ���� ��⮨� �  ⮬, �⮡� �⮯���  ���� 䫮� ᮯ�୨��.\n");
            printf("��஢�� ���� - ��� ������ 10x10 � ������� ��ப�, �� ����� �� ��� �㤥� ࠧ������� ᢮� 䫮�,\n");
            printf("� ��㣮� �㤥� ������ ����� �� 䫮�� ��⨢����.\n");
            printf("1. �ࠢ��� ࠧ��饭�� ��ࠡ���\n");
            printf("���짮��⥫�  ���⠢���  ���� ��ࠡ��  �� �����  ���⮪ (������), ��� ��ࠡ�� �� ��� ���⮪\n");
            printf("(�३��), �� ��ࠡ�� �� ���� ���⮪ (�ᬨ���) � ���� ��ࠡ�� ࠧ��஬ � ���� ����� (����).\n");
            printf("�� �⮬: ��ࠡ��  �� ������ ��室��� ��  �।��� �����; ��ࠡ�� ������  �ᯮ�������� ⮫쪮 ��\n");
            printf("��ਧ��⠫� ��� ���⨪���;�� ࠧ��饭�� ��ࠡ�� �� ����� ������� ��� ��㣠 ��஭��� � 㣫���.\n");
            printf("2. �ࠢ��� ���᪮�� ���\n");
            printf("��᫥ ���⠭���� ��ࠡ��� ��稭����� ���.�� �।�⠢��� ᮡ�� ����।�� ����५� ��ப��.��\n");
            printf("���������   �  ��ࠡ��  ��⨢����  ���⭨� ���  ����砥� ����������� �஢������  �����।����\n");
            printf("����५�. ��� �����稢����� �� 㭨�⮦���� ����� �� ���⭨��� ��� ��ࠡ��� ��⨢����.\n");
            printf("�⮡� �������� � ���� ������ 1..\n");
            z = getch();
            while(z != '1'){
                printf("�����४�� ����!���஡�� �� ࠧ!\n");
                z = getch();
            }
            if(z == '1'){
                system("cls");
                Menu();
            }
        }
    }
    return 0;
}
int main(){
    if(Menu() == 0) return 0;
}
