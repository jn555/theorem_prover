//#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define TABLE_SIZE 10
#define MAX_NAME 256

typedef struct person {
    char name[MAX_NAME];
    int age;
    struct person* next;
} person;

person* hashtable[TABLE_SIZE];

unsigned int hash (char *name)
{
    int length = strnlen(name, MAX_NAME);
    unsigned int hash_value = 0;
    for (int i = 0; i < length; i++)
    {
        hash_value += name[i];
        hash_value = (hash_value * name[i]) % TABLE_SIZE;
    }
    return hash_value;
}

void init_hash_table()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        hashtable[i] = NULL;
    }
}

void print_table()
{
    printf("Start\n");
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (hashtable[i] == NULL)
        {
            printf("\t%i\n", i);
        }
        else 
        {
            person* temp = hashtable[i];
            printf("\t%i\t", i);
            while (temp != NULL)
            {
                printf("%s\t", temp->name);
                temp = temp->next;
            }
            printf("\n");
        }
    }
    printf("End\n");
}

bool insert(person* p)
{
    if (p == NULL) return false;
    int index = hash(p->name);
    if (hashtable[index] == NULL)
    {
        hashtable[index] = p;
        return true;
    }
    else
    {
        //iterate to end of LL
        person* temp = hashtable[index];
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = p;
        return true;
    }
}

person* hashtable_lookup(char* name)
{
    int index = hash(name);
    if (hashtable[index] == NULL)
    {
        return NULL;
    }
    if (strcmp(hashtable[index]->name, name) == 0)
    { 
        return hashtable[index];
    }
    else 
    {
        //linear search
        person* temp = hashtable[index]->next;
        while (temp != NULL)
        {
            if (strcmp(temp->name, name) == 0)
            {
                return temp;
            }
            temp = temp->next;
        }
        return NULL;
    }
}

person* hashtable_delete(char* name)
{
    int index = hash(name);
    if (hashtable[index] == NULL)
    {
        return NULL;
    }
    else if (strcmp(hashtable[index]->name, name) == 0)
    {
        person* temp = hashtable[index];
        hashtable[index] = temp->next;
        return temp; //return to caller so it can free the pointer if heap allocated
    }
    else
    {
        //iterate to end
        person* prev = hashtable[index];
        person* curr = prev->next;

        while (curr != NULL)
        {
            if (strcmp(curr->name, name) == 0)
            {
                prev->next = curr->next;
                return curr;                
            }
            prev = curr;
            curr = curr->next;
        }
        return NULL;
    }
}

int main()
{
    init_hash_table();
    person jacob = {.name = "Jacob", .age=10};
    person darius = {.name = "Darius", .age=10};
    person john = {.name = "John", .age=10};
    person bob = {.name = "Bob", .age=10};
    person bob1 = {.name = "Bob", .age=10};
    insert(&jacob);
    insert(&darius);
    insert(&john);
    insert(&bob);
    insert(&bob1);
    print_table();
    person* yuhh = hashtable_lookup("Darius");
    printf("%s", yuhh->name);
    hashtable_delete("Darius");
    print_table();
}