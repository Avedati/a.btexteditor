#include "../include/ab_string.h"
#include <ctype.h>
#include <gtk/gtk.h>
#include <stdio.h>
#define WINDOW_TITLE "a.b text editor"
#define WINDOW_WIDTH 840
#define WINDOW_HEIGHT 680
#define WINDOW_BORDER_WIDTH 10

const int N_SUPPORTED_LANGUAGES = 13;
const char* SUPPORTED_LANGUAGES[] = {
	"C",
	"C Header File",
	"CSS",
	"C++",
	"C++ Header File",
	"HTML",
	"Java",
	"JavaScript",
	"Plain Text",
	"Plain Text (No File Extension)",
	"Python",
	"Ruby",
	"Rust"
};

// https://www.programiz.com/c-programming/list-all-keywords-c-language
const char* C_KEYWORDS[] = {
	"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register",
	"return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
};

// https://www.w3schools.com/TAGs/
const char* HTML_KEYWORDS[] = {
	"a", "abbr", "acronym", "address", "applet", "area", "article", "aside", "audio", "b", "base", "basefont", "bdi", "bdo", "big", "blockquote", "body", "br",
	"button", "canvas", "caption", "center", "cite", "code", "col", "colgroup", "datalist", "dd", "del", "details", "dfn", "dialog", "dir", "div", "dl", "dt", "em",
	"embed", "fieldset", "figcaption", "figure", "font", "footer", "form", "frame", "frameset", "h1", "h2", "h3", "h4", "h5", "h6",
	"head", "header", "hr", "html", "i", "iframe", "img", "input", "ins", "kbd", "label", "legend", "li", "link", "main", "map", "mark", "meta", "meter", "nav",
	"noframes", "noscript", "object", "ol", "optgroup", "option", "output", "p", "param", "pre", "progress", "q", "rp", "rt", "ruby", "s", "samp", "script",
	"section", "select", "small", "source", "span", "strike", "strong", "style", "sub", "summary", "sup", "table", "tbody", "td", "textarea", "tfoot", "th", "thead",
	"time", "title", "tr", "track", "tt", "u", "ul", "var", "video", "wbr"
};

// https://docs.microsoft.com/en-us/cpp/cpp/keywords-cpp?view=msvc-160
const char* CPP_KEYWORDS[] = {
	"alignas", "alignof", "and", "and_eq", "asm", "auto", "bitand", "bitor", "bool", "break", "case", "catch", "char", "char8_t", "char16_t", "char32_t", "class",
	"compl", "concept", "const", "const_cast", "consteval", "constexpr", "constinit", "continue", "co_await", "co_return", "co_yield", "decltype", "default",
	"delete", "do", "double", "dynamic_cast", "else", "enum", "explicit", "export", "extern", "false", "float", "for", "friend", "goto", "if", "inline", "int",
	"long", "mutable", "namespace", "new", "noexcept", "not", "not_eq", "nullptr", "operator", "or", "or_eq", "private", "protected", "public",
	"register", "reinterpret_cast", "requires", "return", "short", "signed", "sizeof", "static", "static_assert", "static_cast", "struct", "switch",
	"template", "this", "thread_local", "throw", "true", "try", "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual", "void", "volatile",
	"wchar_t", "while", "xor", "xor_eq",
};

// https://docs.oracle.com/javase/tutorial/java/nutsandbolts/_keywords.html
const char* JAVA_KEYWORDS[] = {
	"abstract", "continue", "for", "new", "switch", "assert", "default", "goto", "package", "synchronized", "boolean", "do", "if", "private", "this",
	"break", "double", "implements", "protected", "throw", "byte", "else", "import", "public", "throws", "case", "enum", "instanceof", "return", "transient",
	"catch", "extends", "int", "short", "try", "char", "final", "interface", "static", "void", "class", "finally", "long", "strictfp", "volatile",
	"const", "float", "native", "super", "while"
};

// https://www.w3schools.in/javascript-tutorial/keywords/#:~:text=What%20Are%20JavaScript%20Keywords%3F%20Keywords%20are%20reserved%20words,of%20them%20are%20shown%20in%20the%20below-mentioned%20table%3A
const char* JS_KEYWORDS[] = {
	"abstract", "arguments", "boolean", "break", "byte", "case", "catch", "char", "const", "continue", "debugger", "default", "delete", "do", "double", "else",
	"eval", "false", "final", "finally", "float", "for", "function", "goto", "if", "implements", "in", "instanceof", "int", "interface", "let", "long",
	"native", "new", "null", "package", "private", "protected", "public", "return", "short", "static", "switch", "synchronized",
	"this", "throw", "throws", "transient", "true", "try", "typeof", "var", "void", "volatile", "while", "with", "yield", "class", "enum", "export", "extends",
	"import", "super"
};

