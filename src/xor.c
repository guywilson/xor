#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

void printUsage()
{
	printf("Usage: xor <options> input_file\n");
	printf("\t-k <keystream file>\n");
	printf("\t-o <output file>\n\n");
}

uint32_t getFileSize(FILE * fptr)
{
	uint32_t		size;
	long			currentPos;

	currentPos = ftell(fptr);

	fseek(fptr, 0L, SEEK_SET);
	fseek(fptr, 0L, SEEK_END);

	size = (uint32_t)ftell(fptr);

	fseek(fptr, currentPos, SEEK_SET);

	return size;
}

int main(int argc, char ** argv)
{
	FILE *			fptrKey;
	FILE *			fptrInput;
	FILE *			fptrOutput;
    char *          pszKeyFile = NULL;
    char *          pszInputFile = NULL;
    char *          pszOutputFile = NULL;
	uint32_t		keyLength;
	uint32_t		inputLength;
	uint32_t		counter;
	uint8_t			inputByte;
	uint8_t			outputByte;
	uint8_t			keyByte;

    if (argc > 1) {
        pszInputFile = strdup(&argv[argc - 1][0]);

        for (int i = 1;i < (argc - 1);i++) {
            if (argv[i][0] == '-') {
                if (argv[i][1] == 'o') {
                    pszOutputFile = strdup(&argv[i+1][0]);
                }
                else if (argv[i][1] == 'k') {
                	pszKeyFile = strdup(&argv[i+1][0]);
                }
                else if (argv[i][1] == 'h') {
                    printUsage();
                    exit(0);
                }
            }
        }
    }
	else {
		printUsage();
		exit(0);
	}

	fptrKey = fopen(pszKeyFile, "rb");

	if (fptrKey == NULL) {
		fprintf(stderr, "Failed to open keystream file %s:%s", pszKeyFile, strerror(errno));
		exit(-1);
	}
	
	fptrInput = fopen(pszInputFile, "rb");

	if (fptrInput == NULL) {
		fprintf(stderr, "Failed to open input file %s:%s", pszInputFile, strerror(errno));
		exit(-1);
	}

	fptrOutput = fopen(pszOutputFile, "wb");

	if (fptrOutput == NULL) {
		fprintf(stderr, "Failed to open output file %s:%s", pszOutputFile, strerror(errno));
		exit(-1);
	}

	keyLength = getFileSize(fptrKey);
	inputLength = getFileSize(fptrInput);
	
	if (keyLength < inputLength) {
		fprintf(stderr, "The key must be at least as long as the message to encrypt/decrypt\n");
		exit(-1);
	}

	for (counter = 0;counter < inputLength;counter++) {
		inputByte = (uint8_t)fgetc(fptrInput);

		if (feof(fptrInput)) {
			break;
		}

		keyByte = (uint8_t)fgetc(fptrKey);

		if (feof(fptrKey)) {
			break;
		}

		outputByte = inputByte ^ keyByte;

		fputc((int)outputByte, fptrOutput);
	}

	fclose(fptrOutput);
	fclose(fptrInput);
	fclose(fptrKey);
}
