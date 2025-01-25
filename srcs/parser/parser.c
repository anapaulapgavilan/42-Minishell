/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvaro <alvaro@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:22:13 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/14 00:37:18 by alvaro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_node_command(t_command **lst, t_command *new, t_lexer **tmp,
		bool last)
{
	t_command	*temp;

	new->has_pipe = !last;
	if (*lst == NULL)
		*lst = new;
	else
	{
		temp = *lst;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
	}
	new->next = NULL;
	if (!last && *tmp)
		*tmp = (*tmp)->next;
}

t_command	*handle_pipe_to_command(t_command **ret, t_command **new,
		t_lexer **tmp)
{
	if (!(*tmp)->next || (*tmp)->next->token == PIPE)
		return (cleanup_command_list(*ret), free_command_node(*new), NULL);
	add_node_command(ret, *new, tmp, false);
	*new = init_new((*tmp)->next);
	if (!(*new))
		return (cleanup_command_list(*ret), NULL);
	if (*tmp && (*tmp)->token == NONE)
	{
		(*new)->command = ft_strdup((*tmp)->str);
		if (!(*new)->command)
			return (cleanup_command_list(*ret), free_command_node(*new), NULL);
		*tmp = (*tmp)->next;
	}
	else
		return (cleanup_command_list(*ret), free_command_node(*new), NULL);
	return (NULL);
}

t_command	*handle_initial_command(t_command *new, t_lexer **tmp,
		t_command *ret)
{
	if (*tmp && (*tmp)->token == NONE)
	{
		new->command = ft_strdup((*tmp)->str);
		if (!new->command)
			return (cleanup_command_list(ret), free_command_node(new), NULL);
		*tmp = (*tmp)->next;
	}
	return (new);
}

int	process_tokens(t_command **ret, t_command **new, t_lexer **tmp)
{
	t_command	*result;

	while (*tmp)
	{
		if ((*tmp)->token == PIPE)
		{
			result = handle_pipe_to_command(ret, new, tmp);
			if (result)
				return (1);
			continue ;
		}
		if (add_token_to_command(*new, tmp) == -1)
			return (-1);
		*tmp = (*tmp)->next;
	}
	return (0);
}

t_command	*parser(t_lexer *list)
{
	t_command	*ret;
	t_command	*new;
	t_lexer		*tmp;

	ret = NULL;
	tmp = list;
	new = init_new(list);
	if (!new)
		return (NULL);
	new = handle_initial_command(new, &tmp, ret);
	if (!new)
		return (NULL);
	if (process_tokens(&ret, &new, &tmp) != 0)
		return (cleanup_command_list(ret), free_command_node(new), NULL);
	if (new && new->command)
		add_node_command(&ret, new, &tmp, true);
	else
		free_command_node(new);
	return (ret);
}
