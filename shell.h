#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chainning */
#define CMD_NORM	0
#define CDM_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 is using  system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;

/**
 * struct liststr - single link list
 * @num: the number field
 * @str: a string
 * @next: ponits to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 * struct info - contains pseudo-arguments to pass into a function,
 * allowing uniform prototype for function pointer struct
 * @arg: a string generated from getline containing arguementd
 * @argv: an array of dtring generated from agr
 * @path: a string path for the current command
 * @argc: the argument count
 * @line_count: the error count
 * @err_num: the error code for exit()s
 * @linecount-flag: if on count this line of input
 * @fname: the program filename
 * @env: linked list local copy of environ
 * @environ: custom modified copy of environ from LL env
 * @history: the history node
 * @alias: the alias node
 * @env_changed: on if environ was changed
 * @status: the return status of the last exec'd command
 * @cmd_buf: address of pointer to cmd_buf, on if chaining
 * @cmd_buf_type: CMD_type ||, &&, ;
 * @readfd:  the fd from which to read line input
 * @histcount: the history line number count
 * @program_name: the name of the excutable
 * @input_line: pointer to the input rear for _getline
 * @command_name: pointwer to the first commnad
 * @exec_counter: number of excuted commands
 * @file_descriptor: file descriptor to the input of commands
 * @tokens: pointer to arrary to tokenized input
 * @env: copy of the environ
 * @alias_list: array of pointer with aliases
 */
typedef struct info
{
	char *arg;
	char **argv;
	char *path;
	char *argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environs;
	int env_changed;
	int status;
	char **cmd_buf;
	int cmd_buf_type;
	int readfd;
	int histcount;
	char *program_name;
	char *input_line;
	char *command_name;
	char exec_counter;
	int file_descriptor;
	char **tokens;
	char **env;
	char **alias_list;
} data_of_program;

/**
 * struct builtins - struct for the builtins
 * @builtin: the name of the builtin
 * @function: the associated function to be called for each builtin
 */
typedef struct builtins
{
	char *builtin;
	int (*function)(data_of_program *data);
} builtins;

/* shell.c */
void inicialize_data(data_of_program *data, int arc, char *argv[], char **env);
void sisifo(char *prompt, data_of_program *data);
void handle_ctrl_c(int opr UNUSED);

/* _getline.c */
int _getline(data_of_program *data);
int check_logic_ops(char *array_commands[], int i, char array_operators[]);

/* expansion.c */
void expand_alias(data_of_program *data);
void expand_variables(data_of_program *data);
int buffer_add(char *buffer, char *str_to_add);

/* str_tok.c */
void tokenize(data_of_program *data);
char *_strtok(char *line, char *delim);

/* execute.c */
int execute(data_of_program *data);

/* builtins_list.c */
int builtins_list(data_of_program *data);

/* find_in_path.c */
char **tokenize_path(data_of_program *data);
int find_program(data_of_program *data);

/* HELPER FOR MEMORY MANAGEMENT */
/* helpers.free.c */
void free_array_of_pointers(char **directories);
void free_recurrent_data(data_of_program *data);
void free_all_data(data_of_program *data);

/* builtins_more.c */
int builtin_exit(data_of_program *data);
int builtin_cd(data_of_program *data);
int set_work_directory(data_of_program *data, char *new_dir);
int builtin_help(data_of_program *data);
int builtin_alias(data_of_program *data);

/* builtins_env.c */
int builtin_env(data_of_program *data);
int builtin_set_env(data_of_program *data);
int builtin_unset_env(data_of_program *data);

/* env_management.c */
char *env_get_key(char *name, data_of_program *data);
int env_set_key(char *key, char *value, data_of_program *data);
int env_remove_key(char *key, data_of_program *data);
void print_environ(data_of_program *data);

/* helpers_print.c */
int _print(char *string);
int _printe(char *string);
int _print_error(int errorcode, data_of_program *data);

/* helper_string.c */
int str_length(char *string);
char *str_duplicate(char *string);
int str_compare(char *string1, char *string2, int number);
char *str_concat(char *string1, char *string2);
void str_reverse(char *string);

/* helpers_numbers.c */
void long_to_string(long number, char *string, int base);
int _atoi(char *s);
int count_characters(char *string, char *character);

/* alias_management.c */
int print_alias(data_of_program *data, char *alias);
char *get_alias(data_of_program *data, char *alias);
int set_alias(char *alias_string, data_of_program *data);

#endif
