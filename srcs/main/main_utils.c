/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 10:56:29 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/17 10:57:24 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	cleanup_readline(void)
{
	clear_history();
}

static char	*append_line_with_prompt(char *command, const char *prompt)
{
	char	*new_line;
	char	*temp;

	new_line = readline(prompt);
	if (!new_line)
	{
		free(command);
		return (NULL);
	}
	temp = ft_strjoin(command, new_line);
	if (!temp)
	{
		free(command);
		free(new_line);
		exit(ERR_GENERAL);
	}
	free(command);
	free(new_line);
	return (temp);
}

char	*get_command(void)
{
	char	*command;

	command = readline("minishell> ");
	if (!command)
	{
		cleanup_readline();
		exit(ERR_SUCCESS);
	}
	if (!*command)
	{
		free(command);
		return (get_command());
	}
	while (!valid_quotes(command))
	{
		command = append_line_with_prompt(command, "quote> ");
		if (!command)
			return (NULL);
	}
	return (command);
}
