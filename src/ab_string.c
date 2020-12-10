#include "../include/ab_string.h"
#include <stdlib.h>
#include <string.h>

ab_string* ab_string_new(void) {
	ab_string* str = (ab_string*)malloc(sizeof(ab_string*));
	str->value = malloc(0);
	str->length = 0;
	return str;
}

ab_string* ab_string_from(char* value, int length) {
	ab_string* str = (ab_string*)malloc(sizeof(ab_string*));
	str->value = (char*)malloc(sizeof(char) * (length + 1));
	strcpy(str->value, value);
	str->value[length] = 0;
	str->length = length;
	return str;
}

void ab_string_destroy(ab_string* str) {
	free(str->value);
	free(str);
}

void ab_string_add_ch(ab_string* str, char c) {
	str->value = realloc(str->value, sizeof(char) * (str->length + 2));
	str->value[str->length] = c;
	str->value[str->length+1] = 0;
	str->length++;
}

void ab_string_add_str(ab_string* str1, ab_string* str2) {
	str1->value = realloc(str1->value, sizeof(char) * (str1->length + str2->length + 1));
	strcat(str1->value, str2->value);
	str1->value[str1->length + str2->length] = 0;
	str1->length += str2->length;
}

void ab_string_replace(ab_string* str, char* value, int length) {
	str->value = realloc(str->value, sizeof(char) * (length + 1));
	strcpy(str->value, value);
	str->value[length] = 0;
	str->length = length;
}

int ab_string_find(ab_string* haystack, ab_string* needle) {
	char* result_ptr = strstr(haystack->value, needle->value);
	return result_ptr == NULL ? -1 : result_ptr - haystack->value;
}

ab_string* ab_string_substr(ab_string* str, int start, int length) {
	if(start < 0 || start + length >= str->length) {
		return NULL;
	}
	char result_cstr[length + 1];
	strncpy(result_cstr, &(str->value[start]), length);
	result_cstr[length] = 0;
	return ab_string_from(result_cstr, length);
}
