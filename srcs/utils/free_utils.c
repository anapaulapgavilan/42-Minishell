/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 10:17:28 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/17 10:30:52 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_input_list(t_input **in)
{
	t_input	*temp;
	t_input	*input;

	input = *in;
	while (input)
	{
		temp = input->next;
		if (input->file)
			free(input->file);
		free(input);
		input = temp;
	}
	*in = NULL;
}

void	cleanup_redirections(t_input *list)
{
	t_input	*tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		if (tmp->file)
		{
			free(tmp->file);
			tmp->file = NULL;
		}
		free(tmp);
	}
}

void	cleanup_command_list(t_command *command_list)
{
	t_command	*temp;

	while (command_list)
	{
		temp = command_list->next;
		free_command_node(command_list);
		command_list = temp;
	}
}

void	free_lexer_list(t_lexer *lexer_list)
{
	t_lexer	*tmp;

	while (lexer_list)
	{
		tmp = lexer_list;
		lexer_list = lexer_list->next;
		if (tmp->str)
			free(tmp->str);
		free(tmp);
	}
}
