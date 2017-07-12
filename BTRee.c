#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>
#define m 5

typedef struct bt_node_tag
{
    int data[m-1];
    int count;
    struct bt_node_tag* parent;
    struct bt_node_tag* down[m];
}bt_node;

void MakeAllNULL(bt_node** root);
void PrintBTreeNode(bt_node* root);
void PrintBTree(bt_node* root);
void ShiftRight(bt_node** root, int pos);
void SortBTree(bt_node** root);
bt_node* InitBTree(bt_node* root);
bool IsAllNULL(bt_node* root);
int FindPos(bt_node* root, int d);
void ShiftLeft(bt_node** root, int pos);
bt_node* SplitBTreeNode(bt_node** root, int* dptr, bt_node* ddown);
void SetParent(bt_node**root);
void PushVal(bt_node** root, int val, bt_node* valdown);
bt_node* InsertBTree(bt_node* root, int d);
bool FindNode(bt_node**root, int d);
void RemoveNode(bt_node** root,int pos);
bool SiblingHelp(bt_node** root);
void MergeBTreeNodes(bt_node** ptr, bt_node* ptr2);
void StepLeft(bt_node** root, int pos);
void ParentHelp(bt_node** root);
void SetMaxLess(bt_node** root, int delpos);
void AdjustBTreeNode(bt_node** root, int delpos);
bt_node* DeleteBTreeNode(bt_node* root, int d);
bt_node* FindRoot(bt_node* root);

void MakeAllNULL(bt_node** root)
{
    int i;
    bt_node* ptr=*root;
    ptr->parent=NULL;
    ptr->count=0;
    for(i=0;i<m;i++)
    {
        ptr->down[i]=NULL;
    }
}

void PrintBTreeNode(bt_node* root)
{
    bt_node* ptr=root;
    printf("\n");
    if(ptr==NULL)
    {
        printf("\nBTree is NULL\n");
    }
    else if(ptr->count!=0)
    {
        if(ptr->parent==NULL)
        {
            printf("\nPARENT: NULL\n");
        }
        else
        {
            printf("\nPARENT: xxxx\n");
        }
        printf("COUNT : %d\n",ptr->count);
        int i;
        for(i=0;i<ptr->count-1;i++)
        {
            printf("%d   , ",ptr->data[i]);
        }
        printf("\n");
        for(i=0;i<ptr->count;i++)
        {
            if(ptr->down[i]==NULL)
            {
                printf("NULL, ");
            }
            else
            {
                printf("xxxx, ");
            }
        }
    }
}

void PrintBTree(bt_node* root)
{
    bt_node* ptr=root;
    if(ptr!=NULL)
    {
        if(ptr->count==0)
        {
            printf("\nEmpty BTree\n");
        }
        else
        {
            int i;
            for(i=0;i<(ptr->count);i++)
            {
                PrintBTree(ptr->down[i]);
                if(i<ptr->count-1)
                {
                    printf("%d, ",ptr->data[i]);
                }
            }
        }
    }
}

void ShiftRight(bt_node** root, int pos)
{
    bt_node* ptr=*root;
    int i;
    for(i=ptr->count-3;i>=pos;i--)
    {
        ptr->data[i+1]=ptr->data[i];
    }
    for(i=ptr->count-2;i>=pos;i--)
    {
        ptr->down[i+1]=ptr->down[i];
    }
}

void SortBTree(bt_node** root)
{
    bt_node* ptr=*root;
    if(ptr->count>2)
    {
        int i;
        int val=ptr->data[ptr->count-2];
        bt_node* valdown= ptr->down[ptr->count-1];
        for(i=0;i<ptr->count-1&&val>ptr->data[i];i++)
        {
        }
        ShiftRight(&ptr,i);
        ptr->data[i]=val;
        ptr->down[i]=valdown;
    }
}

bt_node* InitBTree(bt_node* root)
{
    FILE* fptr=fopen("data.txt","r");
    fseek(fptr,0,SEEK_END);
    unsigned int len= ftell(fptr);
    if(len>0)
    {
        int d;
        rewind(fptr);
        while(!feof(fptr))
        {
            fscanf(fptr,"%d",&d);
            root=InsertBTree(root,d);
        }
    }
    fclose(fptr);
    root=FindRoot(root);
    return root;
}

bool IsAllNULL(bt_node* root)
{
    bool ret_val=1;
    bt_node* ptr=root;
    int i;
    for(i=0;i<ptr->count&&ret_val==1;i++)
    {
        if(ptr->down[i]!=NULL)
        {
            ret_val=0;
        }
    }
    return ret_val;
}

