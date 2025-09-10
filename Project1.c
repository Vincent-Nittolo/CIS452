#include<stdio.h>
#include<stdlib.h>

#ifndef LAB3_C
#define LAB3_C

struct node 
{
	int data;
	struct node *next;
};

struct node *init (){
	struct node *head = (struct node*)malloc((sizeof(struct node)) *(sizeof(struct node) ));
	return head;
}

 //initialize: create an empty head node (whose "data" is intentionally missing); This head node will not be used to store any data;

void insert(struct node *head, int data){

	while(head->next != NULL){
		head = head->next;
		}
	struct node *last = (struct node*)malloc((sizeof(struct node)) *(sizeof(struct node) ));
	head->next = last;
	last->data = data;
	last->next = NULL;
}

 //Create a new node to store data and insert it to the end of current linked list; the head node will still be empty and data in the array in "main.c" are not stored in head node

void display (struct node *head){

	while(head->next != NULL){
		head = head->next;
		printf("%d ", head->data);
		}
}

 //print data for all nodes in the linked list except the head node (which is empty)

void deleteAll (struct node *head){

	if(head != NULL){
		deleteAll(head->next);
		free(head);
		}
}

 //delete the entire linked list including the head node

#endif

int main()
{
	int array[]={1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	struct node *head;
	head = init();   //after this statement, a linked list has been created with only a head node
	
	int i;
	for (i=0;i<10;i++) 
	{
		insert(head, array[i]);   //create a new node to store an array element. This new node is inserted to the end of current linked list
	}
	
	display(head);  //print data for all nodes in the linked list except the head node (which is empty)

	deleteAll(head);  //delete the entire linked list including the head node

	return 1;
}
