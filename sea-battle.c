#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include <time.h> 

//Глобальные переменные и константы 
struct Player{
    int field[10][10]; // поле игрока с расставленными на них кораблями
    int qt_ships[10]; // будем отслеживать кол-во кораблей на поле и их целостность
    int ships; // кол-во не потопленных кораблей  
};
struct H{ //хранит данные об истории выстрела
    int p; //номер игрока
    int x; //координата числа
    int y; // координата буквы
    int result; // результат выстрела
};
struct H h[] = {{0, 0, 0 ,0},{0, 0, 0, 0},{0, 0 ,0 ,0},{0, 0, 0, 0},{0, 0, 0 ,0},{0, 0, 0, 0},{0, 0 ,0 ,0},{0, 0, 0, 0}, {0, 0 ,0 ,0},{0, 0, 0, 0}} ;
//Для описания исходов выстрела введём перечисление. 
const int Invalid = 1, // не существует клетки с заданными координатами; в клетку (x, y) уже стреляли;
          Missed  = 2, // клетка (x, y) пуста: промах 
          Hit     = 3, // клетка (x, y) занята кораблём: попали, но не убили 
          Killed  = 4; // клетка (x, y) занята кораблём: попали и убили корабль 
const int  EmptyCell     =  -1, // клетка пустая 
           MissedCell    =  -2, // уже стреляли, до выстрела была пустой 
           DestroyedCell =  -3, // подбитая палуба
           BanCell       =  -4; //ореол корабля 
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
char Cipher(int n){ //преобразует цифры в символы       
    if(n == EmptyCell || n == BanCell) return EmptyChar;
    if(n == MissedCell) return MissedChar;
    if(n == DestroyedCell) return DestroyedChar;
    return 254;
}
void dirs(int *x, int *y, int c){//Выбор направления
    int a = *x, b = *y;
    if(c == 0) a--;
    if(c == 1) a++;
    if(c == 2) b++;
    if(c == 3) b--;
    *x = a;
    *y = b;
}
void History(int p, int x, int y, int result){//история последних 10 ходов
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
void InitPlayer(struct Player *player){// инициализация полей структуры 
    int sh[10] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
    for(int i = 0; i < 10; i++){
        (*player).qt_ships[i] = sh[i];
        for(int j = 0; j < 10; j++){
            (*player).field[i][j] = EmptyCell;
        }
    }
    (*player).ships = 10;
}
int CheckExit(struct Player player, int x, int y){ // функция проверки на удачное размещение одной палубы (поле, координаты палубы)
    if( x < 0 || x > 9 ||y < 0 || y > 9) return -1; // выход за границы 
    else if(player.field[x][y] == EmptyCell){
        return 0;// можно разместить корабль
    }
    else return -2; //в клетке уже есть палуба или корабли соприкасаются 
}




//Функции для управления структурой 
int PutHorizontal(struct Player *player, int ship, int x, int y){ //(поле, ship, координаты ) где ship индекс массива player.qt_ships
    //размещает корабль горизонтально(вводим координаты "головы" и размещаем остальну часть корабля правее ее)
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
int PutVertical(struct Player *player, int ship, int x, int y){ //(поле, ship, координаты ) где ship индекс массива player.qt_ships
    //размещает корабль вертикатьно(вводим координаты "головы" и размещаем остальну часть корабля ниже ее)
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

void AreaS(struct Player *player, int x, int y){ //ореол корабля после потопления корабля
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
int Shoot (struct Player *player, int x, int y){ // выстрелить в клетку x, y по полю player.field[][]
    int ship;
    if( x < 0 || x > 9 ||y < 0 || y > 9) return Invalid; //выход за границы поля
    else if((*player).field[x][y] == MissedCell || (*player).field[x][y] == DestroyedCell) return Invalid; // в эту точку уже попадали
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






//ВЫВОД
// вывод одного поля для расстановки кораблей 
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
        if(i == 0)      printf("    Корабли: \n");
        else if(i == 1) printf("    1   Линкор (\"4444\")\n");
        else if(i == 2) printf("    2 Крейсера (\" 333\")\n");
        else if(i == 3) printf("    3  Эсминца (\"  22\")\n");
        else if(i == 4) printf("    4   Катера (\"   1\")\n");
        else if(i == 5) printf("    Осталось: %d из 10\n", 10 - ship );
        else if(i == 6) printf("    Правила ввода: Положение коробля определяется координатами\n");
        else if(i == 7) printf("    и  ориентацией палубы: 0 - горизонтально; 1 - вертикально.\n");
        else if(i == 8) printf("    Остальные палубы расставляются правее или ниже данной соответственно.\n");
        else if(i == 9) printf("    Для катера ориентация не важна. Пример ввода координат: 1a\n");
        }
        else printf("\n");
    }
    printf("   a b c d e f g h i j\n");
    if(player.qt_ships[ship] == 4) printf("Будет размещен линкор!\n");
    else if(player.qt_ships[ship] == 3) printf("Будет размещен крейсер!\n");
    else if(player.qt_ships[ship] == 2) printf("Будет размещен эсминец!\n");
    else if(player.qt_ships[ship] == 1) printf("Будет размещен катер!\n");

    

}
// вывод полей на экран
void Draw12(struct Player player1, int open1, struct Player player2, int open2, int p){ // open == 1, поле player.field[][] показывается открытом режиме, иначе в закрытом
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
        if(i == 4)      printf("      PLAYER 1: %d кораблей\n", player1.ships);
        else if(i == 5) printf("      PLAYER 2: %d кораблей\n", player2.ships);
        else printf("\n");
    }
    printf("   a b c d e f g h i j        a b c d e f g h i j          Пример ввода координат: 1a\n");
    printf("____________________________________________________\n");
    for(int i = 9; i >= 0; i--){
        if(h[i].result == 0);
        if(h[i].result == Missed) printf("PLAYER %d: (%2d%c) - ПРОМАХ!\n", h[i].p, X(h[i].x), Y(h[i].y));
        if(h[i].result == Hit) printf("PLAYER %d: (%2d%c) - КОРАБЛЬ РАНЕН!\n", h[i].p, X(h[i].x), Y(h[i].y));
        if(h[i].result == Killed) printf("PLAYER %d: (%2d%c) - КОРАБЛЬ УБИТ!\n", h[i].p, X(h[i].x), Y(h[i].y));
    }
    printf("____________________________________________________\n");
}





//Размещение кораблей 
void UserPutShips(struct Player *player, int p){//Расстановка кораблей в ручную
    int ship = 0, x, z;
    char y;
    while(ship < 10){
        fflush(stdin);
        Draw1(*player, ship, p, 1);
        printf("Введите координаты:");
        if(scanf("%d%c", &x, &y) == 2){
            printf("Введите ориентацию:");
            fflush(stdin);
            if(scanf("%d", &z) != 1){
                fflush(stdin);
                printf("Некорректно введена ориентация коробля! Попробуйте еще раз!\n");
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
                        printf("Неудачная попытка поставить корабль! Попробуйте еще раз!\n");
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
                        printf("Неудачная попытка поставить корабль! Попробуйте еще раз!\n");
                        system("pause");
                        system("cls");
                    }
                }
                else{
                printf("Некорректно введена ориентация коробля! Попробуйте еще раз!\n");
                system("pause");
                system("cls");
                }
            }
        }
        else{
            fflush(stdin);
            printf("Некорректно введены координаты коробля! Попробуйте еще раз!\n");
            system("pause");
            system("cls");
        }
    }
    system("cls");
}
int RandomPutShips(struct Player *player){ // Расстановка кораблей автоматически
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
    printf("PLAYER %d РАССТАВИТЬ КОРАБЛИ:\n  1. ВРУЧНУЮ\n  2. СЛУЧАЙНЫМ ОБРАЗОМ\nВаш выбор:", p);
    z = getch();
        while( z != '1' && z!= '2'){
            printf("Некорректный ввод!Попробуйте еще раз!\n");
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



//Ходы игроков и AI
int UserMove(struct Player player1, struct Player *player2, int p){// ход пользователя
    int x;
    char y;
    int result = Invalid;
    while(result != Missed){
        Draw12(player1, 1, *player2, 0, p);
        fflush(stdin);
        printf("Введите координаты: ");
        if(scanf("%d%c", &x, &y) == 2){
            result = Shoot(player2,CoorX(x),CoorY(y));
            if(result == Invalid){
                printf(" По данной координате уже стреляли или ее не существует!\n");
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
            printf("Некорректно введены координаты корабля! Попробуйте еще раз!\n");
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
int AimShootComputer(struct Player *player, int x, int y, int *d){//режим прицельной стрельбы после первого попадания
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
int RandShootComputer(struct Player *player, int *x, int *y){// случайная стрельба
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
int ComputerMove(struct Player *player, int *x, int *y, int *d){ // ход компьютера 
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




//Режимы игры 
int PlayGameUserUser(struct Player *player1, struct Player *player2){ //режим игры пользователь - пользователь 
    int p = 1;
    char z;
    printf("PLAYER 1 ПРИГОТОВТЕСЬ РАССТАВИТЬ КОРАБЛИ\n");
    system("pause");
    system("cls");
    z = '2';
    while(z == '2'){
        InitPlayer(player1);
        PutShips(player1, 1);
        Draw1(*player1, -1, 1, 0);
        printf("КОРАБЛИ УСПЕШНО РАССТАВЛЕНЫ\n");
        printf("Продолжить?\n  1. Да\n  2. Нет, переставить корабли\n");
        z = getch();
        while( z != '1' && z!= '2'){
            printf("Некорректный ввод!Попробуйте еще раз!\n");
            z = getch();
        }
        system("cls");
    }
    printf("PLAYER 2 ПРИГОТОВТЕСЬ РАССТАВИТЬ КОРАБЛИ\n");
    system("pause");
    system("cls");    
    z = '2';
    while(z == '2'){
        InitPlayer(player2);
        PutShips(player2, 2);
        Draw1(*player2, -1, 2, 0);
        printf("КОРАБЛИ УСПЕШНО РАССТАВЛЕНЫ\n");
        printf("Продолжить?\n  1. Да\n  2. Нет, переставить корабли\n");
        z = getch();
        while(z != '1' && z!= '2'){
            printf("Некорректный ввод!Попробуйте еще раз!\n");
            z = getch();
        }
        system("cls");
    }          
    printf("ИГРОКИ ПРИГОТОВТЕСЬ К ИГРЕ\n");
    system("pause");
    system("cls");
    while( (*player1).ships != 0 || (*player2).ships != 0){
        if(p == 1){
            printf("ХОД PLAYER 1\n");
            system("pause");
            system("cls");
            if(UserMove(*player1, player2, p) == 1) {
                system("cls");
                return 0;
            }
            system("cls");
        }
        if(p == 2){
            printf("ХОД PLAYER 2\n");
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
int PlayGameUserComputer(struct Player *player1, struct Player *player2){// режим игры пользователь - компьютер
    int x = 0, y = 0, d = -1;
    int p = 1;
    char z;
    z = '2';
    while(z == '2'){
        printf("РАЗМЕСТИТЕ ВСЕ КОРАБЛИ\n");
        InitPlayer(player1);
        PutShips(player1, 1);
        Draw1(*player1, -1, 1, 0);
        printf("КОРАБЛИ УСПЕШНО РАССТАВЛЕНЫ\n");
        printf("Продолжить?\n  1. Да\n  2. Нет, переставить корабли\n");
        z = getch();
        while(z != '1' && z!= '2'){
            printf("Некорректный ввод!Попробуйте еще раз!\n");
            z = getch();
        }
        system("cls");
    }
    InitPlayer(player2);
    while(RandomPutShips(player2) != 0) InitPlayer(player2);;
    printf("ИГРОК ПРИГОТОВЬСЯ К ИГРЕ\n");
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
    printf("                                                <c> Валерия Карева, 2020\n");
    printf("НОВАЯ ИГРА \n");
    printf("  1. Морской бой один на один\n");
    printf("  2. Игра против бота\n");
    printf("  3. Правила игры\n");
    printf("  4. Выход\n");
    printf("Ваш выбор: ");

}
//Основная функция 
int Menu(){
    srand(time(NULL));//инициализировали генератор 
    struct Player player1, player2;
    InitPlayer(&player1);
    InitPlayer(&player2);
    char z = '0';
    while (z != '4'){
        SeaBattle();
        z = getch();
        while(z != '1' && z!= '2' && z != '3' && z!= '4'){
            printf("Некорректный ввод!Попробуйте еще раз!\n");
            z = getch();
        }
        system("cls");
        if(z == '1'){
            if(PlayGameUserUser(&player1, &player2) == 0){
                Draw12(player1, 1, player2, 1, 1);
                printf("PLAYER 1 ОДЕРЖАЛ ПОБЕДУ!\n");
                system("pause");
                system("cls");
            }
            else{
                Draw12(player1, 1, player2, 1, 1);
                printf("PLAYER 2 ОДЕРЖАЛ ПОБЕДУ!\n");
                system("pause");
                system("cls");
            }
        }
        else if( z == '2'){
            if(PlayGameUserComputer(&player1, &player2) == 0){
                Draw12(player1, 1, player2, 1, 1);
                printf("ВЫ ВЫИГРАЛИ У ИСКУССТВЕННОГО ИНТЕЛЛЕКТА!\n");
                printf("Чтобы вернуться в меню нажмите 1..\n");
                z = getch();
                while(z != '1'){
                    printf("Некорректный ввод!Попробуйте еще раз!\n");
                    z = getch();
                }
                if(z == '1'){
                    system("cls");
                    Menu();
                }
            }
            else{
                Draw12(player1, 1, player2, 1, 1);
                printf("ВЫ ПРОИГРАЛИ  ИСКУССТВЕННОМУ ИНТЕЛЛЕКТУ!\n");
                printf("Чтобы вернуться в меню нажмите 1..\n");
                z = getch();
                while(z != '1'){
                    printf("Некорректный ввод!Попробуйте еще раз!\n");
                    z = getch();
                }
                if(z == '1'){
                    system("cls");
                    Menu();
                }
            }
        }
        else if( z == '3'){
            printf("Вас приветствует игра Морской бой. Цель игры состоит в  том, чтобы утопить  весь флот соперника.\n");
            printf("Игровое поле - два квадрата 10x10 у каждого игрока, на одном из них будет размещаться свой флот,\n");
            printf("в другом будет вестись огонь по флоту противника.\n");
            printf("1. Правила размещения кораблей\n");
            printf("Пользователь  расставляет  один корабль  из четырех  клеток (линкор), два корабля из трех клеток\n");
            printf("(крейсер), три корабля из двух клеток (эсминец) и четыре корабля размером в одну клетку (катер).\n");
            printf("При этом: корабли  не должны выходить за  пределы карты; корабли должны  располагаться только по\n");
            printf("горизонтали или вертикали;при размещении корабли не могут касаться друг друга сторонами и углами.\n");
            printf("2. Правила морского боя\n");
            printf("После расстановки кораблей начинается бой.Он представляет собой поочередные выстрелы игроков.При\n");
            printf("попадании   в  корабль  противника  участник боя  получает возможность проведения  внеочередного\n");
            printf("выстрела. Игра заканчивается при уничтожении одним из участников всех кораблей противника.\n");
            printf("Чтобы вернуться в меню нажмите 1..\n");
            z = getch();
            while(z != '1'){
                printf("Некорректный ввод!Попробуйте еще раз!\n");
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
