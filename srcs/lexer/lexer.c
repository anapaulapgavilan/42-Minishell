/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:21:41 by ana-pper          #+#    #+#             */
/*   Updated: 2025/01/25 12:22:31 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	add_lexer_node(t_lexer **lst, char *str, t_tokens token)
{
	t_lexer	*new_node;
	t_lexer	*last;

	new_node = malloc(sizeof(t_lexer));
	if (!new_node)
		return (free(str), free_lexer_list(*lst), *lst = NULL, 0);
	new_node->str = str;
	new_node->token = token;
	new_node->next = NULL;
	if (!(*lst))
	{
		*lst = new_node;
		new_node->prev = NULL;
	}
	else
	{
		last = *lst;
		while (last->next)
			last = last->next;
		last->next = new_node;
		new_node->prev = last;
	}
	return (1);
}

t_tokens	is_token(char c)
{
	if (c == '|')
		return (PIPE);
	if (c == '>')
		return (GREAT);
	if (c == '<')
		return (LESS);
	return (NONE);
}

char	*return_reserved_token(char *str, int *i, t_tokens *token, int start)
{
	*token = is_token(str[*i]);
	if (str[*i + 1] == str[*i])
	{
		(*i)++;
		*token += 1;
	}
	return (ft_strndup(str + start, ++(*i) - start));
}

char	*get_token(char *str, int *i, t_tokens *token)
{
	int		start;
	char	in_q;

	start = *i;
	in_q = 0;
	*token = NONE;
	if (str[*i] == '<' || str[*i] == '>' || str[*i] == '|')
		return (return_reserved_token(str, i, token, start));
	while (str[*i] && ((!ft_isspace(str[*i]) && is_token(str[*i]) == NONE)
			|| in_q))
	{
		if (str[*i] == '"' || str[*i] == '\'')
			in_q += (in_q == 0) * str[*i] - (in_q == str[*i]) * in_q;
		else if (!in_q && str[*i] == '$' && str[*i + 1] == '?')
		{
			*i += 2;
			break ;
		}
		(*i)++;
	}
	return (ft_strndup(str + start, *i - start));
}

t_lexer	*lexer(char *input)
{
	int			i;
	t_lexer		*list;
	t_tokens	token;
	char		*token_str;

	i = 0;
	list = NULL;
	while (input[i])
	{
		while (ft_isspace(input[i]))
			i++;
		if (input[i])
		{
			token_str = get_token(input, &i, &token);
			if (!token_str)
				return (free_lexer_list(list), NULL);
			if (!add_lexer_node(&list, token_str, token))
				return (free_lexer_list(list), free(token_str), NULL);
		}
	}
	if (list && reorder_tokens(&list) == -1)
		return (free_lexer_list(list), NULL);
	return (list);
}
