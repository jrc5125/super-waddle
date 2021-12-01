/* Calma, John Michael Angelo R.
 * 1001945125
 * CSE 1320-004 HW5-2
*/

/* To compile: gcc -o lights lights.c
 * Usage: ./lights [filename.txt]
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* struct node constructor */
typedef struct Node {
  char* color;   /*color of the light*/
  int** details; /*holds the brightness and size*/
  struct Node* next;
} Node;

/* free heap memory by recursion */
void free_ll(Node* node) {
  if (node) {
    free_ll(node->next);
    node->next = NULL;

    //per node element
    free(node->color);
    free(node->details[0]);
    free(node->details[1]);
    free(node->details);

    free(node);
  }
}

/* data constructor for node */
void create_Node(Node* newNode, char* c, int num1, int num2) {
  //allocate memory for each data element
  newNode->color = (char*)malloc(strlen(c) + 1);
  newNode->details = (int**)malloc(2*sizeof(int*));
  newNode->details[0] = (int*)malloc(sizeof(int));
  newNode->details[1] = (int*)malloc(sizeof(int));

  //fill in data
  strcpy(newNode->color, c);
  *(newNode->details[0]) = num1;
  *(newNode->details[1]) = num2;
  newNode->next = NULL;
}

/* insert Node* curr into a linked list at Node** root */
void push_back(Node** root, Node* curr) {
  if (*root == NULL) { //linked list is empty
    *root = curr;
  }
  else { //insert at end
    Node* temp = *root;
    while (temp->next != NULL) {
      temp = temp->next;
    }
    temp->next = curr;
  }
}

/* create linked list and return head node */
Node* light_info(char* filename) {
  Node* headNode = NULL; //making sure pointer initially has invalid address
  FILE* fp = fopen(filename, "r");

  if (fp != NULL) { //file opened successfully
    char fline[100];
    while (fgets(fline, sizeof(fline), fp) != NULL) {
      Node* new = (Node*)malloc(sizeof(Node)); //dynamically allocate memory for nodes

      //process each line in file
      char* color = strtok(fline, ",");
      int num1 = atoi(strtok(NULL, ","));
      int num2 = atoi(strtok(NULL, "\n"));

      //create node from data and add to linked list
      create_Node(new, color, num1, num2);
      push_back(&headNode, new);
    }

    fclose(fp);
    return headNode;
  }
  else { //problem with fopen
    printf("Error opening file %s\n", filename);
    exit(EXIT_FAILURE);
  }
}

/* display christmas lights using linked list */
void display_lights(Node* head, int lswitch) {
  Node* temp = head; //make copy of ll
  do {
    int i;
    for (i = 0; i < *(temp->details[0]); i++) {
      if (lswitch) { //if turning on
        //bulb size is big
        if (*(temp->details[1])) printf("%c", *(temp->color)-32);

        //bulb size is small
        else printf("%c", *(temp->color));
      }
      else printf("-"); //if turning off
    }
    printf(" ");
    temp = temp->next;
  } while (temp != NULL);
  printf("\n\n");
}

int main(int argc, char* argv[]) {
  if (argc == 2) { //check for proper arguments
    Node* stringlight = light_info(argv[1]);
    char switchstate[10]; //user typed input
    int lightstate = 0; //on or off, default off

    do {
      //user will type on, off, or exit
      fgets(switchstate, sizeof(switchstate), stdin);
      strtok(switchstate, "\n");

      if (!strcasecmp(switchstate, "on")) {
        if (!lightstate) {
          printf("\n***Turning lights on:\n");
          lightstate = 1;
          display_lights(stringlight, lightstate);
        }
        else {
          printf("\n--Lights are already on\n\n");
        }
      }
      else if (!strcasecmp(switchstate, "off")) {
        if (!lightstate) {
          printf("\n--Lights are already off\n\n");
        }
        else {
          printf("\n***Turning lights off\n\n");
          lightstate = 0;
          display_lights(stringlight, lightstate);
        }
      }
      else if (!strcasecmp(switchstate, "exit")) {
        printf("\nExiting...\n");
      }
      else { //user did not type a valid choice
        printf("\nInvalid input. Try again\n\n");
      }
    } while (strcasecmp(switchstate, "exit"));
    free_ll(stringlight);
  }
  else { //arguments are not good
    printf("usage: lights FILENAME\n");
    printf("  where:\n");
    printf("  FILENAME is .txt file containing \n");
    exit(EXIT_FAILURE);
  }
  return 0;
}