// https://www.programiz.com/python-programming/keyword-list#:~:text=%20Description%20of%20Keywords%20in%20Python%20with%20examples,name%20%28user-defined%29%20to%20a%20module%20while...%20More%20
const char* PYTHON_KEYWORDS[] = {
	"False", "await", "else", "import", "pass", "None", "break", "except", "in", "raise", "True", "class", "finally", "is", "return",
	"and", "continue", "for", "lambda", "try", "as", "def", "from", "nonlocal", "while", "assert", "del", "global", "not", "with",
	"async", "elif", "if", "or", "yield"
};

// https://www.geeksforgeeks.org/ruby-keywords/
const char* RUBY_KEYWORDS[] = {
	"__ENCODING__", "__LINE__", "__FILE__", "BEGIN", "END", "alias", "and", "begin", "break", "case", "class", "def", "defined?", "do", "else", "elsif", "end ",
	"ensure", "false", "for", "if", "in", "module", "next", "nil", "not", "or", "redo", "rescue", "retry", "return", "self", "super", "then", "true", "undef",
	"unless", "until", "when", "while", "yield"
};

// https://doc.rust-lang.org/reference/keywords.html#reserved-keywords
const char* RUST_KEYWORDS[] = {
	"as", "break", "const", "continue", "crate", "else", "enum", "extern", "false", "fn", "for", "if", "impl", "in", "let", "loop", "match", "mod", "move", "mut",
	"pub", "ref", "return", "self", "Self", "static", "struct", "super", "trait", "true", "type", "unsafe", "use", "where", "while", "async", "await", "dyn",
	"abstract", "become", "box", "do", "final", "macro", "override", "priv", "typeof", "unsized", "virtual", "yield"
};

const char* SUPPORTED_FILE_EXTENSIONS[] = {
	".c",
	".h",
	".css",
	".cpp",
	".hpp",
	".html",
	".java",
	".js",
	".txt",
	"",
	".py",
	".rb",
	".rs"
};

int currentLanguage = 9;
ab_string* filename;

int nTags1 = 0;
int nTags2 = 0;
int nTags3 = 0;

GtkWidget* window;
GtkWidget* scrolled_window;
GtkWidget* grid;
GtkWidget* text_view;
GtkWidget* label;
GtkWidget* combo_box_text;

void clear_tags(void) {
	GtkTextIter start, end;
	gtk_text_buffer_get_start_iter(gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view)), &start);
	gtk_text_buffer_get_end_iter(gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view)), &end);
	gtk_text_buffer_remove_all_tags(gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view)), &start, &end);
	nTags1 = nTags2 = nTags3 = 0;
}

void add_tag(int i1, int i2, int tag_type) {
	// https://developer.gnome.org/gtk3/stable/TextWidget.html
	GtkTextIter start, end;
	GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
	gtk_text_buffer_get_iter_at_offset(buffer, &start, i1);
	gtk_text_buffer_get_iter_at_offset(buffer, &end, i2);
	switch(tag_type) {
		case 1:
			{
				char name[20];
				// https://stackoverflow.com/questions/5172107/how-to-concatenate-string-and-int-in-c
				snprintf(name, 20, "special1_%d", nTags1++);
				gtk_text_buffer_apply_tag(buffer, gtk_text_buffer_create_tag(gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view)), name, "foreground", "red", NULL), &start, &end);
			}
			break;
		case 2:
			{
				char name[20];
				// https://stackoverflow.com/questions/5172107/how-to-concatenate-string-and-int-in-c
				snprintf(name, 20, "special2_%d", nTags2++);
				gtk_text_buffer_apply_tag(buffer, gtk_text_buffer_create_tag(gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view)), name, "foreground", "green", NULL), &start, &end);
			}
			break;
		case 3:
			{
				char name[20];
				// https://stackoverflow.com/questions/5172107/how-to-concatenate-string-and-int-in-c
				snprintf(name, 20, "special3_%d", nTags3++);
				gtk_text_buffer_apply_tag(buffer, gtk_text_buffer_create_tag(gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view)), name, "foreground", "blue", NULL), &start, &end);
			}
			break;
		default:
			break;
	}
}

