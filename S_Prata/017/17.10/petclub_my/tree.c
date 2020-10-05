/* tree.c -- tree support functions */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

/* local data type */
typedef struct pair {
    Trnode * parent;
    Trnode * child;
    bool left;
} Pair;

/* protototypes for local functions */
static Trnode * MakeNode(const Item * pi);
static bool ToLeft(const Item * i1, const Item * i2);
static bool ToRight(const Item * i1, const Item * i2);
static void AddNode (Trnode * new_node, Trnode * root);
static void InOrder(const Trnode * root, void (* pfun)(Item item));
static Pair SeekItem(const Item * pi, const Tree * ptree);
static void DeleteNode(Trnode **ptr);
static void DeleteAllNodes(Trnode * ptr);

/* function definitions */
void InitializeTree(Tree * ptree)
{
    ptree->root = NULL; // �������� root - 0�0, ptree - (Tree *) 0x22fe30
    ptree->size = 0;
}

bool TreeIsEmpty(const Tree * ptree)
{
    if (ptree->root == NULL)
        return true;
    else
        return false;
}

bool TreeIsFull(const Tree * ptree)
{
    if (ptree->size == MAXITEMS)
        return true;
    else
        return false;
}

int TreeItemCount(const Tree * ptree)
{
    return ptree->size;
}

// ���������
bool AddItem(const Item * pi, Tree * ptree) //Item pi �������� ����� - ptree ����������� ���������
{
    Trnode * new_node;
    Pair look;

    if  (TreeIsFull(ptree))
    {
        fprintf(stderr,"Tree is full\n");
        return false;             /* early return           */
    }

/* SeekItem ���������� ��������� Pair
typedef struct pair {
    Trnode * parent;
    Trnode * child;
} Pair;
*/  // SeekItem ���������� ��������� Pair

look = SeekItem(pi, ptree);
    if (look.child != NULL) // pi - ��������� ���� Item(������� - ���, ���), ��� ��������� ��������� �������
    {
        fprintf(stderr, "Attempted to add duplicate item\n");
        return false;             /* early return           */
    }
    new_node = MakeNode(pi);      /* points to new node     */
    if (new_node == NULL)
    {
        fprintf(stderr, "Couldn't create node\n");
        return false;             /* early return           */
    }
    /* succeeded in creating a new node */
    ptree->size++;

   // if (ptree->root == NULL)      /* ���������� ���� � ������  */
   //     ptree->root = new_node;  //�������� ptree->root �����(�� ������ �������� - (Trnode *) 0x885b10) /* ����� ���� ����� � ������ �������� */
   //else // ����������� ����� ��������� ����� ����� ���� new_node ���� look.parent->left ��� Right ������������� �������� SeekItem
   //        AddNode(new_node,ptree->root); /* �������� ����� ���� � ������, �����  */
if (ptree->root == NULL){
    ptree->root = new_node;
}
else{ // ���� ������� ���, ����� ������� SeekItem ���������� ��������� � ������� ����� �������
      // ������ ��� ������� ���������
      // � ����� ��������� �� ������������ ����

    if (look.left == true){
      look.parent->left = new_node;
    }
    else {
      look.parent->right = new_node;
    }
}
    return true;                  /* successful return      */
}

bool InTree(const Item * pi, const Tree * ptree)
{
    return (SeekItem(pi, ptree).child == NULL) ? false : true;
}

bool DeleteItem(const Item * pi, Tree * ptree)
{
    Pair look; // ��������� ��� ������

    look = SeekItem(pi, ptree);
    if (look.child == NULL)
        return false;

    if (look.parent == NULL)      /* delete root item       */
        DeleteNode(&ptree->root);
    else if (look.parent->left == look.child)
        DeleteNode(&look.parent->left);
    else
        DeleteNode(&look.parent->right);
    ptree->size--;

    return true;
}

// * pfun ��� ����������� ������� void printitem(Item item)
void Traverse (const Tree * ptree, void (* pfun)(Item item))
{

    if (ptree != NULL)
        InOrder(ptree->root, pfun);
}

void DeleteAll(Tree * ptree)
{
    if (ptree != NULL)
        DeleteAllNodes(ptree->root);
    ptree->root = NULL;
    ptree->size = 0;
}


/* local functions */
static void InOrder(const Trnode * root, void (* pfun)(Item item))
{
    if (root != NULL)
    {
        InOrder(root->left, pfun);
        (*pfun)(root->item);
        InOrder(root->right, pfun);
    }
}

static void DeleteAllNodes(Trnode * root)
{
    Trnode * pright;

    if (root != NULL)
    {
        pright = root->right;
        DeleteAllNodes(root->left);
        free(root);
        DeleteAllNodes(pright);
    }
}


static void AddNode (Trnode * new_node, Trnode * root)//����� ���� � �������� ����
{
    if (ToLeft(&new_node->item, &root->item)) //����� ���� � �������� ����
    {
        if (root->left == NULL)      /* empty subtree       */
            root->left = new_node;   /* so add node here    */
        else // � ��� ���� ��������������� ������������ ������ ������� SeekItem � ����� ��������� ����� ����� ���� ���������� ���� � �������� Left ��� Right ����� NULL, �� �������� � ��������?
            AddNode(new_node, root->left);/* ����������� ����� AddNode ����������� �� ������, �� ������� �����*/
    }
    else if (ToRight(&new_node->item, &root->item))
    {
        if (root->right == NULL)
            root->right = new_node;
        else
            AddNode(new_node, root->right);
    }
    else                         /* should be no duplicates */
    {
        fprintf(stderr,"location error in AddNode()\n");
        exit(1);
    }
}

