#include<stdio.h>
#include<string.h>
#include<malloc.h>
#define INPUT_LIMIT 20
#define VARIABLE_NAME_LIMIT 5

struct variable 
{
    char varName[VARIABLE_NAME_LIMIT];
    char input[INPUT_LIMIT];
    struct variable* nextVar;
};
typedef struct variable varNode;

struct scope 
{
    varNode* varHead;
    struct scope* next;
};
typedef struct scope scopeNode;

scopeNode* currentScope = NULL;

scopeNode* createNode()
{
    scopeNode *newNode;
    newNode = (scopeNode*)malloc(sizeof(scopeNode));
    newNode->next = NULL;
    newNode->varHead = NULL;
    return newNode;
}

void showScopes()
{
    scopeNode*ptr;
    ptr = currentScope;

    if(ptr==NULL)
    {
        printf("\nNothing to display\n\n");
        return;
    }

    while(ptr!=NULL)
    {
        printf("\n\nA new scope\n");
        varNode*temp;
        temp = ptr->varHead;

        while(temp!=NULL)
        {
            printf("%s %s\n",temp->varName,temp->input);
            temp=temp->nextVar;
        }
        ptr = ptr->next;
    }
}

void begin()
{
    scopeNode*temp;
    temp = createNode();
    temp->next= currentScope;
    currentScope=temp;     
    printf("\nA new scope begins\n\n");
}

void insertVar(varNode*newVar)
{
    if(currentScope->varHead==NULL) currentScope->varHead=newVar;
    
    else
    {
        varNode*ptr;
        ptr = currentScope->varHead;

        while(ptr!=NULL && strcmp(ptr->varName,newVar->varName)) ptr=ptr->nextVar;

        if(ptr!=NULL) strcpy(ptr->input,newVar->input);
        else
        {
            newVar->nextVar=currentScope->varHead;
            currentScope->varHead=newVar;
        }
    }
}

void Assign()
{
    if(currentScope==NULL)
    {
        printf("\nNo current scope... First begin a scope\n\n");
        return;
    }

    printf("\nAssign ");
    varNode* temp;
    temp =(varNode*) malloc(sizeof(varNode));
    scanf("%s %s",temp->varName,temp->input);
    temp->nextVar = NULL;

    insertVar(temp);
}

void searchVar(char *name)
{
    int found=0;
    scopeNode*ptr;
    ptr = currentScope;

    while(ptr!=NULL && !found)
    {
        varNode*temp;
        temp = ptr->varHead;
        while(temp!=NULL && strcmp(temp->varName,name)) temp=temp->nextVar;

        if(temp!=NULL) found=1;

        if(found) printf("%s\n\n",temp->input);
        else ptr=ptr->next;
    }

    if(!found) printf("No such variable in entire declaration\n\n");
}

void Print()
{
    if(currentScope==NULL)
    {
        printf("There's No scope currently... Use begin first\n\n");
        return;
    }

    char name[VARIABLE_NAME_LIMIT];
    printf("Print ");
    scanf("%s",name);

    searchVar(name);
}

void deleteScope(scopeNode*sptr)
{
   if(sptr->varHead != NULL)
   {
        varNode*temp;

        while(sptr->varHead!=NULL)
        {
            temp = sptr->varHead;
            sptr->varHead = sptr->varHead->nextVar;
            free(temp); 
        }       
   }
   free(sptr); 
}

void End()
{   
    if(currentScope==NULL)
    {
        printf("There's no scope to end\n\n");
        return;
    }

    scopeNode*temp = NULL;

    if(currentScope->next!=NULL) temp=currentScope->next;
    else if (currentScope!=NULL) deleteScope(currentScope);

    currentScope = temp;
    printf("\nThe current scope terminates\n\n");
}

void show_menu()
{
    int option;
    do
    {
        printf("\n1 - Begin \n2 - Assign \n3 - Print \n4 - End \n5 - Display variables in scope \nPress any other key to exit : ");
        scanf("%d",&option);

        switch (option)
        {
            case 1:
                begin();
                break;
            
            case 2:
                Assign();
                break;          
            
            case 3:
                Print();
                break;

            case 4:
                End();
                break;

            case 5:
                showScopes();
                break;

            default:
                printf("\n\nFunction terminates...");
                break;
        }
    } 
    while (option>0 && option<6);
}

int main()
{
    show_menu();
    return 0;
}