void save_file(char* fn) {
	FILE* fp = fopen(fn, "w");
	if(fp != NULL) {
		if(strcmp(fn, filename->value) != 0) { ab_string_replace(filename, fn, strlen(fn)); }
		GtkTextIter iter_start, iter_end;
		gtk_text_buffer_get_start_iter(gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view)), &iter_start);
		gtk_text_buffer_get_end_iter(gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view)), &iter_end);
		char* buffer = gtk_text_buffer_get_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view)), &iter_start, &iter_end, FALSE);
		fwrite(buffer, strlen(buffer), 1, fp);
		fclose(fp);
	}
}

void buffer_changed(GtkTextBuffer* buffer, gpointer user_data) {
	clear_tags();
	GtkTextIter start, end;
	gtk_text_buffer_get_start_iter (buffer, &start);
	gtk_text_buffer_get_end_iter (buffer, &end);
	char* text = gtk_text_buffer_get_text (buffer, &start, &end, FALSE); 
	switch(currentLanguage) {
		// TODO: this
		case 0:
			{
				// https://stackoverflow.com/questions/11088338/how-to-save-content-of-gtktextbuffer-to-a-file#:~:text=First%2C%20get%20the%20GtkTextBuffer%20from%20the%20GtkTextView%20using,of%20your%20choice%2C%20however%2C%20I%20would%20recommend%20Gio.
				char str_end;
				char* str;
				for(int i=0;i<sizeof(C_KEYWORDS)/sizeof(C_KEYWORDS[0]);i++) {
					// https://stackoverflow.com/questions/44619093/how-to-find-all-occurrences-and-all-position-of-a-substring-in-a-string
					for(str = text; (str = strstr(str, C_KEYWORDS[i])) != NULL; str++) {
						if(
							(
							str - text - 1 < 0 ||
							(!isalnum(text[str - text - 1]) && text[str - text - 1] != '_')
							)
							&&
							(
							str - text + strlen(C_KEYWORDS[i]) >= strlen(text) ||
							(!isalnum(text[str - text + strlen(C_KEYWORDS[i])]) && text[str - text + strlen(C_KEYWORDS[i])] != '_')
							)
						) {
							add_tag(str - text, str - text + strlen(C_KEYWORDS[i]), 1);
						}
					}
				}
				int open = 0;
				int last_index = -1;
				for(str = text; (str = strstr(str, "\"")) != NULL; str++) {
					if(str - text - 1 >= 0 && text[str - text - 1] == '\\') { continue; }
					if(open == 1 && last_index != -1) { add_tag(last_index, str - text + 1, 2); }
					open = !open;
					last_index = str - text;
				}
			}
			break;
		case 1:
			{
				// https://stackoverflow.com/questions/11088338/how-to-save-content-of-gtktextbuffer-to-a-file#:~:text=First%2C%20get%20the%20GtkTextBuffer%20from%20the%20GtkTextView%20using,of%20your%20choice%2C%20however%2C%20I%20would%20recommend%20Gio.
				char str_end;
				char* str;
				for(int i=0;i<sizeof(C_KEYWORDS)/sizeof(C_KEYWORDS[0]);i++) {
					// https://stackoverflow.com/questions/44619093/how-to-find-all-occurrences-and-all-position-of-a-substring-in-a-string
					for(str = text; (str = strstr(str, C_KEYWORDS[i])) != NULL; str++) {
						if(
							(
							str - text - 1 < 0 ||
							(!isalnum(text[str - text - 1]) && text[str - text - 1] != '_')
							)
							&&
							(
							str - text + strlen(C_KEYWORDS[i]) >= strlen(text) ||
							(!isalnum(text[str - text + strlen(C_KEYWORDS[i])]) && text[str - text + strlen(C_KEYWORDS[i])] != '_')
							)
						) {
							add_tag(str - text, str - text + strlen(C_KEYWORDS[i]), 1);
						}
					}
				}
				int open = 0;
				int last_index = -1;
				for(str = text; (str = strstr(str, "\"")) != NULL; str++) {
					if(str - text - 1 >= 0 && text[str - text - 1] == '\\') { continue; }
					if(open == 1 && last_index != -1) { add_tag(last_index, str - text + 1, 2); }
					open = !open;
					last_index = str - text;
				}
			}
			break;
		case 2:
			{
				// https://stackoverflow.com/questions/11088338/how-to-save-content-of-gtktextbuffer-to-a-file#:~:text=First%2C%20get%20the%20GtkTextBuffer%20from%20the%20GtkTextView%20using,of%20your%20choice%2C%20however%2C%20I%20would%20recommend%20Gio.
				char str_end;
				char* str;
				for(int i=0;i<sizeof(HTML_KEYWORDS)/sizeof(HTML_KEYWORDS[0]);i++) {
					// https://stackoverflow.com/questions/44619093/how-to-find-all-occurrences-and-all-position-of-a-substring-in-a-string
					for(str = text; (str = strstr(str, HTML_KEYWORDS[i])) != NULL; str++) {
						if(
							(
							str - text - 1 < 0 ||
							(!isalnum(text[str - text - 1]) && text[str - text - 1] != '_')
							)
							&&
							(
							str - text + strlen(HTML_KEYWORDS[i]) >= strlen(text) ||
							(!isalnum(text[str - text + strlen(HTML_KEYWORDS[i])]) && text[str - text + strlen(HTML_KEYWORDS[i])] != '_')
							)
						) {
							add_tag(str - text, str - text + strlen(HTML_KEYWORDS[i]), 1);
						}
					}
				}
				int open = 0;
				int last_index = -1;
				for(str = text; (str = strstr(str, "\"")) != NULL; str++) {
					if(str - text - 1 >= 0 && text[str - text - 1] == '\\') { continue; }
					if(open == 1 && last_index != -1) { add_tag(last_index, str - text + 1, 2); }
					open = !open;
					last_index = str - text;
				}
				for(str = text; (str = strstr(str, "\'")) != NULL; str++) {
					if(str - text - 1 >= 0 && text[str - text - 1] == '\\') { continue; }
					if(open == 1 && last_index != -1) { add_tag(last_index, str - text + 1, 2); }
					open = !open;
					last_index = str - text;
				}
			}
			break;
		case 3:
			{
				// https://stackoverflow.com/questions/11088338/how-to-save-content-of-gtktextbuffer-to-a-file#:~:text=First%2C%20get%20the%20GtkTextBuffer%20from%20the%20GtkTextView%20using,of%20your%20choice%2C%20however%2C%20I%20would%20recommend%20Gio.
				char str_end;
				char* str;
				for(int i=0;i<sizeof(CPP_KEYWORDS)/sizeof(CPP_KEYWORDS[0]);i++) {
					// https://stackoverflow.com/questions/44619093/how-to-find-all-occurrences-and-all-position-of-a-substring-in-a-string
					for(str = text; (str = strstr(str, CPP_KEYWORDS[i])) != NULL; str++) {
						if(
							(
							str - text - 1 < 0 ||
							(!isalnum(text[str - text - 1]) && text[str - text - 1] != '_')
							)
							&&
							(
							str - text + strlen(CPP_KEYWORDS[i]) >= strlen(text) ||
							(!isalnum(text[str - text + strlen(CPP_KEYWORDS[i])]) && text[str - text + strlen(CPP_KEYWORDS[i])] != '_')
							)
						) {
							add_tag(str - text, str - text + strlen(CPP_KEYWORDS[i]), 1);
						}
					}
				}
				int open = 0;
				int last_index = -1;
				for(str = text; (str = strstr(str, "\"")) != NULL; str++) {
					if(str - text - 1 >= 0 && text[str - text - 1] == '\\') { continue; }
					if(open == 1 && last_index != -1) { add_tag(last_index, str - text + 1, 2); }
					open = !open;
					last_index = str - text;
				}
			}
			break;
		case 4:
			{
				// https://stackoverflow.com/questions/11088338/how-to-save-content-of-gtktextbuffer-to-a-file#:~:text=First%2C%20get%20the%20GtkTextBuffer%20from%20the%20GtkTextView%20using,of%20your%20choice%2C%20however%2C%20I%20would%20recommend%20Gio.
				char str_end;
				char* str;
				for(int i=0;i<sizeof(CPP_KEYWORDS)/sizeof(CPP_KEYWORDS[0]);i++) {
					// https://stackoverflow.com/questions/44619093/how-to-find-all-occurrences-and-all-position-of-a-substring-in-a-string
					for(str = text; (str = strstr(str, CPP_KEYWORDS[i])) != NULL; str++) {
						if(
							(
							str - text - 1 < 0 ||
							(!isalnum(text[str - text - 1]) && text[str - text - 1] != '_')
							)
							&&
							(
							str - text + strlen(CPP_KEYWORDS[i]) >= strlen(text) ||
							(!isalnum(text[str - text + strlen(CPP_KEYWORDS[i])]) && text[str - text + strlen(CPP_KEYWORDS[i])] != '_')
							)
						) {
							add_tag(str - text, str - text + strlen(CPP_KEYWORDS[i]), 1);
						}
					}
				}
				int open = 0;
				int last_index = -1;
				for(str = text; (str = strstr(str, "\"")) != NULL; str++) {
					if(str - text - 1 >= 0 && text[str - text - 1] == '\\') { continue; }
					if(open == 1 && last_index != -1) { add_tag(last_index, str - text + 1, 2); }
					open = !open;
					last_index = str - text;
				}
			}
			break;
		case 5:
			{
				// https://stackoverflow.com/questions/11088338/how-to-save-content-of-gtktextbuffer-to-a-file#:~:text=First%2C%20get%20the%20GtkTextBuffer%20from%20the%20GtkTextView%20using,of%20your%20choice%2C%20however%2C%20I%20would%20recommend%20Gio.
				char str_end;
				char* str;
				for(int i=0;i<sizeof(HTML_KEYWORDS)/sizeof(HTML_KEYWORDS[0]);i++) {
					// https://stackoverflow.com/questions/44619093/how-to-find-all-occurrences-and-all-position-of-a-substring-in-a-string
					for(str = text; (str = strstr(str, HTML_KEYWORDS[i])) != NULL; str++) {
						if(
							(
							str - text - 1 < 0 ||
							(!isalnum(text[str - text - 1]) && text[str - text - 1] != '_')
							)
							&&
							(
							str - text + strlen(HTML_KEYWORDS[i]) >= strlen(text) ||
							(!isalnum(text[str - text + strlen(HTML_KEYWORDS[i])]) && text[str - text + strlen(HTML_KEYWORDS[i])] != '_')
							)
						) {
							add_tag(str - text, str - text + strlen(HTML_KEYWORDS[i]), 1);
						}
					}
				}
				for(str = text; (str = strstr(str, "<")) != NULL; str++) {
					add_tag(str - text, str - text + 1, 3);
				}
				for(str = text; (str = strstr(str, ">")) != NULL; str++) {
					add_tag(str - text, str - text + 1, 3);
				}
				int open = 0;
				int last_index = -1;
				for(str = text; (str = strstr(str, "\"")) != NULL; str++) {
					if(str - text - 1 >= 0 && text[str - text - 1] == '\\') { continue; }
					if(open == 1 && last_index != -1) { add_tag(last_index, str - text + 1, 2); }
					open = !open;
					last_index = str - text;
				}
				for(str = text; (str = strstr(str, "\'")) != NULL; str++) {
					if(str - text - 1 >= 0 && text[str - text - 1] == '\\') { continue; }
					if(open == 1 && last_index != -1) { add_tag(last_index, str - text + 1, 2); }
					open = !open;
					last_index = str - text;
				}
			}
			break;
		case 6:
			{
				// https://stackoverflow.com/questions/11088338/how-to-save-content-of-gtktextbuffer-to-a-file#:~:text=First%2C%20get%20the%20GtkTextBuffer%20from%20the%20GtkTextView%20using,of%20your%20choice%2C%20however%2C%20I%20would%20recommend%20Gio.
				char str_end;
				char* str;
				for(int i=0;i<sizeof(JAVA_KEYWORDS)/sizeof(JAVA_KEYWORDS[0]);i++) {
					// https://stackoverflow.com/questions/44619093/how-to-find-all-occurrences-and-all-position-of-a-substring-in-a-string
					for(str = text; (str = strstr(str, JAVA_KEYWORDS[i])) != NULL; str++) {
						if(
							(
							str - text - 1 < 0 ||
							(!isalnum(text[str - text - 1]) && text[str - text - 1] != '_')
							)
							&&
							(
							str - text + strlen(JAVA_KEYWORDS[i]) >= strlen(text) ||
							(!isalnum(text[str - text + strlen(JAVA_KEYWORDS[i])]) && text[str - text + strlen(JAVA_KEYWORDS[i])] != '_')
							)
						) {
							add_tag(str - text, str - text + strlen(JAVA_KEYWORDS[i]), 1);
						}
					}
				}
				int open = 0;
				int last_index = -1;
				for(str = text; (str = strstr(str, "\"")) != NULL; str++) {
					if(str - text - 1 >= 0 && text[str - text - 1] == '\\') { continue; }
					if(open == 1 && last_index != -1) { add_tag(last_index, str - text + 1, 2); }
					open = !open;
					last_index = str - text;
				}
				for(str = text; (str = strstr(str, "\'")) != NULL; str++) {
					if(str - text - 1 >= 0 && text[str - text - 1] == '\\') { continue; }
					if(open == 1 && last_index != -1) { add_tag(last_index, str - text + 1, 2); }
					open = !open;
					last_index = str - text;
				}
			}
			break;
		case 7:
			{
				// https://stackoverflow.com/questions/11088338/how-to-save-content-of-gtktextbuffer-to-a-file#:~:text=First%2C%20get%20the%20GtkTextBuffer%20from%20the%20GtkTextView%20using,of%20your%20choice%2C%20however%2C%20I%20would%20recommend%20Gio.
				char str_end;
				char* str;
				for(int i=0;i<sizeof(JS_KEYWORDS)/sizeof(JS_KEYWORDS[0]);i++) {
					// https://stackoverflow.com/questions/44619093/how-to-find-all-occurrences-and-all-position-of-a-substring-in-a-string
					for(str = text; (str = strstr(str, JS_KEYWORDS[i])) != NULL; str++) {
						if(
							(
							str - text - 1 < 0 ||
							(!isalnum(text[str - text - 1]) && text[str - text - 1] != '_')
							)
							&&
							(
							str - text + strlen(JS_KEYWORDS[i]) >= strlen(text) ||
							(!isalnum(text[str - text + strlen(JS_KEYWORDS[i])]) && text[str - text + strlen(JS_KEYWORDS[i])] != '_')
							)
						) {
							add_tag(str - text, str - text + strlen(JS_KEYWORDS[i]), 1);
						}
					}
				}
				int open = 0;
				int last_index = -1;
				for(str = text; (str = strstr(str, "\"")) != NULL; str++) {
					if(str - text - 1 >= 0 && text[str - text - 1] == '\\') { continue; }
					if(open == 1 && last_index != -1) { add_tag(last_index, str - text + 1, 2); }
					open = !open;
					last_index = str - text;
				}
				for(str = text; (str = strstr(str, "\'")) != NULL; str++) {
					if(str - text - 1 >= 0 && text[str - text - 1] == '\\') { continue; }
					if(open == 1 && last_index != -1) { add_tag(last_index, str - text + 1, 2); }
					open = !open;
					last_index = str - text;
				}
			}
			break;
		case 10:
			{
				// https://stackoverflow.com/questions/11088338/how-to-save-content-of-gtktextbuffer-to-a-file#:~:text=First%2C%20get%20the%20GtkTextBuffer%20from%20the%20GtkTextView%20using,of%20your%20choice%2C%20however%2C%20I%20would%20recommend%20Gio.
				char str_end;
				char* str;
				for(int i=0;i<sizeof(PYTHON_KEYWORDS)/sizeof(PYTHON_KEYWORDS[0]);i++) {
					// https://stackoverflow.com/questions/44619093/how-to-find-all-occurrences-and-all-position-of-a-substring-in-a-string
					for(str = text; (str = strstr(str, PYTHON_KEYWORDS[i])) != NULL; str++) {
						if(
							(
							str - text - 1 < 0 ||
							(!isalnum(text[str - text - 1]) && text[str - text - 1] != '_')
							)
							&&
							(
							str - text + strlen(PYTHON_KEYWORDS[i]) >= strlen(text) ||
							(!isalnum(text[str - text + strlen(PYTHON_KEYWORDS[i])]) && text[str - text + strlen(PYTHON_KEYWORDS[i])] != '_')
							)
						) {
							add_tag(str - text, str - text + strlen(PYTHON_KEYWORDS[i]), 1);
						}
					}
				}
				int open = 0;
				int last_index = -1;
				for(str = text; (str = strstr(str, "\"")) != NULL; str++) {
					if(str - text - 1 >= 0 && text[str - text - 1] == '\\') { continue; }
					if(open == 1 && last_index != -1) { add_tag(last_index, str - text + 1, 2); }
					open = !open;
					last_index = str - text;
				}
				for(str = text; (str = strstr(str, "\'")) != NULL; str++) {
					if(str - text - 1 >= 0 && text[str - text - 1] == '\\') { continue; }
					if(open == 1 && last_index != -1) { add_tag(last_index, str - text + 1, 2); }
					open = !open;
					last_index = str - text;
				}
			}
			break;
		case 11:
			{
				// https://stackoverflow.com/questions/11088338/how-to-save-content-of-gtktextbuffer-to-a-file#:~:text=First%2C%20get%20the%20GtkTextBuffer%20from%20the%20GtkTextView%20using,of%20your%20choice%2C%20however%2C%20I%20would%20recommend%20Gio.
				char str_end;
				char* str;
				for(int i=0;i<sizeof(RUBY_KEYWORDS)/sizeof(RUBY_KEYWORDS[0]);i++) {
					// https://stackoverflow.com/questions/44619093/how-to-find-all-occurrences-and-all-position-of-a-substring-in-a-string
					for(str = text; (str = strstr(str, RUBY_KEYWORDS[i])) != NULL; str++) {
						if(
							(
							str - text - 1 < 0 ||
							(!isalnum(text[str - text - 1]) && text[str - text - 1] != '_')
							)
							&&
							(
							str - text + strlen(RUBY_KEYWORDS[i]) >= strlen(text) ||
							(!isalnum(text[str - text + strlen(RUBY_KEYWORDS[i])]) && text[str - text + strlen(RUBY_KEYWORDS[i])] != '_')
							)
						) {
							add_tag(str - text, str - text + strlen(RUBY_KEYWORDS[i]), 1);
						}
					}
				}
				int open = 0;
				int last_index = -1;
				for(str = text; (str = strstr(str, "\"")) != NULL; str++) {
					if(str - text - 1 >= 0 && text[str - text - 1] == '\\') { continue; }
					if(open == 1 && last_index != -1) { add_tag(last_index, str - text + 1, 2); }
					open = !open;
					last_index = str - text;
				}
				for(str = text; (str = strstr(str, "\'")) != NULL; str++) {
					if(str - text - 1 >= 0 && text[str - text - 1] == '\\') { continue; }
					if(open == 1 && last_index != -1) { add_tag(last_index, str - text + 1, 2); }
					open = !open;
					last_index = str - text;
				}
			}
			break;
		case 12:
			{
				// https://stackoverflow.com/questions/11088338/how-to-save-content-of-gtktextbuffer-to-a-file#:~:text=First%2C%20get%20the%20GtkTextBuffer%20from%20the%20GtkTextView%20using,of%20your%20choice%2C%20however%2C%20I%20would%20recommend%20Gio.
				char str_end;
				char* str;
				for(int i=0;i<sizeof(RUST_KEYWORDS)/sizeof(RUST_KEYWORDS[0]);i++) {
					// https://stackoverflow.com/questions/44619093/how-to-find-all-occurrences-and-all-position-of-a-substring-in-a-string
					for(str = text; (str = strstr(str, RUST_KEYWORDS[i])) != NULL; str++) {
						if(
							(
							str - text - 1 < 0 ||
							(!isalnum(text[str - text - 1]) && text[str - text - 1] != '_')
							)
							&&
							(
							str - text + strlen(RUST_KEYWORDS[i]) >= strlen(text) ||
							(!isalnum(text[str - text + strlen(RUST_KEYWORDS[i])]) && text[str - text + strlen(RUST_KEYWORDS[i])] != '_')
							)
						) {
							add_tag(str - text, str - text + strlen(RUST_KEYWORDS[i]), 1);
						}
					}
				}
				int open = 0;
				int last_index = -1;
				for(str = text; (str = strstr(str, "\"")) != NULL; str++) {
					if(str - text - 1 >= 0 && text[str - text - 1] == '\\') { continue; }
					if(open == 1 && last_index != -1) { add_tag(last_index, str - text + 1, 2); }
					open = !open;
					last_index = str - text;
				}
			}
			break;
		default:
			break;
	}
}