int FindPos(bt_node* root, int d)
{
    int i;
    bt_node* ptr=root;
    for(i=0;i<m-1&&ptr->data[i]<d;i++)
    {
    }
    return i;
}

void ShiftLeft(bt_node** root, int pos)
{
    bt_node* ptr=*root;
    int i,j=0;
    for(i=pos;j<ptr->count-1;i++&&j++)
    {
        ptr->data[j]=ptr->data[i];
    }
    j=0;
    for(i=pos;j<ptr->count;i++&&j++)
    {
        ptr->down[j]=ptr->down[i];
    }
}

bt_node* SplitBTreeNode(bt_node** root, int* dptr, bt_node* ddown)
{
    bt_node* ptr=*root;
    int splitpos= ceil((m-1)/2);
    int valpos= FindPos(ptr,*dptr);
    bt_node* nptr=(bt_node*)malloc(sizeof(bt_node));
    nptr->parent=ptr->parent;
    nptr->count=1;
    int i;
    for(i=0;i<splitpos;i++)
    {
        nptr->data[i]=ptr->data[i];
        nptr->down[i]=ptr->down[i];
        nptr->count=nptr->count+1;
    }
    nptr->down[i]=ptr->down[i];

    ptr->count=ptr->count-splitpos;
    ShiftLeft(&ptr,splitpos);

    if(valpos==splitpos)
    {
        ptr->down[0]=ddown;
    }
    else
    {
        if(valpos<splitpos)
        {
            nptr->data[nptr->count-1]= *dptr;
            nptr->down[nptr->count]= ddown;
            nptr->count=nptr->count+1;
            SortBTree(&nptr);
        }
        else
        {
            ptr->data[ptr->count-1]= *dptr;
            ptr->down[ptr->count]= ddown;
            ptr->count=ptr->count+1;
            SortBTree(&ptr);
        }
        *dptr=ptr->data[0];
        ptr->count=ptr->count-1;
        ShiftLeft(&ptr,1);
    }
    return nptr;
}

void SetParent(bt_node**root)
{
    bt_node* ptr=*root;
    int i;
    for(i=0;i<ptr->count;i++)
    {
        (ptr->down[i])->parent=ptr;
    }
}

void PushVal(bt_node** root, int val, bt_node* valdown)
{
    bt_node* ptr=*root;
    if(ptr->count<m)
    {
        ptr->data[ptr->count-1]=val;
        ptr->down[ptr->count]=valdown;
        ptr->count=ptr->count+1;
        SortBTree(&ptr);
    }
    else
    {
        bt_node* lptr=SplitBTreeNode(&ptr,&val,valdown);
        SetParent(&lptr);
        if(ptr->parent==NULL)
        {
            bt_node* nptr=(bt_node*)malloc(sizeof(bt_node));
            nptr->parent=NULL;
            MakeAllNULL(&nptr);
            nptr->count=2;
            nptr->data[0]=val;
            nptr->down[0]=lptr;
            nptr->down[1]=ptr;
            ptr->parent=nptr;
            lptr->parent=nptr;
        }
        else
        {
            PushVal(&(ptr->parent),val,lptr);
        }
    }
}

bt_node* InsertBTree(bt_node* root, int d)
{
    bool done=0;
    bt_node* ptr=root;
    if(ptr==NULL)
    {

        bt_node* nptr=(bt_node*)malloc(sizeof(bt_node));
        nptr->parent=NULL;
        MakeAllNULL(&nptr);
        nptr->count=2;
        nptr->data[0]=d;
        root=nptr;
    }
    else
    {
        while(ptr!=NULL&&done==0)
        {
            if(IsAllNULL(ptr))
            {
                if(ptr->count<m)
                {
                    ptr->data[ptr->count-1]=d;
                    ptr->down[ptr->count]=NULL;
                    ptr->count=ptr->count+1;
                    SortBTree(&ptr);
                }
                else
                {
                    bt_node* lptr=SplitBTreeNode(&ptr,&d,NULL);
                    if(ptr->parent==NULL)
                    {
                        bt_node* nptr=(bt_node*)malloc(sizeof(bt_node));
                        nptr->parent=NULL;
                        MakeAllNULL(&nptr);
                        nptr->count=2;
                        nptr->data[0]=d;
                        nptr->down[0]=lptr;
                        nptr->down[1]=ptr;
                        ptr->parent=nptr;
                        lptr->parent=nptr;
                    }
                    else
                    {
                        PushVal(&(ptr->parent),d,lptr);

                    }
                }
                done=1;
            }
            else
            {
                int i;
                for(i=0;i<ptr->count-1&&ptr->data[i]<d;i++)
                {
                }
                ptr=ptr->down[i];
            }
        }
    }
    while(root->parent!=NULL)
    {
        root=root->parent;
    }
    return root;
}

