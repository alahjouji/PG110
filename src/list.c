// Library
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <SDL/SDL_image.h>

#include "../include/list.h"
//-------------------------------------------------------------------------------------------------

struct elt
{
	struct elt * next;
	struct elt * prev;
	void * element;
};

struct list
{
  struct elt * head;
  struct elt * tail;
  int length;
  void (*free)(void *val);
};

struct list* list_new ( void (*ptr) )
{
	struct list* list= malloc(sizeof(*list));
	assert(list);

	list->head=NULL;
	list->tail=NULL;
	list->length=0;
	list->free=ptr;

	return list;
}


struct list* list_add_tail ( struct list* list , void * ptr )
{
	assert(list);
	assert(ptr);
	struct elt* new_elt = malloc(sizeof(*new_elt));
	assert(new_elt);
	struct elt* temp = malloc(sizeof(*temp));
	assert(temp);

    if ( list->head == NULL && list->tail == NULL)
    {
    	new_elt->element=ptr;
    	new_elt->next=NULL;
    	new_elt->prev=NULL;
    	list->head=new_elt;
    	list->tail=new_elt;
    	list->length=1;

    	return list;
    }

    new_elt->element=ptr;
	new_elt->next=NULL;
	new_elt->prev=list->tail;
	temp=list->head;
	while(temp->next!=NULL)
	{
		temp=temp->next;
	}
	temp->next=new_elt;
	list->tail=new_elt;
	list->length++;

    return list;
}


void * list_get_value ( struct list* list , int position)
{
	assert(list);
	struct elt* elt = malloc(sizeof(*elt));
	assert(elt);
	elt=list->head;
	for (int i=1 ; i < position ; i++)
		elt = elt->next;

	return elt->element;
}

void * list_get_tail_value ( struct list* list )
{
	assert(list);
	return list->tail->element;
}

void list_free( struct list* list )
{
	assert(list);
	int length = list->length;
	struct elt* next;
	struct elt* curr = list->head;
	while (length--)
	{
		next = curr->next;
		if (list->free)
			list->free(curr->element);
		free(curr);
		curr = next;
	}
	free(list);

}

int list_get_length(struct list* list)
{
	assert(list);
	return list->length;
}

void list_free_element_head(struct list* list)
{
	assert(list);
	struct elt* temp;
	temp=list->head->next; // second element of the list
	temp->prev=NULL;
	list->free(list->head->element);
	list->head=temp;
	list->length--;

}

void list_free_element_tail(struct list* list)
{
	assert(list);
	struct elt* temp;
	temp=list->tail->prev;
	temp->next=NULL;
	list->free(list->tail->element);
	list->tail=temp;
	list->length--;
}

void list_free_element ( struct list* list , int position )
{
	assert(list);
	if (list->length < 1 || position < 1 || position > list->length)
	{
		printf("[ERROR] list_rm_element : list's length or position\n");
		return;
	}
	if (list->length == 1 )
	{
		list->free(list->head->element);
		list->head=NULL;
		list->tail=NULL;
		list->length=0;
		return;
	}
	if (position == 1 )
	{
		list_free_element_head(list);
		return;
	}
	if (position == list->length )
	{
		list_free_element_tail(list);
		return;
	}
	struct elt* temp;
	temp=list->head;
	for(int i=1;i<position;i++)
		temp=temp->next;
	temp->next->prev = temp->prev;
	temp->prev->next = temp->next;
	list->free(temp->element);
	free(temp);
	list->length--;
}
//-------------------------------------------------------------------------------------------------
// End Of File
