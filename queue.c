// Muhammed Ali Doğan
// 150115035
// Project#3

#ifndef queue
#define queue

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define SIZE 40

typedef struct qu{
    int items[SIZE];
    int front;
    int rear;
}qu;

qu* build_queue();
void enqueue(qu *q, int value);
int dequeue(qu *q);
void display(qu *q);
int is_empty(qu *q);
void print_queue(qu *q);

#endif

qu* build_queue()
{
    qu* q = malloc(sizeof(qu));
    q->front = -1;
    q->rear = -1;
    return q;
}
int is_empty(qu* q)
{
    if(q->rear == -1) 
        return 1;
    else 
        return 0;
}
void enqueue(qu* q, int value)
{
    if(q->rear == SIZE-1)
        printf("\nQueue is Full!!");
    else {
        if(q->front == -1)
            q->front = 0;
        q->rear++;
        q->items[q->rear] = value;
    }
}
int dequeue(qu* q)
{
    int item;
    if(is_empty(q)){
        printf("Queue is empty");
        item = -1;
    }
    else{
        item = q->items[q->front];
        q->front++;
        if(q->front > q->rear){
            //printf("Resetting queue");
            q->front = q->rear = -1;
        }
    }
    return item;
}
void print_queue(qu *q)
{
    int i = q->front;

    if(is_empty(q)) {
        printf("Queue is empty");
    } else {
        //printf("\nQueue contains \n");
        for(i = q->front; i < q->rear + 1; i++) {
                printf("%d ", q->items[i]);
        }
    }    
}