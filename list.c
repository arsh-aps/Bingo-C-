#define NULL 0
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

void ADD(int item, List *list){
    Node *next = malloc(sizeof(Node));
    next->value = item;
    next->next = NULL;
    if (list->Start == NULL) list->Start = next;
    else list->End->next = next;
    
    list->End = next;
}

int CHECK(int item, List *list){
    Node *current = list->Start;
    while (current != NULL){
        if (current->value == item) return 1;
        current = current->next;
    }
    return 0;
}

int Clamp(int x){
    if (x>0) return x;
    return 0;
}

void PRINT(List *list){
    Node *current = list->Start;
    char letters[] = {'L', 'I', 'N', 'U', 'X'};
    printf("CallList: ");
    while (current != NULL){
        printf("%c", letters[Clamp(current->value - 1) / 15]);
        printf("%02d", current->value);
        current = current->next;
        if (current != NULL) printf(" ");
    }
}
List *INITIALIZE(int size){
    List *list = malloc(sizeof(List));
    list->Start = NULL;
    list->End = NULL;
    for (int i = 0; i < size; i++){
        ADD(0, list);
    }
    return list;
}
