
#include "AtomicMarkableReference.h"
#include <stdatomic.h>

typedef struct node_t 
{
	int value;
	atomic_markable_reference nextDone;
	atomic_intptr_t prev;
	atomic_bool mark;
	int pushTid;
	atomic_int index;
	atomic_int counter; /* initially set to 0 */
} node_t;

typedef struct push_op_t 
{
	atomic_long phase_number;
	atomic_int pushed;
	node_t* node;
} push_op_t ;

typedef struct wait_free_stack_t
{
	
} wait_free_stack_t;

int push()
{

}

int pop()
{

}
