NAME = minishell

CC = gcc

CFLAGS = -Wall -Wextra -Werror -g3 -I includes/ -I libft/includes/ 

LIBFT = -L libft -lft -lreadline

HEADER = minishell.h

MAIN = main signals main_utils

UTILS = fd_management format free check_line free_utils

LEXER = lexer fix_orders

ENV = env_init shell_nesting get_env env_init_utils

PARSER = parser expander expand parser_utils

REDIR = redir pipex redir_utils_1 redir_utils_2 redir_utils_3 redir_utils_4 redir_utils_5 pipex_utils_1 pipex_utils_2 pipex_utils_3 pipex_utils_4 pipex_utils_5 pipex_utils_6 pipex_utils_7 pipex_utils_8   

REDIRECT = redirects redirects_utils

BUILTIN = is_builtin ft_echo ft_env ft_cd ft_pwd ft_exit ft_export ft_unset ft_cd_utils ft_cd_utils_1

SRC = $(addsuffix .c, $(addprefix srcs/main/, $(MAIN))) \
		$(addsuffix .c, $(addprefix srcs/utils/, $(UTILS))) \
		$(addsuffix .c, $(addprefix srcs/env/, $(ENV))) \
		$(addsuffix .c, $(addprefix srcs/lexer/, $(LEXER))) \
		$(addsuffix .c, $(addprefix srcs/parser/, $(PARSER))) \
		$(addsuffix .c, $(addprefix srcs/redir/, $(REDIR))) \
		$(addsuffix .c, $(addprefix srcs/builtin/, $(BUILTIN))) \
		$(addsuffix .c, $(addprefix srcs/redirections/, $(REDIRECT))) \

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@echo "\n"
	@make -C libft/
	@echo "\033[0;32mCOMPILING MINISHELL..."
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(PIPEX)
	@echo "\n\033[0;32mDONE...\n\033[0m"

%.o: %.c
	@printf "\033[0;33mGENERATING MINISHELL OBJECTS... %-33.33s\r" $@
	@${CC} ${CFLAGS} -c $< -o $@

clean:
	@echo "\033[0;31mCLEANING LIBFT..."
	@make clean -C libft/
	@echo "\nREMOVING MINISHELL BINARIES..."
	@rm -f $(OBJ)
	@echo "\033[0m"

fclean:
	@echo "\033[0;31mCLEANING LIBFT..."
	@make fclean -C libft/
	@echo "\nDELETING OBJECTS..."
	@rm -f $(OBJ)
	@echo "\nDELETING EXECUTABLE..."
	@rm -f $(NAME)
	@echo "\033[0m"

re: fclean all

test: all
	./minishell

norm:
	norminette $(SRC) includes/$(HEADER)

.PHONY: clean fclean re test norm