bool FindNode(bt_node**root, int d)
{
    bt_node* ptr=*root;
    bool found=0;
    int i;
    while(ptr!=NULL&&!found)
    {
        for(i=0;i<ptr->count-1&&d>ptr->data[i];i++)
        {
        }
        if(d==ptr->data[i])
        {
            found=1;
        }
        if(!found)
        {
            ptr=ptr->down[i];
        }
    }
    if(found)
    {
        *root=ptr;
    }
    return found;
}

void RemoveNode(bt_node** root,int pos)
{
    bt_node*ptr=*root;
    int i;
    for(i=pos;i<ptr->count-1;i++)
    {
        ptr->data[i]=ptr->data[i+1];
        ptr->down[i]=ptr->down[i+1];
    }
    ptr->down[i]=ptr->down[i+1];
    ptr->count=ptr->count-1;
}

bool SiblingHelp(bt_node** root)
{
    int limit=ceil((m+1)/2);
    bool can=0;
    bt_node* ptr=*root;
    int val=ptr->data[0];
    ptr=ptr->parent;
    int i;
    for(i=0;i<ptr->count-1&&val>ptr->data[i];i++)
    {
    }
    if(i==0)
    {
        if((ptr->down[i+1])->count>limit)
        {
            (ptr->down[i])->data[(ptr->down[i])->count-1]=ptr->data[0];
            ptr->data[0]=(ptr->down[i+1])->data[0];
            (ptr->down[i])->down[(ptr->down[i])->count]=(ptr->down[i+1])->down[0];
            (ptr->down[i])->count=(ptr->down[i])->count+1;
            (ptr->down[i+1])->count=(ptr->down[i+1])->count-1;
            ShiftLeft(&(ptr->down[i+1]),1);
            can=1;
        }
    }
    else
    {
        if(i!=ptr->count-1&&(ptr->down[i+1])->count>limit)
        {
            (ptr->down[i])->data[(ptr->down[i])->count-1]=ptr->data[i];
            ptr->data[i]=(ptr->down[i+1])->data[0];
            (ptr->down[i])->down[(ptr->down[i])->count]=(ptr->down[i+1])->down[0];
            (ptr->down[i])->count=(ptr->down[i])->count+1;
            (ptr->down[i+1])->count=(ptr->down[i+1])->count-1;
            ShiftLeft(&(ptr->down[i+1]),1);
            can=1;
        }
        else if((ptr->down[i-1])->count>limit)
        {
            (ptr->down[i])->data[(ptr->down[i])->count-1]=ptr->data[i-1];
            ptr->data[i-1]=(ptr->down[i-1])->data[(ptr->down[i-1])->count-2];
            (ptr->down[i])->down[(ptr->down[i])->count]=(ptr->down[i-1])->down[(ptr->down[i-1])->count-1];
            (ptr->down[i-1])->down[(ptr->down[i-1])->count-1]=NULL;
            (ptr->down[i])->count=(ptr->down[i])->count+1;
            (ptr->down[i-1])->count=(ptr->down[i-1])->count-1;
            can=1;
        }

    }
    return can;
}

void MergeBTreeNodes(bt_node** ptr, bt_node* ptr2)
{
    bt_node* ptr1= *ptr;
    int i, j=0;
    for(i=ptr1->count-1;j<ptr2->count-1;i++&&j++)
    {
        ptr1->data[i]=ptr2->data[j];
        ptr1->down[i]=ptr2->down[j];
        ptr1->count=ptr1->count+1;
    }
    ptr1->down[i]=ptr2->down[j];
}

void StepLeft(bt_node** root, int pos)
{
    bt_node* ptr=*root;
    int i;
    if(ptr->count>2)
    {
        for(i=pos;i<ptr->count-1;i++)
        {
            ptr->data[i]=ptr->data[i+1];

        }
        for(i=pos+1;i<ptr->count-1;i++)
        {ptr->data[i]=ptr->data[i+1];
            ptr->down[i]=ptr->down[i+1];
        }
        ptr->down[i]=ptr->down[i+1];

    }
    ptr->count=ptr->count-1;

}

