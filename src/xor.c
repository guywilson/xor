#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

uint8_t * xor(uint8_t * input, uint32_t inputLength, uint8_t * key, uint32_t keyLength)
{
	uint8_t *		output;
	uint32_t		i;

	if (keyLength < inputLength) {
		fprintf(stderr, "The key must be at least as long as the message to encrypt/decrypt\n");
		return NULL;
	}
	
	output = (uint8_t *)malloc(inputLength);

	if (output == NULL) {
		fprintf(stderr, "Failed to allocate %u bytes for the output buffer", inputLength);
		return NULL;
	}
	
	for (i = 0;i < inputLength;i++) {
		output[i] = input[i] ^ key[i];
	}

	return output;
}

void printUsage()
{
	printf("Usage: xor <options> input_file\n");
	printf("\t-k <keystream file>\n");
	printf("\t-o <output file>\n\n");
}

int main(int argc, char ** argv)
{
	FILE *			fptrKey;
	FILE *			fptrInput;
	FILE *			fptrOutput;
    char *          pszKeyFile = NULL;
    char *          pszInputFile = NULL;
    char *          pszOutputFile = NULL;

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

	fptrOutput = fopen(pszKeyFile, "wb");

	if (fptrOutput == NULL) {
		fprintf(stderr, "Failed to open output file %s:%s", pszOutputFile, strerror(errno));
		exit(-1);
	}
}
