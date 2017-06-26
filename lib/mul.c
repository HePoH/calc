#include "../include/cmplx_num.h"

char PLUGIN_NAME[] = "mul.so";
char MAIN_FUNCTION[] = "cn_mul";
char MENU_TITLE[] = "[Mul]tiplication";

cmplx_num* cn_mul(cmplx_num* first, cmplx_num* second){
	cmplx_num* ans = NULL;

	ans = malloc(sizeof(cmplx_num));
	if (ans == NULL)
		return NULL;

	ans->a = first->a * second->a - first->b * second->b;
	ans->b = first->a * second->b + first->b * second->a;

	return ans;
}
