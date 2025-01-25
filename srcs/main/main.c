/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:21:58 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/17 10:57:13 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_sig			g_sig;

void	process_commands(t_command *commands, t_mini *mini)
{
	t_command	*current;

	expander(commands, mini->env);
	current = commands;
	while (current)
	{
		if (current->has_pipe)
		{
			if (execute_pipeline(commands, mini) != ERR_SUCCESS)
				break ;
			current = NULL;
		}
		else
		{
			if (redir_and_execute(current, mini) != ERR_SUCCESS)
				break ;
			current = current->next;
		}
	}
	free_command_list(commands);
}

t_command	*parse_and_expand(t_lexer *lex_list, char *line)
{
	t_command	*commands;

	commands = parser(lex_list);
	if (!commands)
	{
		free_lexer_list(lex_list);
		free(line);
		return (NULL);
	}
	return (commands);
}

static t_lexer	*read_and_lex_command(char **line, t_mini *mini)
{
	t_lexer	*lex_list;

	*line = get_command();
	g_sig.sigint = 0;
	if (!(*line))
	{
		handle_eof(mini);
		return (NULL);
	}
	add_history(*line);
	lex_list = lexer(*line);
	if (!lex_list)
	{
		free(*line);
		*line = NULL;
	}
	return (lex_list);
}

void	minishell_loop(t_mini *mini)
{
	char		*line;
	t_lexer		*lex_list;
	t_command	*commands;

	line = NULL;
	lex_list = NULL;
	while (1)
	{
		lex_list = read_and_lex_command(&line, mini);
		if (!lex_list)
			continue ;
		if (check_line(mini, lex_list))
		{
			commands = parse_and_expand(lex_list, line);
			if (!commands)
				continue ;
			process_commands(commands, mini);
		}
		free(line);
		free_lexer_list(lex_list);
		line = NULL;
		lex_list = NULL;
	}
}

int	main(int argc, char **argv, char **env)
{
	t_mini	mini;

	(void)argc;
	(void)argv;
	mini_init(&mini);
	if (env_init(&mini, env) != ERR_SUCCESS)
	{
		return (ERR_GENERAL);
	}
	shell_nesting_increment(mini.env);
	setup_signals();
	minishell_loop(&mini);
	free_env(mini.env);
	cleanup_readline();
	return (ERR_SUCCESS);
}
