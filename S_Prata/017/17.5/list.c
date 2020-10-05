/* list.c -- functions supporting list operations */
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/* local function prototype */
static void CopyToNode(Item item, Node * pnode);

/* interface functions   */
/* set the list to empty */

// инициализация списка пустым содержимым;
void InitializeList(List * plist) //plist  - адрес указателя на первый элемент в связном списке
{
    * plist = NULL; // получаем указатель на первый элемент в связном списке и инициализируем его нулем
}

/* returns true if list is empty */
bool ListIsEmpty(const List * plist)
{
    if (*plist == NULL)
        return true;
    else
        return false;
}

/* returns true if list is full */
bool ListIsFull(const List * plist)
{
    Node * pt;
    bool full;
    
    pt = (Node *) malloc(sizeof(Node));
    if (pt == NULL)
        full = true;
    else
        full = false;
    free(pt);
    
    return full;
}

/* returns number of nodes */
unsigned int ListItemCount(const List * plist)
{
    unsigned int count = 0;
    Node * pnode = *plist;    /* set to start of list */
    
    while (pnode != NULL)
    {
        ++count;
        pnode = pnode->next;  /* set to next node     */
    }
    
    return count;
}


/* ===создает узел==== для хранения элемента и добавляет его в конец списка, */  
/* указанного переменной plist (медленная реализация) */ 

// Addltem() вызывается с ==адресом== указателя на заголовок во втором аргументе, поэтому * plist — это значение указателя на заголовок. 
// Указатель на заголовок указывает на первый элемент в связном списке элементов (Item item в структуре Node).
bool AddItem(Item item, List * plist) 
// item это temp(введенные данные), plist это &movies(адрес указателя на заголовок, т.е. на  первый элемент в связном списке элементов - Item item в структуре Node)  
// добавляем введенные данные из temp(здесь item) в структуру по адресу * plist // Node * List;
// разыменованый * plist изначально проинициализирован NULL, т.е. структыра пуста;
{
    Node * pnew; // указатель на структуру
    Node * scan = *plist; // разыменовываем указатель plist получаем адрес(указатель) на заголовок, т.е на первую структуру  //  изначально * plist инициализирован NULL; 
	// plist = (List *) 0x22fe48 это адрес(т.е.указатель) указателя на первую структуру // разыменованый *plist=0x0 т.е. структура пуста // scan=0x0
	// * scan адрес структуры // ======здесь надо разобраться, нулевая структура копируется в адрес====
    
    pnew = (Node *) malloc(sizeof(Node)); //указатель на память под новую структуру // (Node *) 0x3b5b20
    if (pnew == NULL)
        return false;     /* quit function on failure  */
    
    CopyToNode(item, pnew); //копируем item(это temp) в pnew(это новая структура)
    pnew->next = NULL;
    if (scan == NULL)          /* писок пуст, поэтому поместить */
    *plist = pnew;             /* pnew в начало списка list  // копируем структуру
		                       //(*plist =(Node *)0x3b5b20) //plist = (List *) 0x22fe48  */
							  
    else
    {
		 // идем по цепочке структур, ищем конец списка т.е текущую структуру и добавляем адрес новой структуры pnew в scan->next
        while (scan->next != NULL)
            scan = scan->next;  /* find end of list    */
        scan->next = pnew;      /* add pnew to end     */
    }
    
    return true;
}

/* visit each node and execute function pointed to by pfun */
// void (* pfun)(Item item) - указательна функцию
// plist это &movies, т.е. указатель типа List - (List *) 0x22fe48
void Traverse  (const List * plist, void (* pfun)(Item item) )
{
    Node * pnode = *plist;    /* set to start of list   */
    
    while (pnode != NULL)
    {
        (*pfun)(pnode->item); /* apply function to item */ // функция выводит на экран значения для кадой структуры через printf
        pnode = pnode->next;  /* advance to next item   */ // получаем адрес следующей структуры в цепочке структур
    }
}

/* free memory allocated by malloc() */
/* set list pointer to NULL          */
// функция  EmptyTheList  () освобождает память, которая ранее была выделена с помощью malloc()
void EmptyTheList(List * plist)
{
    Node * psave;
    
    while (*plist != NULL)
    {
        psave = (*plist)->next; /* save address of next node */
        free(*plist);           /* free current node         */
        *plist = psave;         /* advance to next node      */
    }
}

/* local function definition  */
/* copies an item into a node */
static void CopyToNode(Item item, Node * pnode) // pnode это pnew
{
    pnode->item = item;  /* structure copy // банально копируем структуру */
}
