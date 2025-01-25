/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvaro <alvaro@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 01:00:57 by alvaro            #+#    #+#             */
/*   Updated: 2024/12/14 01:00:58 by alvaro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_type(t_lexer *token, t_tokens type)
{
	return (token && token->token == type);
}

int	is_types(t_lexer *token, char *types)
{
	if (ft_strchr(types, 'G') && is_type(token, GREAT))
		return (1);
	if (ft_strchr(types, 'A') && is_type(token, GREAT_GREAT))
		return (1);
	if (ft_strchr(types, 'L') && is_type(token, LESS))
		return (1);
	if (ft_strchr(types, 'I') && is_type(token, LESS_LESS))
		return (1);
	if (ft_strchr(types, 'P') && is_type(token, PIPE))
		return (1);
	if (ft_strchr(types, 'E') && is_type(token, NONE))
		return (1);
	return (0);
}

void	syntax_error(char *str)
{
	ft_putstr_fd("minishell: syntax error near unexpected token \'", STDERR);
	ft_putstr_fd(str, STDERR);
	ft_putendl_fd("'", STDERR);
}

int	check_line(t_mini *mini, t_lexer *token)
{
	while (token)
	{
		if (is_types(token, "GLAI") && (!token->next || is_types(token->next,
					"GALIP")))
		{
			if (token->next)
				syntax_error(token->next->str);
			else
				syntax_error("newline");
			mini->return_code = 258;
			return (0);
		}
		if (is_types(token, "P"))
		{
			if (!token->prev || !token->next || is_types(token->prev, "P")
				|| is_types(token->next, "P"))
				return (syntax_error(token->str), mini->return_code = 258, 0);
		}
		if (is_types(token, "GALI") && token->next && is_types(token->next,
				"GALI"))
			return (syntax_error(token->next->str), mini->return_code = 258, 0);
		token = token->next;
	}
	return (1);
}
