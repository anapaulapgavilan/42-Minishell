/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvaro <alvaro@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:22:58 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/14 01:03:16 by alvaro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	valid_quotes(char *command)
{
	int	single_quote;
	int	double_quote;
	int	i;

	single_quote = 0;
	double_quote = 0;
	i = 0;
	while (command[i])
	{
		if (command[i] == '\'' && double_quote % 2 == 0)
			single_quote++;
		else if (command[i] == '\"' && single_quote % 2 == 0)
			double_quote++;
		i++;
	}
	return (single_quote % 2 == 0 && double_quote % 2 == 0);
}

void	print_error(char *arg)
{
	ft_putstr_fd("minishell: cd: ", STDERR);
	ft_putstr_fd(arg, STDERR);
	ft_putstr_fd(": ", STDERR);
	ft_putendl_fd(strerror(errno), STDERR);
}
