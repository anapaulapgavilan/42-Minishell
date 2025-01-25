/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix_orders.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvaro <alvaro@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 14:25:30 by asaiz-lo          #+#    #+#             */
/*   Updated: 2024/12/13 23:35:43 by alvaro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	reorder_tokens(t_lexer **list)
{
	t_lexer	*comand_finder;
	t_lexer	*prev;

	comand_finder = *list;
	prev = *list;
	if (is_token(comand_finder->str[0]) == NONE)
		return (1);
	while (is_token(comand_finder->str[0]) != NONE)
	{
		if (comand_finder->next && comand_finder->next->next)
		{
			prev = comand_finder->next;
			comand_finder = comand_finder->next->next;
		}
		else
			return (-1);
	}
	prev->next = comand_finder->next;
	comand_finder->next = *list;
	*list = comand_finder;
	return (0);
}
