/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvaro <alvaro@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 23:33:54 by alvaro            #+#    #+#             */
/*   Updated: 2024/12/13 23:34:25 by alvaro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_list_inp(t_input *l)
{
	int	i;

	i = 1;
	while (l)
	{
		printf("%i. %s  %d\n", i, l->file, l->condition);
		l = l->next;
		i++;
	}
}

void	expand_input(t_input *list, t_env *env, bool out_mode)
{
	while (list)
	{
		if (!list->condition || out_mode)
			list->file = expand(list->file, env);
		list = list->next;
	}
}

int	expander(t_command *list, t_env *env)
{
	size_t	i;

	while (list)
	{
		list->command = expand(list->command, env);
		expand_input(list->input, env, 0);
		expand_input(list->output, env, 1);
		i = -1;
		while ((++i) < list->args_size && list->args[i])
		{
			list->args[i] = expand(list->args[i], env);
		}
		list = list->next;
	}
	return (ERR_SUCCESS);
}
