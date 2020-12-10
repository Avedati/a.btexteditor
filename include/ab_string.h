#ifndef _AB_STRING_H_
#define _AB_STRING_H_ 0

typedef struct ab_string {
	char* value;
	int length;
} ab_string;

ab_string* ab_string_new(void);
ab_string* ab_string_from(char*,int);
void ab_string_destroy(ab_string*);
void ab_string_add_ch(ab_string*,char);
void ab_string_add_str(ab_string*,ab_string*);
void ab_string_replace(ab_string*,char*,int);
int ab_string_find(ab_string*,ab_string*);
ab_string* ab_string_substr(ab_string*,int,int);

#endif // _AB_STRING_H_