void load_new_file(char* fn) {
	FILE* fp = fopen(fn, "r");
	if(fp != NULL) {
		ab_string_replace(filename, fn, strlen(fn));
		int set_file_type = 0;
		for(int i=0;i<N_SUPPORTED_LANGUAGES;i++) {
			if(i == 9) { continue; }
			else if(
				strlen(fn) > strlen(SUPPORTED_FILE_EXTENSIONS[i]) &&
				strncmp(&fn[strlen(fn) - strlen(SUPPORTED_FILE_EXTENSIONS[i])], SUPPORTED_FILE_EXTENSIONS[i], strlen(SUPPORTED_FILE_EXTENSIONS[i])) == 0
			) {
				currentLanguage = i;
				set_file_type = 1;
				break;
			}
		}
		if(set_file_type == 0) {
			currentLanguage = 9;
		}
		gtk_label_set_text(GTK_LABEL(label), filename->value);
		gtk_combo_box_set_active(GTK_COMBO_BOX(combo_box_text), currentLanguage);

		// https://stackoverflow.com/questions/3381080/reading-all-content-from-a-text-file-c
		char* content;
		long size;
		fseek(fp, 0, SEEK_END); 
		size = ftell(fp);
		fseek(fp, 0, SEEK_SET); 
		content = (char*)malloc(sizeof(char) * size);
		fread(content, 1, size, fp);
		gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view)), content, size);
		free(content);
		fclose(fp);
	}
}

