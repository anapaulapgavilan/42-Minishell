/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 18:32:34 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/15 12:06:53 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_echo(char **args)
{
	int		i;
	int		j;
	bool	n_option;

	i = 0;
	n_option = false;
	while (args[i] && ft_strcmp(args[i], "-n") == 0)
	{
		n_option = true;
		i++;
	}
	while (args[i])
	{
		j = -1;
		while (args[i][++j])
			write(1, &args[i][j], 1);
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!n_option)
		write(1, "\n", 1);
	return (0);
}
