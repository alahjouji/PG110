#ifndef _LIST_H_
#define _LIST_H_
//-------------------------------------------------------------------------------------------------



// Set up of the list
extern struct list* 	list_new (  );
extern struct list* 	list_add_tail ( struct list* list , void * ptr );

// Free of the list
extern void 	list_free ( struct list* list );
extern void		list_free_element ( struct list* list , int position );
extern void 	list_free_element_head ( struct list* list );
extern void 	list_free_element_tail ( struct list* list );

// Display the list
extern void 	list_display ( struct list* list);

// List get
extern int		list_get_length(struct list* list);
extern void * 	list_get_value ( struct list* list , int position);
extern void * 	list_get_tail_value ( struct list* list);

//-------------------------------------------------------------------------------------------------
#endif // _LIST_H_
//-------------------------------------------------------------------------------------------------
// End Of File
