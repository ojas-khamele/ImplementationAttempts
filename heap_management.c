#include<stdio.h>
#include<malloc.h>
#include<string.h>

#define MEM_LIMIT 20000


enum DataTypes
{
    NONE,
    INTEGER,
    DECIMAL,
    STRING
};

union takeInput
{
    int integer;
    float decimal;
    char word[20];
};
typedef union takeInput input_tag;

struct ListNode
{
    int size;
    void* vptr;
    int item_type;
    struct ListNode* next;
    int allocation_number;
};
typedef struct ListNode Node;

struct FreeList 
{
    void* address;
    int size;
    struct FreeList* next;
};
typedef struct FreeList FreeData_node;


FreeData_node* Create_free_node()
{
    FreeData_node*new_node;
    new_node=(FreeData_node*)malloc(sizeof(FreeData_node));
    new_node->next=NULL;

    return new_node;
}

FreeData_node* Insert_in_freeList(FreeData_node*head,FreeData_node*new_node)
{
    if(head==NULL) head=new_node; 
    else
    {
        FreeData_node*ptr;
        ptr=head;

        while(ptr->next!=NULL) ptr=ptr->next;

        ptr->next=new_node;
    }
    return head;
}

void show_free_list(FreeData_node*ptr)
{
    while(ptr!=NULL)
    {
        printf("%d ",ptr->size);
        ptr=ptr->next;
    }
}


Node* Create_new_node(int size,void*ptr,int item_type)
{
    static int allocNum=1;
    Node*newptr;
    newptr=(Node*)malloc(sizeof(Node));
    newptr->next=NULL;

    newptr->size=size;
    newptr->item_type=item_type;
    newptr->vptr=ptr;
    newptr->allocation_number=allocNum;
    
    allocNum++;

    return newptr;
}

Node* Insert_in_MetaData(Node*head,Node*new_node)
{
    Node*ptr;
    ptr=head;
    
    if(head!=NULL)
    {
        while(ptr!=NULL && ptr->next!=NULL) ptr=ptr->next;
        ptr->next=new_node;
    }

    else head=new_node;
    
    return head;
}

void visit(Node*ptr)
{
    switch(ptr->item_type)
    {
        case INTEGER:if(1);
        int *intptr;
        intptr=(int*)ptr->vptr;
        printf("%d ",*intptr);
        break;

        case DECIMAL:if(1);
        float *floatptr;
        floatptr=(float*)ptr->vptr;
        printf("%f ",*floatptr);
        break;

        case STRING:if(1);
        char *wordptr;
        wordptr=(char*)ptr->vptr;
        printf("%s ",wordptr);
    }
}

void show_MetaData(Node*head)
{
    while(head!=NULL)
    {
        visit(head);
        printf("\n%d is size %d is type\n",head->size,head->item_type);
    
        head=head->next;
    }
}


void delete_freespace(FreeData_node**freeData,void*allocated_space)
{
    FreeData_node*temp,*prev;
    temp=*freeData;

    while(temp!=NULL && temp->address!=allocated_space)
    {
        prev=temp;
        temp=temp->next;
    }

    if(temp==*freeData && temp!=NULL) *freeData=(*freeData)->next;
    else if(temp!=NULL) prev->next=temp->next;
    else printf("Something went wrong\n");
}

int space_in_free(FreeData_node**freeData,void**ptr,int size)
{
    int assigned=0;

    if(*freeData!=NULL)
    {
        while(*freeData!=NULL && (*freeData)->size < size) *freeData=(*freeData)->next;
        if(*freeData!=NULL)
        {
            assigned=1;
            *ptr=(*freeData)->address;
            //printf("Allocation in a hole\n");
        }
    }
    return assigned;
}

void Allocate(void**heap_ptr,int*index,Node**metaData,FreeData_node**freeData)
{
    int dataType;
    input_tag val;
    int allocated_size;
    void* allocated_address;
    
    printf("The data types are indexed as follows\n1-Integer\n2-Float\n3-String\n\n");
    printf("Enter your data type : ");
    scanf("%d",&dataType);

    printf("Now enter the data : ");
        
    switch(dataType)
    {
        case INTEGER: scanf("%d",&val.integer);
        int*intptr;
        intptr=(int*)*heap_ptr;

        if(*index+sizeof(int)<=MEM_LIMIT) intptr[*index]=val.integer;
        else printf("Heap Buffer Overflow\n\n");

        allocated_size=sizeof(int);
        allocated_address=intptr;
        break;
            
        case DECIMAL: scanf("%f",&val.decimal);
        float*floatptr;
        floatptr=(float*)*heap_ptr;

        if(*index+sizeof(float)<=MEM_LIMIT) floatptr[*index]=val.decimal;
        else printf("Heap Buffer Overflow\n\n");

        allocated_size=sizeof(float);
        allocated_address=floatptr;
        break;
            
        case STRING: scanf("%s",val.word);
        char*wordptr;
        wordptr=(char*)*heap_ptr;

        if(*index+strlen(val.word)<=MEM_LIMIT) wordptr=val.word;
        else printf("Heap Buffer Overflow\n\n");

        allocated_size=strlen(val.word);
        allocated_address=wordptr;
        break;
            
        default: printf("Wrong input\n"); 
    }

    void*hole_address;
    if(space_in_free(freeData,&hole_address,allocated_size))
    {
        *index=*index-allocated_size;
        allocated_address=hole_address;
        delete_freespace(freeData,hole_address);
    }

    Node*new_node;
    new_node=Create_new_node(allocated_size,allocated_address,dataType);
    *metaData=Insert_in_MetaData(*metaData,new_node);
        
    *index=*index+allocated_size;
}

Node* Deallocate(Node*head,FreeData_node**freeData)
{
    int allocNo;
    
    printf("Enter the allocation number that needs to be deallocated : ");
    scanf("%d",&allocNo);

    Node*ptr,*prev;
    ptr=head;
    while(ptr!=NULL && ptr->allocation_number!=allocNo)
    {
        prev=ptr;
        ptr=ptr->next;
    }

    if(ptr==NULL) printf("No such allocation exists\n");
    else
    {
        // printf("Allocation details : ");
        // printf("%d is item type\n",ptr->item_type);
        ptr->item_type=NONE;
       
        FreeData_node*temp;
        temp=Create_free_node();
        temp->address=ptr->vptr;
        temp->size=ptr->size;
        
        *freeData=Insert_in_freeList(*freeData,temp);
    }

    if(head==ptr && head!=NULL) head=head->next;
    else if (ptr!=NULL) prev->next=ptr->next;

    return head;
}


int main()
{
    void*heap_ptr;
    heap_ptr=(void*)malloc(sizeof(char)*MEM_LIMIT);

    Node*metaData;
    metaData=NULL;

    FreeData_node*freeData;
    freeData=NULL;

    int index=0;

    int opt;
    do
    {
        printf("Enter:\n1 to Allocate\n0 to Deallocate\nAny other key to exit\n");
        scanf("%d",&opt);
        if(opt==1)Allocate(&heap_ptr,&index,&metaData,&freeData);
        else if (opt==0) metaData=Deallocate(metaData,&freeData);
    }
    while(opt==0 || opt==1);

    //show_MetaData(metaData);
    //show_free_list(freeData);
    return 0;
}