gboolean keypressed(GtkWidget* widget, GdkEventKey* event, gpointer data) {
	if(event->keyval == GDK_KEY_o && (event->state & GDK_CONTROL_MASK) == GDK_CONTROL_MASK) {
		// https://developer.gnome.org/gtk3/stable/GtkFileChooserDialog.html
		GtkWidget* dialog = gtk_file_chooser_dialog_new("Open File", GTK_WINDOW(widget), GTK_FILE_CHOOSER_ACTION_OPEN, ("_Cancel"), GTK_RESPONSE_CANCEL, ("_Open"), GTK_RESPONSE_ACCEPT, NULL);
		gint result = gtk_dialog_run(GTK_DIALOG(dialog));
		if(result == GTK_RESPONSE_ACCEPT) {
			char* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
			load_new_file(filename);
			g_free(filename);
		}
		gtk_widget_destroy(dialog);
		return TRUE;
	}
	else if(event->keyval == GDK_KEY_s && (event->state & GDK_CONTROL_MASK) == GDK_CONTROL_MASK) {
		// https://developer.gnome.org/gtk3/stable/GtkFileChooserDialog.html
		GtkWidget* dialog = gtk_file_chooser_dialog_new("Save File", GTK_WINDOW(widget), GTK_FILE_CHOOSER_ACTION_SAVE, ("_Cancel"), GTK_RESPONSE_CANCEL, ("_Save"), GTK_RESPONSE_ACCEPT, NULL);
		GtkFileChooser* chooser = GTK_FILE_CHOOSER(dialog);
		gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);
		gtk_file_chooser_set_filename(chooser, filename->value);
		gint result = gtk_dialog_run(GTK_DIALOG(dialog));
		if(result == GTK_RESPONSE_ACCEPT) {
			char* filename = gtk_file_chooser_get_filename(chooser);
			save_file(filename);
			g_free(filename);
		}
		gtk_widget_destroy(dialog);
		return TRUE;
	}
	return FALSE;
}

