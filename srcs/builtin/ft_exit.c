/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 18:32:48 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/15 12:44:52 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_err_msg_export(char *str)
{
	write(2, "minishell: export: ", 19);
	ft_putstr_fd(str, 2);
	write(2, ": not a valid identifier\n", 25);
}

int	ft_isdigit_str(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	count_args(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

int	validate_arguments(int size)
{
	if (size > 1)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		exit(ERR_BUILTIN_USAGE);
	}
	return (0);
}

int	ft_exit(t_command *c)
{
	int	size;
	int	exit_code;

	size = count_args(c->args);
	if (validate_arguments(size))
		return (ERR_BUILTIN_USAGE);
	else if (size == 0)
		exit(ERR_SUCCESS);
	else
	{
		if (ft_isdigit_str(c->args[0]))
		{
			exit_code = ft_atoi(c->args[0]);
			exit(exit_code);
		}
		else
		{
			ft_putstr_fd("minishell: exit: numeric argument required\n",
				STDERR_FILENO);
			exit(ERR_EXIT_INVALID_NUM);
		}
	}
	return (ERR_SUCCESS);
}
