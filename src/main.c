#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/saph.h"
#include "../include/merror.h"

#define argcmp(a, s, l) strcmp(a, s) == 0 || strcmp(a, l) == 0


char* remove_last_ext(const char* filename);

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		SaphCore* core = saph_init("print 1 + 1 * 3.367 - 9");
		saph_run(core, 1);
		saph_free(core);

		return;
	}

	const char* arg1 = argv[1];
	
	if (argcmp(arg1, "-v", "--version"))
		printf(BUILD);
	else if (argcmp(arg1, "-h", "--help"))
	{
		printf("usage: saph [-v | -h | file] [options]\n");
		printf("options:\n");
		printf("-v, --version\t: display Saph version\n");
		printf("-h, --help\t: display this help message\n");
		printf("-b, --build\t: compile source code into an executable\n");
		printf("-d, --debug\t: enable debug mode\n");
		printf("\t\t  preserves intermediate files and displays executed commands\n");
		printf("arguments:\n");
		printf("file\t\t: source code file\n");
	}
	else
	{
		char* filename = remove_last_ext(arg1);
		FILE* fp = fopen(arg1, "r");
		if (!fp) return 1;
		char source[100];

		fgets(source, sizeof(source), fp);
		SaphCore* core = saph_init(source);
		int build = 0, debug = 0;

		if (argc > 2)
		{
			for (int i = 2; i < argc; i++)
			{
				if (argcmp(argv[i], "-b", "--build"))
					build = 1;

				if (argcmp(argv[i], "-d", "--debug"))
					debug = 1;
			}
		}

		if (build)
			saph_build(core, filename, debug);
		else
			saph_run(core, debug);

		saph_free(core);
		fclose(fp);
		free(filename);
	}

	return 0;
}

char* remove_last_ext(const char* filename)
{
	const int extra_space = 100;
	char* result = malloc(strlen(filename) + extra_space + 1);
	if (!result) return NULL;

	strcpy(result, filename);
	char* last_ext = strrchr(result, '.');

	if (last_ext && strcmp(last_ext, ".sa") == 0)
		*last_ext = '\0';

	return result;
}
