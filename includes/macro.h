#ifndef MACRO_H
#define MACRO_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "settings.h"
#include "memory_utils.h"

struct Macro {
	char* name;
	char** variables;
	size_t variables_size;
	size_t variables_capacity;
	char** code;
	size_t code_size;
	size_t code_capacity;
};

struct MacroList {
	struct Macro* macro;
	struct MacroList* next;
};

struct CallMacroVariables {
	char** variables;
	size_t size;
	size_t capacity;
};

void resizeAndCopy(char** textData, size_t* size, size_t* capacity, const char* str);

struct Macro* createMacro(const char* name);
void freeMacro(struct Macro** macro);

void macroAddVariable(struct Macro* macro, const char* variable);
void macroAddCode(struct Macro* macro, const char* code);
ssize_t findMacroVariable(struct Macro* macro, const char* variable);

struct MacroList* createEmptyMacroList();
struct MacroList* createMacroList(struct Macro* macro);
void freeMacroList(struct MacroList** list);

void addMacroToList(struct MacroList* list, struct Macro* macro);
struct Macro* findMacroInList(const struct MacroList* list, const char* name);

struct CallMacroVariables* createCallMacroVariables();
void freeCallMacroVariables(struct CallMacroVariables** cmv);

void addVariableToCallMacroVariables(struct CallMacroVariables* cmv, const char* variable);

#endif // !MACRO_H