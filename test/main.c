#include <stdio.h>
extern void test_cstl_vector();
extern void test_cstl_map();
extern void test_cstl_deque();

int main(int argc, char *argv[])
{
	test_cstl_vector();
	test_cstl_map();
	test_cstl_deque();
}
