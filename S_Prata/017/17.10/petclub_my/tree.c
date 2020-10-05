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
    ptree->root = NULL; // значение root - 0х0, ptree - (Tree *) 0x22fe30
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

// интерфейс
bool AddItem(const Item * pi, Tree * ptree) //Item pi источник ввода - ptree принимающая структура
{
    Trnode * new_node;
    Pair look;

    if  (TreeIsFull(ptree))
    {
        fprintf(stderr,"Tree is full\n");
        return false;             /* early return           */
    }

/* SeekItem возвращает структуру Pair
typedef struct pair {
    Trnode * parent;
    Trnode * child;
} Pair;
*/  // SeekItem возвращает структуру Pair

look = SeekItem(pi, ptree);
    if (look.child != NULL) // pi - структура типа Item(элемент - имя, вид), уже заполнена введеннми данными
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

   // if (ptree->root == NULL)      /* добавление узла к дереву  */
   //     ptree->root = new_node;  //значение ptree->root адрес(на первой итерации - (Trnode *) 0x885b10) /* новый уэел будет в дереве корневым */
   //else // попробовать сразу присвоить адрес нвого узла new_node узлу look.parent->left или Right возвращаемого функцией SeekItem
   //        AddNode(new_node,ptree->root); /* добавить новый узел в дерево, адрес  */
if (ptree->root == NULL){
    ptree->root = new_node;
}
else{ // надо сделать так, чтобы функция SeekItem возвращала структуру в которой будет признак
      // левого или правого поддерева
      // а также указатель на родительский узел

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
    Pair look; // локальный тип данных

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

// * pfun это указательна функцию void printitem(Item item)
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


static void AddNode (Trnode * new_node, Trnode * root)//новая нода и корневая нода
{
    if (ToLeft(&new_node->item, &root->item)) //новая нода и корневая нода
    {
        if (root->left == NULL)      /* empty subtree       */
            root->left = new_node;   /* so add node here    */
        else // а что если воспользоваться результатами поиска функции SeekItem и сразу присвоить адрес новой ноды найденному узлу у которого Left или Right равен NULL, не прибегая к рекурсии?
            AddNode(new_node, root->left);/* рекурсивный вызов AddNode продвижение по дереву, по адресам узлов*/
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
//i1 искомый элемент типа Item, i2 существующий элемент типа Item
{
/*
strcmpC'A", "А") возвращает 0
strcmpC'A", "В") возвращает -1
strcmpC'B", "А") возвращает 1
*/
    int comp1;
    if ((comp1 = strcmp(i1->petname, i2->petname)) < 0) //i1->petname искомое значение элемента типа Item, i2->petname существующее значение элемента типа Item
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
strcmpC'A", "А") возвращает 0
strcmpC'A", "В") возвращает -1
strcmpC'B", "А") возвращает 1
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
    /* ================определение в tree.h
    typedef struct trnode
    {    Item item;
         struct trnode * left;  // pointer to right branch
         struct trnode * right; // pointer to left branch
    } Trnode; =========*/

    Trnode * new_node;
    // Создание нового узла и копирование в него элемента

    new_node = (Trnode *) malloc(sizeof(Trnode)); //  приводим результат к указателю Trnode * // адрес ноды (Trnode *) 0x885b80
    if (new_node != NULL)
    {
        // Копируем элемент в новый узел
        new_node->item = *pi; // извлекаем значения структуры Item pi , pi ранее уже заполнена введеннми данными
        new_node->left = NULL; // добавляем left = NULL в новую ноду
        new_node->right = NULL;// добавляем right = NULL в новую ноду
    }
    return new_node;
}

/*
typedef struct pair {
    Trnode * parent;
    Trnode * child; //Trnode это: Item item; struct trnode * left; struct trnode * right;
} Pair;
*/
static Pair SeekItem(const Item * pi, const Tree * ptree) //pi источник ввода - ptree принимающая структура(дерево)
{
    Pair look; // вспомогательная структура для поиска, просто в ===текщую итерацию=== указывает адрес родительского и дочернего узла
    look.parent = NULL; // не является родителем
    // структура Tree * ptree это Trnode * root; и int size; , а Trnode это Item item; , struct trnode * left; и struct trnode * right;
    look.child = ptree->root; // значение рутового узла - указатель  (изначально инициализирован значением NULL)

    if (look.child == NULL) // если значение ptree->root узла принимающей структуры(дерева) равен NULL
        return look;                        /* early return   */

    while (look.child != NULL) // цикл начинаем с корня дерева
    {   // значение look.child->item - адрес (Item *) 0x885b10 как и у look.child
        if (ToLeft(pi, &(look.child->item))) // Item *pi (имя, вид) - источник ввода
                                             // ToLeft возращает либо nrue(если значение petname pi меньше petname item ) либо false в обратном случае
        {
            look.parent = look.child; // на первой итерации (Item *) 0x885b10
            look.child = look.child->left; // продвижение поиска влево (работаем с адресом значения слева look.child->left)
            look.left = true;
        }
        else if (ToRight(pi, &(look.child->item)))
        {
            look.parent = look.child; //
            look.child = look.child->right;
            look.left = false;
        }
        else       /*если элемент не расположен ни слева,
                   ни справа, он должен быть таким же     */
            break; /* look.child - это адрес узла, содержащего элемент  */
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
        // (*ptr) - это адрес удаляемого узла // проверяем temp->right т.е. для узла слева проверяем узел справа
        for (temp = (*ptr)->left; temp->right != NULL; //
             temp = temp->right)// теперь temp и (*ptr)->left указывают на один адрес,
            continue;
        temp->right = (*ptr)->right;// к временному узлу temp с адресом (*ptr)->left, добавили адрес правой подветви
        temp = *ptr;
        *ptr =(*ptr)->left; // здесь узел с адресом (*ptr)->left уже имеет присоединенную правую ветвь (через temp)
                            // с адресом (*ptr)->right, если например у нового *ptr не будет своего левого узла,
                            // тогда (*ptr)->left обнулится
        free(temp);
    }
}

