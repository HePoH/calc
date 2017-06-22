#include "../include/cmplx_num.h"

int file_filter(const struct dirent *entry) {
	return strstr(entry->d_name, ".so") != NULL;
}

struct dirent** find_libs(char* path, int* fc) {
	struct dirent** fnl;
	*fc = scandir(path, &fnl, file_filter, alphasort);

	return (*fc > 0) ? fnl : NULL;
}

void* load_lib(char* path){
	void *ext_lib;
	pl_f func = NULL;

	ext_lib = dlopen(path, RTLD_NOW);
	if (!ext_lib){
		fprintf(stderr, "dlopen() error: %s\n", dlerror());
		return NULL;
	}

	/*func = dlsym(ext_lib, "function");*/
	void* obj = dlsym(ext_lib, "MAIN_FUNCTION");
	char* ptr = (char*) obj;
	printf("PLUGIN_NAME = %s (%d) \n\n", ptr, strlen(ptr));

	dlclose(ext_lib);

	return func;
}

int main(){
	char c, lp[BUF_SIZE];
	int i = 0, lc = 0;
	struct dirent** lfn = NULL;

	cmplx_num *first = 0, *second = 0, *ans = 0;
	pl_f* func = NULL;
	/*cmplx_num* (**func)(cmplx_num*, cmplx_num*);*/

	/*first = (cmplx_num*) malloc(sizeof(cmplx_num));
	second = (cmplx_num*) malloc(sizeof(cmplx_num));

	if (first == NULL || second == NULL)
		return 0;

	printf("\033[2J");
	printf("\033[0;0f");*/

	chdir("./plugins/");
	lfn = find_libs(".", &lc);
	func = malloc(lc * sizeof(pl_f));

	char dir[BUF_SIZE];
	getcwd(dir, BUF_SIZE);
	snprintf(lp, BUF_SIZE, "%s/%s", dir, lfn[0]->d_name);

	printf("PATH: %s\n", lp);
	load_lib(lp);

	/*for (i = 0; i < lc; i++)
		func[i] = malloc(sizeof(void(*)(cmplx_num*, cmplx_num*)));
		func[i] = (cmplx_num*) load_lib("/home/2017d_medvedev/work/calc/plugins/add.so");*/

	for (i = 0; i < lc; i++)
		printf("Lib: %s\n", lfn[i]->d_name);

	getchar();
	/*while(c != 'q'){
		printf("\033[2J");
		printf("\033[0;0f");

		printf("\n[ Arithmetic operations on complex numbers ]\n");
		printf(" 1. [Add]ition\n 2. [Sub]traction\n 3. [Mul]tiplication\n 4. [Div]ision\n 5. [Ex]it\n");
		printf("\n > Select an arithmetic operation: ");
		scanf("%c", &c);

		switch (c){
			printf("\033[2J");
			printf("\033[0;0f");

			case '1': printf("\nInput the first complex number: ");
				scanf("%lf %lf", &first->a, &first->b);

				printf("Input the second complex number: ");
				scanf("%lf %lf", &second->a, &second->b);

				ans = cn_add(first, second);
				(!ans) ? printf("Error") : printf("\nResult add: %.1f + %.1fi", ans->a, ans->b);
				free(ans);

				break;

			case '2': printf("\nInput the first complex number: ");
				scanf("%lf %lf", &first->a, &first->b);

				printf("Input the second complex number: ");
				scanf("%lf %lf", &second->a, &second->b);

				ans = cn_sub(first, second);
				(!ans) ? printf("Error") : printf("\nRsult sub: %.1f + %.1fi", ans->a, ans->b);
				free(ans);

				break;

			case '3': printf("\nInput the first complex number: ");
				scanf("%lf %lf", &first->a, &first->b);

				printf("Input the second complex number: ");
				scanf("%lf %lf", &second->a, &second->b);

				ans = cn_mul(first, second);
				(!ans) ? printf("Error") : printf("\nRsult mul: num = %.1f + %.1fi", ans->a, ans->b);
				free(ans);

				break;

			case '4': printf("\nInput the first complex number: ");
				scanf("%lf %lf", &first->a, &first->b);

				printf("Input the second complex number: ");
				scanf("%lf %lf", &second->a, &second->b);

				ans = cn_div(first, second);
				(!ans) ? printf("Error") : printf("\nResult div: num = %.1f + %.1fi", ans->a, ans->b);
				free(ans);

				break;
			case '5': case 'q': return;
			default: break;
		}

		getchar();
		getchar();
	}*/

	return 0;
}
