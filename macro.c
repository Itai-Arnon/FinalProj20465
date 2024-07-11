#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "macro.h"

/*int getErrorLoci(FILE* file){}*/

FILE *fptr_before = NULL;
FILE *fptr_after = NULL;

int main(int argc, char *argv[]) {
	char buffer[80];

	printf("argc: %d\n", argc);
	printf("argv: %s\n", argv[argc - 1]);
	readline(argc, argv);


	printf("%d\n", argv_length(argv[argc - 1]));

	return 0;
}
int dummy(){
	return 1;
}
int typeofline(char *line) {
	char start[7];
	char mac_name[MAX_MACRO_NAME];
	char *str = line;
	int pos = 0;

	if (sscanf(str, "%s%n", start, &pos) == 1) {
		if (strncmp(MACRO_START_WORD, str, 4) == 0) {
			str += pos + 1;
			if (sscanf(str, "%s%n", mac_name, &pos) == 1) {
				printf("placeholder for function that checks name");
				return macro_start;/*==0*/
			}
		}
		else if(strncmp(MACRO_END_WORD, start, 7) == 0)	{
			return macro_end;
		}
		else if	(dummy() == 1)
			return macro_expand;


	}


	void readline(int _argc, char **_argv) {
		char buffer[LINE_LENGTH];
		char *macro_buffer;

		int count = 0, end = 0;
		fptr_before = initPointerFile(_argc, _argv, fptr_before, 0);
		fptr_after = initPointerFile(_argc, _argv, fptr_after, 1);

		while (fgets(buffer, 10, fptr_before) != NULL);

		printf("%s\n", buffer);
		/*later will head the switch*/
		/*typeofline(char* line)*/
		fclose(fptr_before);
		fclose(fptr_after);
	}





/*macro_buffer = malloc(count * LINE_LENGTH * sizeof(char));*/
/*	strcpy(macro_buffer,buffer);
	printf("%s\n",macro_buffer);*/


	int argv_length(char *arr) {
		int count = 0;
		while (arr[++count] != 0);
		return count;
	}

/*type=0 for input file type =1 for output*/
	FILE *initPointerFile(int _argc, char **_argv, FILE *fptr, int type) {
		char line[LINE_LENGTH];
		char filename_before[64];
		char filename_after[64];
		int argv_len = 0;
		if (_argc > 1) {/*argc  must always be subtracted by one*/
			argv_len = argv_length(_argv[_argc - 1]);
			switch (type) {
				case 0:
					strcpy(filename_before, PATH_BASE);/*TODO no need in linux*/
					strncat(filename_before, _argv[_argc - 1], argv_len); /*TODO arg_c iteration tbd*/
					printf("%s\n", filename_before);
					if (!(fptr = fopen(filename_before, "r"))) {
						printf("%s\n", ERR_FILE_BEFORE);
						exit(0);
					}
					return fptr;
				case 1:
					strcpy(filename_after, PATH_BASE);/*TODO no need in linux*/
					strcat(filename_after, "out");
					strncat(filename_after, _argv[_argc - 1], argv_len);
					/*TODO add extension in linux*/
					printf("%s\n", filename_after);
					if (!(fptr = fopen(filename_after, "a"))) {
						printf("%s\n", ERR_FILE_AFTER);
						exit(0);
					}
					printf("success\n");
					return fptr;

				default:
					break;
			}

		} else printf("%s\n", ERR_NO_MORE_FILES);
	}


