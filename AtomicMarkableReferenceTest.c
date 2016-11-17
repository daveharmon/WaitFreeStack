
#include "AtomicMarkableReference.h"
#include <stdio.h>

int main()
{
	int ref = 5;
	atomic_markable_reference* m_ref = atomic_markable_reference_init(&ref, 1);
	printf("clearing mark, current value: %i\n", is_marked(m_ref));
	int rc = attempt_mark(m_ref, 1, 0);
	printf("rc of attempt mark: %i, value: %i\n", rc, is_marked(m_ref));
	printf("reference get: %i\n", *(int*)get_reference(m_ref));
	int new_ref = 6;
	printf("return value of compare_and_set: %i\n", 
		compare_and_set(m_ref, &ref, &new_ref, 0, 1));
	printf("reference get: %i\n", *(int*)get_reference(m_ref));
	printf("mark get: %i\n", is_marked(m_ref));
	int mark_value = -1;
	rc = *(int*)get(m_ref, &mark_value);
	printf("return code of get: %d, and current mark: %d\n", rc, mark_value);
	printf("is_marked: should be 1: %i\n", is_marked(m_ref));
	printf("testing set to 7 and cleared\n");
	new_ref = 7;
	set(m_ref, &new_ref, 0);
	mark_value = -1;
	rc = *(int*)get(m_ref, &mark_value);
	printf("return code of get: %d, and current mark: %d\n", rc, mark_value);
	printf("testing weak compare and set\n");
	ref = 8;
	printf("return code of weak c&s: %d\n", 
		weak_compare_and_set(m_ref, &new_ref, &ref, 0, 1));
	mark_value = -1;
	rc = *(int*)get(m_ref, &mark_value);
	printf("return code of get: %d, and current mark: %d\n", rc, mark_value);
	atomic_markable_reference_destroy(m_ref);
	printf("ref is lock free: %i\n", is_lock_free(m_ref));
	exit(0);
}
