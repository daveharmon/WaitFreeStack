
#include "atomicmarkablereference.h"
#include <stdatomic.h>
#include <stdlib.h>

typedef struct atomic_markable_reference_t
{
	atomic_intptr_t ref;
	atomic_int mark;
} atomic_markable_reference_t;

/* -------------------------- Public Functions --------------------------- */

atomic_markable_reference* atomic_markable_reference_init(void* init_obj, int init_mark)
{
	atomic_markable_reference_t* m_ref = malloc(sizeof(atomic_markable_reference_t));

	atomic_init(&m_ref->ref, (intptr_t)init_obj);
	atomic_store(&m_ref->mark, init_mark);

	return (atomic_markable_reference*) m_ref;
}

void atomic_markable_reference_destroy(atomic_markable_reference* m_ref)
{
	free(m_ref);
}

int attempt_mark(atomic_markable_reference* m_ref, 
	int expected_mark, int new_mark)
{
	atomic_markable_reference_t* r = (atomic_markable_reference_t*)m_ref;
	return atomic_compare_exchange_strong(&r->mark, &expected_mark, new_mark);
}

int compare_and_set(atomic_markable_reference* m_ref, void* expected_ref, 
	void* new_ref, int expected_mark, int new_mark)
{
	atomic_markable_reference_t* r = (atomic_markable_reference_t*)m_ref;
	atomic_thread_fence(memory_order_acquire);
	// calling &expected_ref works, but compiler won't shut up about warnings
	intptr_t e_ref = (intptr_t)expected_ref;
	int rc = atomic_compare_exchange_strong(&r->ref, &e_ref, (intptr_t)new_ref);
	if (rc)
		atomic_compare_exchange_strong(&r->mark, &expected_mark, new_mark);
	atomic_thread_fence(memory_order_release);
	return rc;
}

void* get(atomic_markable_reference* m_ref, int* mark_value)
{
	atomic_markable_reference_t* r = (atomic_markable_reference_t*)m_ref;
	atomic_thread_fence(memory_order_acquire);
	void* rc = (void*)atomic_load(&r->ref);
	*mark_value = atomic_load(&r->mark);
	atomic_thread_fence(memory_order_release);
	return rc;
}

void* get_reference(atomic_markable_reference* m_ref)
{
	atomic_markable_reference_t* r = (atomic_markable_reference_t*)m_ref;
	return (void*)atomic_load(&r->ref);
}

int is_marked(atomic_markable_reference* m_ref)
{
	atomic_markable_reference_t* r = (atomic_markable_reference_t*)m_ref;
	return atomic_load(&r->mark);
}

void set(atomic_markable_reference* m_ref, void* new_ref, int new_mark)
{
	atomic_markable_reference_t* r = (atomic_markable_reference_t*)m_ref;
	atomic_thread_fence(memory_order_acquire);
	atomic_store(&r->ref, (intptr_t)new_ref);
	atomic_store(&r->mark, new_mark);
	atomic_thread_fence(memory_order_release);
}

int weak_compare_and_set(atomic_markable_reference* m_ref, void* expected_ref, 
	void* new_ref, int expected_mark, int new_mark)
{
	atomic_markable_reference_t* r = (atomic_markable_reference_t*)m_ref;
	atomic_thread_fence(memory_order_acquire);
	intptr_t e_ref = (intptr_t)expected_ref;
	int rc = atomic_compare_exchange_weak(&r->ref, &e_ref, (intptr_t)new_ref);
	if (rc)
		atomic_compare_exchange_weak(&r->mark, &expected_mark, new_mark);
	atomic_thread_fence(memory_order_release);
	return rc;
}

int is_lock_free(atomic_markable_reference* m_ref)
{
	atomic_markable_reference_t* r = (atomic_markable_reference_t*)m_ref;
	atomic_thread_fence(memory_order_acquire);
	int rcr = atomic_is_lock_free(&r->ref);
	int rcm = atomic_is_lock_free(&r->mark);
	atomic_thread_fence(memory_order_release);
	return (rcr && rcm);
}
