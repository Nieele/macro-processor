#define _GNU_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "settings.h"
#include "macro.h"
#include "str_utils.h"

struct Macro* readMacro(FILE* inFile, char* line, size_t* lineSize, const char* delimiters, char* word)
{
	bool statusReading = true;

	if (scanWord(NULL, delimiters, BUFFER_SIZE, word) == -1)
		exit(INCORRECT_MACRO_DEFINITION);

	struct Macro* macro = createMacro(word);

	while (scanWord(NULL, delimiters, BUFFER_SIZE, word) != -1)
		macroAddVariable(macro, word);

	while (getline(&line, lineSize, inFile) != -1) {
		// skipping empty lines
		if (scanWord(line, delimiters, BUFFER_SIZE, word) == -1)
			continue;

		// the end of the macro is found
		if (strcmp(word, "endm") == 0) {
			statusReading = false;
			break;
		}

		macroAddCode(macro, line);
	}

	// the end was not found
	if (statusReading == true)
		exit(INCORRECT_MACRO_DEFINITION);

	return macro;
}

void insertMacro(FILE* outFile, struct Macro* findingMacro, const char* delimiters, char* word)
{
	// read the variables of the called macro
	struct CallMacroVariables* cmv = createCallMacroVariables();

	while (scanWord(NULL, delimiters, BUFFER_SIZE, word) != -1)
		addVariableToCallMacroVariables(cmv, word);

	if (cmv->size != findingMacro->variables_size)
		exit(INCORRECT_MACRO_DEFINITION);

	bool firstVariable;
	ssize_t posVariable = -1;

	// replace variables
	for (size_t i = 0; i < findingMacro->code_size; ++i) {
		firstVariable = true;

		scanWord(findingMacro->code[i], delimiters, BUFFER_SIZE, word);
		fprintf(outFile, "%s ", word);

		while (scanWord(NULL, delimiters, BUFFER_SIZE, word) != -1) {
			if (firstVariable == false)
				fprintf(outFile, ", ");
			firstVariable = false;

			if ((posVariable = findMacroVariable(findingMacro, word)) != -1)
				fprintf(outFile, "%s", cmv->variables[posVariable]);
			else
				fprintf(outFile, "%s", word);
		}

		fprintf(outFile, "\n");
	}

	freeCallMacroVariables(&cmv);
}

void generateCode(FILE* inFile, FILE* outFile)
{
	char* line = NULL;
	const char* delimiters = " \t,";
	size_t lineSize = 0;

	char word[BUFFER_SIZE];

	struct MacroList* list = createEmptyMacroList();

	struct Macro* findingMacro = NULL;

	while (getline(&line, &lineSize, inFile) != -1) {
		if (scanWord(line, delimiters, BUFFER_SIZE, word) != -1) {
			if (strcmp(word, "macro") == 0) 
			{
				addMacroToList(list, readMacro(inFile, line, &lineSize, delimiters, word));
			}
			else if ((findingMacro = findMacroInList(list, word)) != NULL) 
			{
				insertMacro(outFile, findingMacro, delimiters, word);
			}
			else 
			{
				fprintf(outFile, "%s", line);
			}
		}
	}

	freeMacroList(&list);
	free(line);
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("Error! The command should like \"macro [in filename] [out filename]\"\n");
		return INCORRECT_NUMBER_ARGUMENTS;
	}

	FILE* inFile;
	FILE* outFile;
	if ((inFile = fopen(argv[1], "r")) == NULL || (outFile = fopen(argv[2], "w")) == NULL)
	{
		return CANT_OPEN_FILE;
	}

	generateCode(inFile, outFile);

	fclose(inFile);
	fclose(outFile);

	return 0;
}
