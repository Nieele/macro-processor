#include "macro.h"

void resizeAndCopy(char** textData, size_t* size, size_t* capacity, const char* str) 
{
	if (textData == NULL)
		return;

	if (*size > *capacity) {
		(*capacity) *= 2;
		safe_realloc(textData, (*capacity) * sizeof(char*));
	}

	textData[*size] = safe_malloc(strlen(str) * sizeof(char) + 1);
	strcpy(textData[*size], str);
	(*size)++;
}

struct Macro* createMacro(const char* name)
{
	struct Macro* temp = safe_malloc(sizeof(struct Macro));

	temp->name = safe_malloc(strlen(name) * sizeof(char) + 1);
	strcpy(temp->name, name);

	temp->variables = safe_malloc(DEFAULT_VARIABLES_CAPACITY * sizeof(char*));
	temp->variables_size = 0;
	temp->variables_capacity = DEFAULT_VARIABLES_CAPACITY;

	temp->code = safe_malloc(DEFAULT_CODE_CAPACITY * sizeof(char*));
	temp->code_size = 0;
	temp->code_capacity = DEFAULT_CODE_CAPACITY;
	
	return temp;
}

void freeMacro(struct Macro** macro) {
	if ((*macro) == NULL)
		return;

	free((*macro)->name);

	for (size_t i = 0; i < (*macro)->variables_size; ++i)
		free((*macro)->variables[i]);

	for (size_t i = 0; i < (*macro)->code_size; ++i)
		free((*macro)->code[i]);

	free((*macro)->variables);
	free((*macro)->code);

	free(*macro);
	*macro = NULL;
}

void macroAddVariable(struct Macro* macro, const char* variable)
{
	if (macro == NULL || variable == NULL)
		return;

	resizeAndCopy(macro->variables, &macro->variables_size, &macro->variables_capacity, variable);
}

void macroAddCode(struct Macro* macro, const char* code)
{
	if (macro == NULL || code == NULL)
		return;

	resizeAndCopy(macro->code, &macro->code_size, &macro->code_capacity, code);
}

ssize_t findMacroVariable(struct Macro* macro, const char* variable)
{
	for (ssize_t i = 0; i < macro->variables_size; ++i)
		if (strcmp(macro->variables[i], variable) == 0)
			return i;
	return -1;
}

struct MacroList* createEmptyMacroList()
{
	struct MacroList* temp = safe_malloc(sizeof(struct MacroList));
	temp->macro = NULL;
	temp->next = NULL;
	return temp;
}

struct MacroList* createMacroList(struct Macro* macro)
{
	struct MacroList* temp = safe_malloc(sizeof(struct MacroList));
	temp->macro = macro;
	temp->next = NULL;
	return temp;
}

void freeMacroList(struct MacroList** list) {
	if (*list == NULL)
		return;

	if ((*list)->next != NULL)
		freeMacroList(&(*list)->next);
	
	freeMacro(&(*list)->macro);
	free(*list);
	*list = NULL;
}

void addMacroToList(struct MacroList* list, struct Macro* macro) {
	if (list == NULL || macro == NULL)
		return;

	if (list->macro == NULL) {
		list->macro = macro;
		return;
	}

	while (1) {
		if (list->next == NULL) {
			list->next = createMacroList(macro);
			return;
		}
		else
			list = list->next;
	}
}

struct Macro* findMacroInList(const struct MacroList* list, const char* name) {
	if (list == NULL || name == NULL)
		return NULL;
	
	if (list->macro == NULL)
		return NULL;

	while (list != NULL) {
		if (strcmp(list->macro->name, name) == 0)
			return list->macro;
		else list = list->next;
	}

	return NULL;
}

struct CallMacroVariables* createCallMacroVariables()
{
	struct CallMacroVariables* temp = safe_malloc(sizeof(struct CallMacroVariables));
	temp->variables = safe_malloc(DEFAULT_VARIABLES_CAPACITY * sizeof(char*));
	temp->size = 0;
	temp->capacity = DEFAULT_VARIABLES_CAPACITY;

	return temp;
}

void freeCallMacroVariables(struct CallMacroVariables** cmv)
{
	if (*cmv == NULL)
		return;

	for (size_t i = 0; i < (*cmv)->size; ++i)
		free((*cmv)->variables[i]);
	free((*cmv)->variables);
	free(*cmv);
	*cmv = NULL;
}

void addVariableToCallMacroVariables(struct CallMacroVariables* cmv, const char* variable)
{
	if (cmv == NULL)
		return;

	resizeAndCopy(cmv->variables, &cmv->size, &cmv->capacity, variable);
}