static bool ToLeft(const Item * i1, const Item * i2)
//i1 ������� ������� ���� Item, i2 ������������ ������� ���� Item
{
/*
strcmpC'A", "�") ���������� 0
strcmpC'A", "�") ���������� -1
strcmpC'B", "�") ���������� 1
*/
    int comp1;
    if ((comp1 = strcmp(i1->petname, i2->petname)) < 0) //i1->petname ������� �������� �������� ���� Item, i2->petname ������������ �������� �������� ���� Item
        return true;
    else if (comp1 == 0 &&
             strcmp(i1->petkind, i2->petkind) < 0 )
        return true;
    else
        return false;
}

static bool ToRight(const Item * i1, const Item * i2)
{
/*
strcmpC'A", "�") ���������� 0
strcmpC'A", "�") ���������� -1
strcmpC'B", "�") ���������� 1
*/
    int comp1;

    if ((comp1 = strcmp(i1->petname, i2->petname)) > 0)
        return true;
    else if (comp1 == 0 &&
             strcmp(i1->petkind, i2->petkind) > 0 )
        return true;
    else
        return false;
}

static Trnode * MakeNode(const Item * pi)
{
    /* ================����������� � tree.h
    typedef struct trnode
    {    Item item;
         struct trnode * left;  // pointer to right branch
         struct trnode * right; // pointer to left branch
    } Trnode; =========*/

    Trnode * new_node;
    // �������� ������ ���� � ����������� � ���� ��������

    new_node = (Trnode *) malloc(sizeof(Trnode)); //  �������� ��������� � ��������� Trnode * // ����� ���� (Trnode *) 0x885b80
    if (new_node != NULL)
    {
        // �������� ������� � ����� ����
        new_node->item = *pi; // ��������� �������� ��������� Item pi , pi ����� ��� ��������� ��������� �������
        new_node->left = NULL; // ��������� left = NULL � ����� ����
        new_node->right = NULL;// ��������� right = NULL � ����� ����
    }
    return new_node;
}

/*
typedef struct pair {
    Trnode * parent;
    Trnode * child; //Trnode ���: Item item; struct trnode * left; struct trnode * right;
} Pair;
*/
static Pair SeekItem(const Item * pi, const Tree * ptree) //pi �������� ����� - ptree ����������� ���������(������)
{
    Pair look; // ��������������� ��������� ��� ������, ������ � ===������ ��������=== ��������� ����� ������������� � ��������� ����
    look.parent = NULL; // �� �������� ���������
    // ��������� Tree * ptree ��� Trnode * root; � int size; , � Trnode ��� Item item; , struct trnode * left; � struct trnode * right;
    look.child = ptree->root; // �������� �������� ���� - ���������  (���������� ��������������� ��������� NULL)

    if (look.child == NULL) // ���� �������� ptree->root ���� ����������� ���������(������) ����� NULL
        return look;                        /* early return   */

    while (look.child != NULL) // ���� �������� � ����� ������
    {   // �������� look.child->item - ����� (Item *) 0x885b10 ��� � � look.child
        if (ToLeft(pi, &(look.child->item))) // Item *pi (���, ���) - �������� �����
                                             // ToLeft ��������� ���� nrue(���� �������� petname pi ������ petname item ) ���� false � �������� ������
        {
            look.parent = look.child; // �� ������ �������� (Item *) 0x885b10
            look.child = look.child->left; // ����������� ������ ����� (�������� � ������� �������� ����� look.child->left)
            look.left = true;
        }
        else if (ToRight(pi, &(look.child->item)))
        {
            look.parent = look.child; //
            look.child = look.child->right;
            look.left = false;
        }
        else       /*���� ������� �� ���������� �� �����,
                   �� ������, �� ������ ���� ����� ��     */
            break; /* look.child - ��� ����� ����, ����������� �������  */
    }

    return look;                       /* successful return   */
}

static void DeleteNode(Trnode **ptr)
/* ptr is address of parent member pointing to target node  */
{
    Trnode * temp;

    if ( (*ptr)->left == NULL)
    {
        temp = *ptr;
        *ptr = (*ptr)->right;
        free(temp);
    }
    else if ( (*ptr)->right == NULL)
    {
        temp = *ptr;
        *ptr = (*ptr)->left;
        free(temp);
    }
    else    /* deleted node has two children */
    {
        /* find where to reattach right subtree */
        // (*ptr) - ��� ����� ���������� ���� // ��������� temp->right �.�. ��� ���� ����� ��������� ���� ������
        for (temp = (*ptr)->left; temp->right != NULL; //
             temp = temp->right)// ������ temp � (*ptr)->left ��������� �� ���� �����,
            continue;
        temp->right = (*ptr)->right;// � ���������� ���� temp � ������� (*ptr)->left, �������� ����� ������ ��������
        temp = *ptr;
        *ptr =(*ptr)->left; // ����� ���� � ������� (*ptr)->left ��� ����� �������������� ������ ����� (����� temp)
                            // � ������� (*ptr)->right, ���� �������� � ������ *ptr �� ����� ������ ������ ����,
                            // ����� (*ptr)->left ���������
        free(temp);
    }
}

