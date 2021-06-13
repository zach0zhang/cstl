#include <stdio.h>

extern void test_cstl_vector();
extern void test_cstl_map();
extern void test_cstl_deque();
extern void test_cstl_stack();
extern void test_cstl_queue();
extern void test_cstl_set();

int main(int argc, char *argv[])
{
	test_cstl_vector();
	test_cstl_map();
	test_cstl_deque();
	test_cstl_stack();
	test_cstl_queue();
	test_cstl_set();
}
