/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvaro <alvaro@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 23:56:57 by alvaro            #+#    #+#             */
/*   Updated: 2024/12/14 00:39:09 by alvaro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*innit_new(void)
{
	t_command	*new;

	new = (t_command *)malloc(sizeof(t_command));
	if (!new)
		return (NULL);
	new->command = NULL;
	new->args = NULL;
	new->args_size = 0;
	new->input = NULL;
	new->output = NULL;
	new->fd_in = -1;
	new->fd_out = -1;
	new->has_pipe = false;
	new->append = false;
	new->next = NULL;
	return (new);
}

int	handle_type(t_command *new, t_lexer **tmp)
{
	if ((*tmp)->token == LESS || (*tmp)->token == LESS_LESS)
	{
		if (!(*tmp)->next)
			return (-1);
		if (!add_input_node(&new->input, (*tmp)->next->str,
				(*tmp)->token == LESS_LESS))
			return (-1);
		*tmp = (*tmp)->next;
	}
	else if ((*tmp)->token == GREAT || (*tmp)->token == GREAT_GREAT)
	{
		if (!(*tmp)->next)
			return (-1);
		if (!add_input_node(&new->output, (*tmp)->next->str,
				(*tmp)->token == GREAT_GREAT))
			return (-1);
		new->append = ((*tmp)->token == GREAT_GREAT);
		*tmp = (*tmp)->next;
	}
	return (0);
}

int	add_token_to_command(t_command *new, t_lexer **tmp)
{
	int	i;
	int	j;

	i = 0;
	if (!tmp || !*tmp)
		return (-1);
	if ((*tmp)->token == LESS || (*tmp)->token == LESS_LESS
		|| (*tmp)->token == GREAT || (*tmp)->token == GREAT_GREAT)
		return (handle_type(new, tmp));
	while (new->args[i])
		i++;
	if (new->args[i])
		free(new->args[i]);
	new->args[i] = ft_strdup((*tmp)->str);
	if (!new->args[i])
	{
		j = 0;
		while (j < i)
		{
			free(new->args[j]);
			new->args[j++] = NULL;
		}
		return (free(new->args), new->args = NULL, -1);
	}
	return (0);
}

bool	add_input_node(t_input **lst, char *str, bool condition)
{
	t_input	*new;
	t_input	*temp;

	new = (t_input *)malloc(sizeof(t_input));
	if (!new)
		return (false);
	new->file = ft_strdup(str);
	if (!new->file)
	{
		free(new);
		return (false);
	}
	new->condition = condition;
	new->next = NULL;
	if (*lst == NULL)
		*lst = new;
	else
	{
		temp = *lst;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
	}
	return (true);
}

t_command	*init_new(t_lexer *tmp)
{
	size_t		size;
	t_lexer		*curr;
	t_command	*new;
	size_t		i;

	size = 0;
	curr = tmp;
	new = innit_new();
	while (curr && curr->token != PIPE)
	{
		if (curr->token == NONE)
			size++;
		curr = curr->next;
	}
	new->args = (char **)malloc(sizeof(char *) * (size + 1));
	if (!new->args)
		return (free(new), NULL);
	i = 0;
	while (i <= size)
		new->args[i++] = NULL;
	return (new->args_size = size, new);
}
