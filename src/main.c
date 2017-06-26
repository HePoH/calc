#include "../include/cmplx_num.h"

int lib_file_filter(const struct dirent *entry) {
	return strstr(entry->d_name, ".so") != NULL;
}

struct dirent** find_libs(char* path, int* fc) {
	struct dirent** fnl = NULL;
	*fc = scandir(path, &fnl, lib_file_filter, alphasort);

	return (*fc > 0) ? fnl : NULL;
}

char* get_plgn_path(char* path, char* plgn_name) {
	char* plgn_path = NULL, cur_dir[BUF_SIZE] = {0};

	plgn_path = malloc(BUF_SIZE * sizeof(char));
	if (plgn_path == NULL) {
		perror("malloc");
		return NULL;
	}

	getcwd(cur_dir, BUF_SIZE);
	snprintf(plgn_path, BUF_SIZE, "%s/%s", cur_dir, plgn_name);

	return plgn_path;
}

PLUGIN_INFO* load_plgns(char* path, int* plgns_count){
	int i = 0, pc = 0;
	struct dirent** pfn = NULL;
	void *ext_lib = NULL;
	plgn_f func = NULL;
	char* plgn_path = NULL, *error = NULL;
	PLUGIN_INFO* plgns = NULL;

	if (chdir(path) != 0) {
		perror("chdir");
		return NULL;
	}

	pfn = find_libs(".", &pc);
	plgns = malloc(pc * sizeof(PLUGIN_INFO));
	if (plgns == NULL) {
		perror("malloc");
		return NULL;
	}

	*plgns_count = pc;

	for (i = 0; i < pc; i++) {
		plgn_path = get_plgn_path(path, pfn[i]->d_name);

		ext_lib = dlopen(plgn_path, RTLD_NOW);
		if (!ext_lib){
			fprintf(stderr, "dlopen() error: %s\n", dlerror());
			return NULL;
		}

		plgns[i].plgn_name = dlsym(ext_lib, "PLUGIN_NAME");
                plgns[i].main_func = dlsym(ext_lib, "MAIN_FUNCTION");
                plgns[i].menu_title = dlsym(ext_lib, "MENU_TITLE");
		plgns[i].pf = dlsym(ext_lib, plgns[i].main_func);

		if ((error = dlerror()) != NULL)  {
                	fprintf (stderr, "dlsym() error: %s\n", error);
                	return NULL;
        	}

		/*printf("[%s]: %s\n -- %s\n -- %s\n -- %s\n -- %p\n\n", pfn[i]->d_name, plgn_path, plgns[i].plgn_name, plgns[i].main_func, plgns[i].menu_title, plgns[i].pf);
		dlclose(ext_lib);*/
	}

	return plgns;
}

int main(){
	char user_sel[BUF_SIZE] = {0};
	int i = 0, pc = 0, us = 0;

	cmplx_num *first = NULL, *second = NULL, *ans = NULL;
	PLUGIN_INFO* plgns = NULL;

	first = malloc(sizeof(cmplx_num));
	second = malloc(sizeof(cmplx_num));

	if (first == NULL || second == NULL) {
		perror("malloc");
		return 0;
	}

	printf("\033[2J");
	printf("\033[0;0f");

	plgns = load_plgns("./plugins/", &pc);
	if (plgns == NULL){
		return 0;
	}

	printf("[ Plugins: %d ]\n\n", pc);
	for (i = 0; i < pc; ++i)
		printf("%d. [%s]:\n -- %s\n -- %s\n -- %s\n -- %p\n\n", i + 1, plgns[i].plgn_name, plgns[i].plgn_name, plgns[i].main_func, plgns[i].menu_title, plgns[i].pf);

	getchar();
	while(1){
		printf("\033[2J");
		printf("\033[0;0f");

		printf("\n[ Arithmetic operations on complex numbers ]\n\n");

		for (i = 0; i < pc; i++)
			printf("%d. %s\n", i + 1, plgns[i].menu_title);

		printf("%d. [Ex]it\n", i + 1);

		printf("\n > Select an arithmetic operation: ");
		fgets(user_sel, BUF_SIZE, stdin);
		sscanf(user_sel, "%d", &us);
		us--;

		if ((us == pc) || (user_sel[0] == 'q') || (strcmp("quit", user_sel) == 0) || (strcmp("exit", user_sel) == 0))
			return 0;
		else
			if (us < pc) {
				printf("\033[2J");
                        	printf("\033[0;0f");

				printf("\nInput the first complex number: ");
				scanf("%lf %lf", &first->a, &first->b);

				printf("Input the second complex number: ");
				scanf("%lf %lf", &second->a, &second->b);

				ans = plgns[us].pf(first, second);
				(!ans) ? printf("Error") : printf("\nResult: %.1f + %.1fi", ans->a, ans->b);
				free(ans);

				getchar();
				getchar();
			}
	}

	return 0;
}