void ParentHelp(bt_node** root)
{
    int limit=ceil((m+1)/2);
    bt_node* ptr=*root;
    int val=ptr->data[0];
    ptr=ptr->parent;
    int i;
    for(i=0;i<ptr->count-1&&val>ptr->data[i];i++)
    {
    }
    if(ptr->parent!=NULL)
    {
        if(i==ptr->count-1)
        {
            (ptr->down[i-1])->data[(ptr->down[i-1])->count-1]=ptr->data[i-1];
            (ptr->down[i-1])->count=(ptr->down[i-1])->count+1;
            MergeBTreeNodes(&(ptr->down[i-1]),ptr->down[i]);
            ptr->down[i+1]=NULL;
            StepLeft(&ptr,i);
            if(ptr->count<limit)
            {
                ParentHelp(&ptr);
            }
        }
        else
        {
            (ptr->down[i])->data[(ptr->down[i])->count-1]=ptr->data[i];
            (ptr->down[i])->count=(ptr->down[i])->count+1;
            MergeBTreeNodes(&(ptr->down[i]),ptr->down[i+1]);
            ptr->down[i+1]=NULL;
            StepLeft(&ptr,i);
            if(ptr->count<limit)
            {
                ParentHelp(&ptr);
            }
        }
    }
    else
    {
        if(i==ptr->count-1)
        {
            (ptr->down[i-1])->data[(ptr->down[i-1])->count-1]=ptr->data[i-1];
            (ptr->down[i-1])->count=(ptr->down[i-1])->count+1;
            MergeBTreeNodes(&(ptr->down[i-1]),ptr->down[i]);
            ptr->down[i+1]=NULL;
            StepLeft(&ptr,i);
        }
        else
        {
            (ptr->down[i])->data[(ptr->down[i])->count-1]=ptr->data[i];
            (ptr->down[i])->count=(ptr->down[i])->count+1;

            MergeBTreeNodes(&(ptr->down[i]),ptr->down[i+1]);
            ptr->down[i+1]=NULL;
            StepLeft(&ptr,i);
        }

        if(ptr->count<2)
        {
            bt_node* temp=ptr;
            ptr=ptr->down[0];
            free(temp);
            ptr->parent=NULL;
        }
    }
}

void SetMaxLess(bt_node** root, int delpos)
{
    int limit=ceil((m+1)/2);
    bt_node* ptr=*root;
    bt_node* lptr=ptr->down[delpos];
    int lpos;
    while(!IsAllNULL(lptr))
    {
        lptr=lptr->down[lptr->count-1];
    }
    lpos=lptr->count-2;
    ptr->data[delpos]=lptr->data[lpos];
    AdjustBTreeNode(&lptr,lpos);
}

void AdjustBTreeNode(bt_node** root, int delpos)
{
    int limit=ceil((m+1)/2);
    bt_node* ptr=*root;
    if((ptr->count)-1<limit)
    {

        if(ptr->parent!=NULL)
        {
            if(IsAllNULL(ptr))
            {

                RemoveNode(&ptr,delpos);
                bool done=0;
                done=SiblingHelp(&ptr);

                if(!done)
                {
                    ParentHelp(&ptr);
                }
            }
            else
            {
                SetMaxLess(&ptr, delpos);
            }
        }
        else            //root
        {
            if(IsAllNULL(ptr))
            {
                RemoveNode(&ptr,delpos);
                if(ptr->count<2)
                {
                    free(ptr);
                }
            }
            else
            {
                SetMaxLess(&ptr, delpos);
            }
        }
    }
    else
    {
        if(ptr->parent!=NULL)
        {
            if(IsAllNULL(ptr))
            {
                RemoveNode(&ptr,delpos);
            }
            else
            {
                SetMaxLess(&ptr, delpos);
            }
        }
        else            //root
        {
            if(IsAllNULL(ptr))
            {
                RemoveNode(&ptr,delpos);
                if(ptr->count<2)
                {
                    free(ptr);
                }
            }
            else
            {
                SetMaxLess(&ptr, delpos);
            }
        }
    }
}

bt_node* DeleteBTreeNode(bt_node* root, int d)
{

    bt_node* ptr=root;
    bool found=0;
    found=FindNode(&ptr,d);
    if(!found)
    {
        printf("\nData Not found\n");
    }
    else
    {
        int i;
        for(i=0;ptr->data[i]<d;i++)
        {
        }
        AdjustBTreeNode(&ptr,i);
    }
    ptr=FindRoot(ptr);
    return ptr;
}

bt_node* FindRoot(bt_node* root)
{
    bt_node*ptr=root;
    while(ptr->parent!=NULL)
    {
        ptr=ptr->parent;
    }
    return ptr;
}

void main()
{
    bt_node* root=NULL;
    root=InitBTree(root);
    PrintBTree(root);
    printf("\n\n");
    root=DeleteBTreeNode(root,6);
    PrintBTreeNode(root);
    PrintBTreeNode(root->down[1]);
    printf("\n\n");
    PrintBTree(root);
}
