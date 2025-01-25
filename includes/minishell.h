/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 11:28:18 by asaiz-lo          #+#    #+#             */
/*   Updated: 2025/01/25 12:14:17 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/wait.h>
# include <unistd.h>

# define STDIN 0
# define STDOUT 1
# define STDERR 2

# define SKIP 1
# define NOSKIP 0

# define BUFF_SIZE 4096
# define EXPANSION -36
# define UKNOWN_TYPE "minishell: syntax error near unexpected token"
# define SYNTAX_ERROR "minishell: syntax error near unexpected token `"
# define INPUT_ERROR "Error Opening Input File \n"
# define INPUT_REDIR_ERROR "Error Redirecting STDIN \n"
# define OUTPUT_ERROR "Error Opening Output File \n"
# define OUTPUT_REDIR_ERROR "Error Redirecting STDOUT \n"
# define REDIR_ERROR "Redirection Error \n"
# define EXEC_ERROR "Builtin Execution Failed \n"
# define READ 0
# define WRITE 1
# define HEREDOC -42
# define ERR_SUCCESS 0
# define ERR_GENERAL 1
# define ERR_CMD_NOT_FOUND 127
# define ERR_NOT_EXECUTABLE 126
# define ERR_EXEC_FAILED 1
# define ERR_INPUT_FILE_NOT_FOUND 1
# define ERR_INPUT_PERMISSION_DENIED 126
# define ERR_HEREDOC_FAILURE 1
# define ERR_OUTPUT_FILE_CREATION 1
# define ERR_OUTPUT_PERMISSION_DENIED 126
# define ERR_BUILTIN_USAGE 2
# define ERR_EXIT_INVALID_NUM 128
# define ERR_ENV_VAR_NOT_FOUND 1
# define ERR_SIGINT 130
# define ERR_SIGQUIT 131
# define ERR_SIGNAL_BASE 128
# define ERR_PIPE_CREATION 6
# define ERR_PIPE_WRITE 7
# define ERR_PIPE_EXECUTION 8
# define ERR_INVALID_ARG 1
# define ERR_RETURN_BASE 128
# define ERR_FILE_NOT_FOUND 2
# define ERR_DIRECTORY_NOT_FOUND 1
# define ERR_PERMISSION_DENIED 126

# define BUFF_SIZE 4096
# define EXPANSION -36
# define ERROR 1
# define SUCCESS 0
# define IS_DIRECTORY 126
# define ERR_CMD_NOT_FOUND 127

typedef enum s_tokens
{
	NONE = 1,
	PIPE = 2,
	PIPE_PIPE = 3,
	GREAT = 4,
	GREAT_GREAT = 5,
	LESS = 6,
	LESS_LESS = 7,
}						t_tokens;

typedef struct s_lexer
{
	char				*str;
	t_tokens			token;
	int					i;
	bool				is_separator;
	struct s_lexer		*next;
	struct s_lexer		*prev;
}						t_lexer;

typedef struct s_env
{
	char				*value;
	struct s_env		*next;
}						t_env;

typedef struct s_mini
{
	t_tokens			*start;
	t_env				*env;
	t_env				*secret_env;
	int					input;
	int					output;
	int					fd_input;
	int					fd_output;
	int					pipe_input;
	int					pipe_output;
	int					pid;
	int					charge;
	int					parent;
	int					last;
	int					return_code;
	int					exit_flag;
	int					skip_exec;
}						t_mini;

typedef struct s_sig
{
	int					sigint;
	int					sigquit;
	int					exit_status;
	int					in_heredoc;
	pid_t				pid;
}						t_sig;

typedef struct s_input
{
	char				*file;
	bool				condition;
	bool				expanded;
	struct s_input		*next;
}						t_input;

typedef struct s_command
{
	char				*command;
	char				**args;
	size_t				args_size;
	t_input				*input;
	t_input				*output;
	int					fd_in;
	int					fd_out;
	char				*heredoc;
	bool				has_pipe;
	bool				append;
	struct s_command	*next;
}						t_command;

typedef struct s_exec_context
{
	int					saved_in;
	int					saved_out;
	char				*path;
	char				**bin_paths;
	char				**env_array;
	char				**exec_args;
}						t_exec_context;

typedef struct s_pipeline_context
{
	int					in_fd;
	int					pipe_fd[2];
	int					saved_stdin;
	int					saved_stdout;
	pid_t				child_pids[1024];
	int					child_count;
	int					status;
}						t_pipeline_context;

typedef struct s_redirection_context
{
	int					saved_stdin;
	int					saved_stdout;
	int					*in_fd;
	char				**err;
}						t_redirection_context;

typedef struct s_pipeline_setup_context
{
	int					*in_fd;
	int					saved_stdin;
	int					saved_stdout;
	int					*pipe_fd;
}						t_pipeline_setup_context;

typedef struct s_pipe_context
{
	int					*pipe_fd;
	int					in_fd;
	int					saved_stdin;
	int					saved_stdout;
}						t_pipe_context;

typedef struct s_fork_error_context
{
	t_command			*current;
	int					*pipe_fd;
	int					in_fd;
	int					saved_stdin;
	int					saved_stdout;
}						t_fork_error_context;

typedef struct s_fork_error_init
{
	t_command			*current;
	int					*pipe_fd;
	int					in_fd;
	int					saved_stdin;
	int					saved_stdout;
}						t_fork_error_init;

typedef struct s_parent_process_context
{
	t_command			*current;
	int					pid;
	int					*pipe_fd;
	int					*in_fd;
	int					*child_pids;
	int					*child_count;
	int					*status;
}						t_parent_process_context;

typedef struct s_input_redirection_context
{
	int					*in_fd;
	int					saved_stdin;
	int					saved_stdout;
}						t_input_redirection_context;

extern t_sig			g_sig;

// Utils
void					print_lexer_tokens(t_lexer *lex_list);
void					print_shlvl(t_env *env);
void					print_command_list(t_command *cmd);
void					print_single_command(t_command *cmd);
int						is_types(t_lexer *token, char *types);
int						check_line(t_mini *mini, t_lexer *token);
void					print_error(char *arg);
void					cleanup_readline(void);
char					*get_command(void);

// File Descriptor Management (fd_management.c)
void					mini_init(t_mini *mini);
void					init_fds(t_mini *mini);
void					close_fds(t_mini *mini);

// Inicializacion de Environment Variables (env_init.c)
int						env_init(t_mini *mini, char **env_array);
int						is_in_env(t_env *env, const char *key);
void					free_env(t_env *env);
void					minishell_loop(t_mini *mini);
int						env_update(t_env *env, const char *value);

// Lexer
char					*get_command(void);
bool					valid_quotes(char *command);
int						reorder_tokens(t_lexer **list);
t_tokens				is_token(char c);
t_lexer					*lexer(char *input);

// Parser
t_command				*parser(t_lexer *list);
t_command				*innit_new(void);
int						add_token_to_command(t_command *new, t_lexer **tmp);
bool					add_input_node(t_input **lst, char *str,
							bool condition);
t_command				*init_new(t_lexer *tmp);
t_command				*next_command(t_command *cmd_list, bool skip);
void					free_command_node(t_command *cmd);

// Expander
char					*expand(char *str, t_env *env);
void					print_list_inp(t_input *l);
void					expand_input(t_input *list, t_env *env, bool out_mode);
void					replace(char **str, int *i, int len, t_env *env);
int						expander(t_command *list, t_env *env);

// Redirects
int						redirect(t_command *list, char **err);
int						save_std_fds(int *saved_in, int *saved_out);
void					print_input_error(int redir_status, char *err);
void					print_output_error(int redir_status, char *err);
void					print_redirection_error(int redir_status, char *err);
int						handle_redirection_error(int redir_status, char *err,
							int saved_in, int saved_out);
int						setup_redirection(t_command *cmd, int saved_in,
							int saved_out, char **err);
char					**allocate_exec_args(t_command *cmd);
int						populate_exec_args(char **exec_args, t_command *cmd);
int						initialize_exec_context(t_exec_context *ctx);
int						prepare_command_execution(t_command *cmd,
							t_exec_context *ctx, char **err);
t_env					*find_path_env(t_env *env);
int						handle_path_not_set(t_exec_context *ctx);
int						handle_command_not_found(t_command *cmd,
							t_exec_context *ctx);
int						check_direct_command(t_command *cmd,
							t_exec_context *ctx);
int						check_path_command(const char *bin_path,
							const char *command, char **result_path);
void					cleanup_command_resources(t_exec_context *ctx,
							t_command *cmd);
void					cleanup_path_resources(t_exec_context *ctx);
int						check_all_bin_paths(char **bin_paths,
							const char *command, char **result_path);
int						resolve_command_path(t_command *cmd, t_mini *mini,
							t_exec_context *ctx);
int						execute_in_child(t_exec_context *ctx, t_mini *mini);
void					update_exit_status(pid_t pid);
int						redir_and_execute(t_command *cmd, t_mini *mini);

// Redirection Management
void					save_and_restore_std(int *saved_in, int *saved_out,
							int fd_in, int fd_out);
void					restore_std(int saved_in, int saved_out);
int						redir_and_execute(t_command *cmd, t_mini *mini);
char					*path_join(const char *bin, const char *command);
void					free_tab(char **tab);
char					*env_to_str(t_env *env);
void					cleanup_exec_resources(char *path, char **bin_paths,
							char **exec_args, char **env_array);
char					*check_dir(char *bin, char *command);
int						open_in(t_input *in, t_command *list);
int						open_out(t_input *out, t_command *list);
int						redirect(t_command *list, char **err);
int						redirect_output_pipe(t_command *cmd, char **err);
int						handle_redirection(int (*open_func)(t_input *,
								t_command *), t_input *redir, t_command *list,
							char **err);
int						heredoc(const char *delim);

// Built-In Commands
int						is_builtin(char *command);
int						execute_builtin(t_command *cmd, t_mini *mini);
int						ft_echo(char **args);
int						ft_env(t_env *env);
int						ft_cd(char **args, t_env *env);
int						ft_pwd(void);
int						ft_exit(t_command *c);
int						ft_export(t_command *c, t_mini *mini);
int						ft_unset(t_command *c, t_mini *mini);
void					print_err_msg_export(char *str);
int						create_and_attach_new_node(t_env *current,
							const char *value);
t_env					*find_or_update_node(t_env *env, const char *value,
							int *result);
int						env_add(const char *value, t_env *env);
char					*resolve_path(const char *path, const char *cwd);
int						update_env_var(t_env *env, const char *key,
							const char *value);
char					*env_value_cd(const char *key, t_env *env);
char					*get_env_value_cd(const char *key, t_env *env);
int						get_current_directory(char *cwd);
char					*resolve_target_path(char **args, t_env *env);
char					*resolve_and_validate_path(const char *target_path,
							const char *cwd);

// Free
void					free_input_list(t_input **in);
void					cleanup_redirections(t_input *list);
void					cleanup_command_list(t_command *command_list);
void					free_lexer_list(t_lexer *lexer_list);
void					free_args(char **args);
void					free_command_node(t_command *cmd);
void					free_command_resources(t_command *cmd);
void					free_args_list(char **args, size_t args_size);
void					free_command_list(t_command *cmd);

// Shell Nesting
int						shell_nesting_increment(t_env *env);
int						env_value_len(const char *env);
char					*env_value(char *env);
char					*get_env_name(char *dest, const char *src);
char					*get_env_value(char *arg, t_env *env);
int						env_update(t_env *env, const char *value);

// Signal Handling
void					handle_sigint(int sig);
void					handle_eof(t_mini *mini);
void					handle_ignore(int sig);
void					setup_signals(void);

// Pipe Management
int						setup_pipe(int *pipe_fd);
int						write_heredoc_line(int pipe_write_fd, const char *line);
int						process_heredoc_input(int *pipe_fd, const char *delim);
int						create_heredoc_pipe(int *pipe_fd);
int						handle_heredoc(const char *delim);
void					print_file_error(char *prefix, char *file,
							char *suffix);
int						process_heredoc(char *file);
int						open_input_file(char *file);
void					update_fd_in(t_command *cmd, int new_fd);
int						handle_input_condition(t_input *input, int *fd);
int						process_input_redirections(t_command *cmd);
void					close_unused_pipes(int pipe_fd[2], int has_pipe);
int						save_std_fds_pipe(int *saved_stdin, int *saved_stdout);
int						handle_input_redirection_pipe(t_command *current,
							t_input_redirection_context *ctx);
void					update_in_fd(t_command *current, int *in_fd);
void					handle_output_redirection_error(char *err,
							int output_status, int saved_stdin,
							int saved_stdout);
int						handle_pipe_creation(int *pipe_fd, int in_fd,
							int saved_stdin, int saved_stdout);
void					execute_child_process_pipe(t_command *current,
							t_mini *mini, int in_fd, int *pipe_fd);
int						initialize_pipeline_context(t_pipeline_context *ctx);
int						process_parent_for_command(t_command *current,
							pid_t pid, t_pipeline_context *ctx);
int						process_single_command(t_command *current, t_mini *mini,
							t_pipeline_context *ctx);
int						process_pipeline_commands(t_command *cmd_list,
							t_mini *mini, t_pipeline_context *ctx);
int						finalize_pipeline_context(t_pipeline_context *ctx);
int						execute_pipeline(t_command *cmd_list, t_mini *mini);
int						handle_input_redirection(int fd_in,
							t_exec_context *ctx);
int						handle_output_redirection(int fd_out,
							t_exec_context *ctx);
int						setup_file_descriptors(t_command *cmd,
							t_exec_context *ctx);
size_t					determine_args_size(t_command *cmd);
int						setup_execution_args(t_command *cmd,
							t_exec_context *ctx);
int						execute_command(t_exec_context *ctx, t_mini *mini);
void					close_saved_file_descriptors(t_exec_context *ctx);
void					init_redirection_context(t_redirection_context *ctx,
							int *in_fd);
void					set_redirection_context(t_redirection_context *ctx,
							int saved_stdin, int saved_stdout, char **err);
int						handle_output_redirection_pipe(t_command *current,
							t_redirection_context *ctx);
void					init_fork_error_context(t_fork_error_context *ctx,
							t_fork_error_init *init_data);
int						handle_fork_error(t_fork_error_context *ctx);
void					init_parent_process_context(
							t_parent_process_context *ctx,
							t_command *current, int pid,
							t_pipeline_context *pipeline_ctx);
int						handle_parent_process_pipe(
							t_parent_process_context *ctx);
int						handle_pipeline_setup(t_command *current,
							t_pipeline_setup_context *setup_ctx);
int						redir_and_execute_pipe(t_command *cmd, t_mini *mini);
void					init_pipeline_setup_context(
							t_pipeline_setup_context *setup_ctx,
							t_pipeline_context *ctx);
int						redirect_output_pipe(t_command *cmd, char **err);

#endif
