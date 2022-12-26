#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include "list.h"
#define GeneralRowRegex "[0-9][0-9] [0-9][0-9] [0-9][0-9] [0-9][0-9] [0-9][0-9]"
#define Row3Regex "[0-9][0-9] [0-9][0-9] 00 [0-9][0-9] [0-9][0-9]"
#define clear "clear"
#define marker 1000

regex_t regex;

int nums_check (int *a, int num); 
//function for checking if numbers in column are unique and fall within the appropriate range

int check_cardformat(FILE *fp);
//function checking for general card format correctness

void print_board(int* board[]);
//function that prints the board

void is_number_in(int* board[], int number);
//checks if the called number is on the board, and then marks it

int check_win_condition(int* board[]);
//checks win condition

int main(int argc, char *argv[]){
    int seed = -1; 
    int i, call_num;
    char temp = '\0';
    char line[16];
    FILE *fp;
    char user_inp[77];
    char c, prefix;
    int L[5], I[5], N[5], U[5], X[5];
    int* columns[] = {L, I, N, U, X};

    if (argc != 3){
        fprintf(stderr,"Usage: %s seed cardFile\n",argv[0]);
        exit(1);
    }

    sscanf(argv[1], "%d%c", &seed,&temp);
    if (fopen(argv[1], "r") != NULL || temp || seed < 0){
        fprintf(stderr, "Expected unsigned integer seed, but got %s\n",argv[1]);
        exit(2);
    } else srand((unsigned) seed);

    if ((fp=fopen(argv[2],"r")) == NULL ){
        fprintf(stderr, "%s is nonexistent or unreadable\n",argv[2]);
        exit(3);
    }

    for (i=0;i<5;i++){
        fgets(line,16,fp);
        sscanf(line,"%d %d %d %d %d", &L[i],&I[i],&N[i],&U[i],&X[i]);
    }

    fseek(fp, 0,  SEEK_SET); //resets pointer to start of file

    if ((check_cardformat(fp)) == 1 || nums_check(L, 1) == 1|| nums_check(I, 2) == 1|| nums_check(N, 3) == 1|| nums_check(U, 4) == 1|| nums_check(X, 5) == 1){
        fprintf(stderr, "%s has bad format\n",argv[2]);
        exit(4);
    }

    system(clear);
    List *call_list = INITIALIZE(0);
    columns[2][2] = marker;
    do {
        PRINT(call_list);
        printf("\n\nL   I   N   U   X\n");
        print_board(columns);
        printf("Enter any non-enter key for Call (q to quit): ");
        fgets(user_inp,77,stdin);
        for (i = 0; i < 75; i++)
        {
            c = user_inp[i]; 
            if (c == 'q' || c == '\n') break;
            call_num = (rand() % 75 + 1);
            while (CHECK(call_num, call_list)) call_num = (rand() % 75 + 1);
            is_number_in(columns, call_num);
            ADD(call_num, call_list);
            if (check_win_condition(columns)){
                system(clear);
                PRINT(call_list);
                printf("\n\nL   I   N   U   X\n");
                print_board(columns);
                printf("WINNER\n");
                exit(0);
            }
            
        }
        
        system(clear);

    }while (c!='q');

    PRINT(call_list);
    printf("\n\nL   I   N   U   X\n");
    print_board(columns);

    exit(0);
}

int Max(int a,int b){
    if (a > b) return a;
    return b;
}

int nums_check (int *a, int num){
    int i, ii;
    for (i = 0;i<5;i++){
        for (ii = i + 1;ii<5;ii++){
           if (!(num == 3 && i == 2)){ 
            if (a[i] <= 15 * Max(0,num-1) || a[i] > (15 * num) || a[i] == a[ii]){
                return 1;
            }
           }
        }
        
    }
    return 0;

}

int check_cardformat(FILE *fp){
    char line[16];
    int i;
    for (i=0;i<5;i++){
        fgets(line,16,fp);
        if (i == 2) regcomp(&regex, Row3Regex,0);   
        else regcomp(&regex, GeneralRowRegex,0);
        
        if ( regexec(&regex, line, 0, NULL, 0) != 0){
            return 1;
        }

    }
        return 0;

}

void print_board(int* board[]){
    int i,j = 0;
    for (i=0; i < 5; i++){
        for (j=0; j < 5; j++){
            if (board[j][i] >= marker) printf("%02dm", board[j][i] % marker); //applies modulo operator to get 'unmarked' number
            else printf("%02d", board[j][i]);
            if (j != 4 && board[j][i] < marker) printf("  ");
            else if (j != 4) printf(" ");
        }
        printf("\n");
    }
}

void is_number_in(int* board[], int number){
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 5; j++){
        if (board[i][j] == number){  
            board[i][j] += marker; //adds a predefined constant to a number in the board array to 'mark' it
            return;
        }
        }
    }
}

int check_win_condition(int* board[]){
    int i,j = 0;
    for (i=0; i < 5; i++){
        int sum = 0;
        for (j=0; j < 5; j++) sum += board[i][j];
        if (sum > 5000) return 1;
    }

    for (i=0; i < 5; i++){
        int sum = 0;
        for (j=0; j < 5; j++) sum += board[j][i];
        if (sum > 5000) return 1;
    }

    int sum = board[0][0] + board[0][4] + board[4][0] + board[4][4];
    if (sum > 4000) return 1;
    return 0;
}

