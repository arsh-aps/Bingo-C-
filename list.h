typedef struct Node{
  int value;
  struct Node* next;
} Node;

typedef struct List {
   Node* Start;
   Node* End;
} List; 

void ADD(int item, List *list);
int CHECK(int item, List *list);
void PRINT(List *list);
List *INITIALIZE(int size);