void activate(GtkApplication* app, gpointer user_data) {
	filename = ab_string_from("Untitled", 8);
	ab_string_add_str(filename, ab_string_from(SUPPORTED_FILE_EXTENSIONS[currentLanguage], strlen(SUPPORTED_FILE_EXTENSIONS[currentLanguage])));

	

	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), WINDOW_TITLE);
	gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);
	// https://stackoverflow.com/questions/4772804/how-to-set-a-fixed-windows-size-for-a-gtk-app-in-c
	gtk_window_set_resizable (GTK_WINDOW(window), FALSE);
	gtk_container_set_border_width(GTK_CONTAINER(window), WINDOW_BORDER_WIDTH);

	grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(window), grid);

	scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 0, 2, 38);

	text_view = gtk_text_view_new();
	gtk_widget_set_hexpand(text_view, TRUE);
	gtk_widget_set_vexpand(text_view, TRUE);

	gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);

	label = gtk_label_new(filename->value);
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 39, 1, 1);

	combo_box_text = gtk_combo_box_text_new();
	for(int i=0;i<N_SUPPORTED_LANGUAGES;i++) {
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box_text), SUPPORTED_LANGUAGES[i]);
	}
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_box_text), currentLanguage);
	gtk_widget_set_hexpand(combo_box_text, TRUE);
	gtk_widget_set_vexpand(combo_box_text, TRUE);
	gtk_grid_attach(GTK_GRID(grid), combo_box_text, 1, 39, 1, 1);

	// https://riptutorial.com/gtk3/example/26488/css-in-action
	GtkCssProvider* css_provider = gtk_css_provider_new();
	gtk_css_provider_load_from_path(css_provider, "css/style.css", NULL);
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	// https://stackoverflow.com/questions/44098084/how-do-i-handle-keyboard-events-in-gtk3
	gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);
	g_signal_connect(G_OBJECT(window), "key_press_event", G_CALLBACK(keypressed), NULL);

	g_signal_connect(gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view)), "changed", G_CALLBACK(buffer_changed), NULL);

	gtk_widget_show_all(window);
}

int main(int argc, char** argv) {
	GtkApplication* app;
	int status;
	app = gtk_application_new("net.spiderfencer.test", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	ab_string_destroy(filename);
	return status;
}
