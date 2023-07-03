#include "Stack.h"
#include <malloc.h>

#define MAX_SIZE 100


Stack createStack()
{
    Stack* s = (Stack*)malloc(sizeof(Stack));
    if (s)
    {
        s->data = NULL;
        s->size = 0;
        s->top = -1;
    }
    return *s;

};

void push(Stack* stack , void* element) 
{

    if (stack->size  == 0)
    {
        stack->data =malloc(sizeof(void*));
    }
    else
    {
        void** temp = realloc(stack->data , sizeof(void*) * (stack->size + 1));
        if (temp == NULL)
        {
            printf("Memory allocation failed\n");
            return;
        }
        stack->data = temp;
    }
    if (stack->data)
    {
        stack->data[stack->size] = element;
    }

    if (stack->top == MAX_SIZE - 1) {
        printf("Stack Overflow\n");
        return;
    }
    stack->top++;
    stack->size++;
}

void pop(Stack* stack) {
    if (stack->top == -1) {
        printf("Stack Underflow\n");
        return; 
    }
    //free(stack->data[stack->top]);
    stack->size--;
    stack->top--;
}

void* peek(Stack* stack) {
    if (stack->top == -1) {
        printf("Stack is empty\n");
        return;
    }
    return stack->data[stack->top];
}
