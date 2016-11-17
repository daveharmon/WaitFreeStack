#ifndef ATOMICMARKABLEREFERENCE_H
#define ATOMICMARKABLEREFERENCE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct atomic_markable_reference atomic_markable_reference;

atomic_markable_reference* atomic_markable_reference_init(void* init_obj, int init_mark);
void atomic_markable_reference_destroy(atomic_markable_reference* m_ref);

/*
 * Atomically sets the value of the mark.  
 * Returns:
 * 		true if successful
 */
int attempt_mark(atomic_markable_reference* m_ref, 
	int expected_mark, int new_mark);

/*
 * Atomically sets the value of both the reference and mark to the given 
 * update values if the current reference is == to the expected reference 
 * and the current mark is equal to the expected mark.
 * Parameters:
 *		expectedReference - the expected value of the reference
 *		newReference - the new value for the reference
 *		expectedMark - the expected value of the mark
 *		newMark - the new value for the mark
 * Returns:
 *		true if successful
 */
int compare_and_set(atomic_markable_reference* m_ref, void* expected_ref, 
	void* new_ref, int expected_mark, int new_mark);

/*
 * Returns the current values of both the reference and the mark. 
 * Typical usage is int holder; ref = v.get(holder); 
 * Parameters:
 *		markHolder - an allocated int.  On return, it will hold the value of the mark.
 * Returns:
 *		the current value of the reference
 */
void* get(atomic_markable_reference* m_ref, int* mark_value);

/*
 * Returns the current value of the reference.
 * Returns:
 *		the current value of the reference
 */
void* get_reference(atomic_markable_reference* m_ref);

/* 
 * Returns the current value of the mark.
 * Returns:
 * 		the current value of the mark
 */
int is_marked(atomic_markable_reference* m_ref);

/* 
 * Unconditionally sets the value of both the reference and mark.
 * Parameters:
 * 		newReference - the new value for the reference
 * 		newMark - the new value for the mark
 */
void set(atomic_markable_reference* m_ref, void* new_ref, int new_mark);

/* 
 * Atomically sets the value of both the reference and mark to the given update
 * values if the current reference is == to the expected reference and the current
 * mark is equal to the expected mark.
 * May fail spuriously and does not provide ordering guarantees, so is only 
 * rarely an appropriate alternative to compareAndSet.
 * Parameters:
 * 		expectedReference - the expected value of the reference
 * 		newReference - the new value for the reference
 * 		expectedMark - the expected value of the mark
 * 		newMark - the new value for the mark
 * Returns:
 * 		true if successful
 */
int weak_compare_and_set(atomic_markable_reference* m_ref, void* expected_ref, 
	void* new_ref, int expected_mark, int new_mark);

/*
 * In C, the implementation is architecture dependent, and not every
 * implementation is lock free, this should atomically tell you if both
 * the mark and reference values in the underlying structure are lock free
 * Returns:
 * 		true if both member variables are lock free
 */
 int is_lock_free(atomic_markable_reference* m_ref);

#ifdef __cplusplus
}
#endif

#endif
