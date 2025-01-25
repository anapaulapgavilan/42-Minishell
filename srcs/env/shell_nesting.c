/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_nesting.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvaro <alvaro@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:21:17 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/14 01:27:13 by alvaro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	invalid_lvl(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (ERR_INVALID_ARG);
		i++;
	}
	return (ERR_SUCCESS);
}

static int	get_lvl(const char *str)
{
	int	i;
	int	sign;
	int	num;

	i = 0;
	sign = 1;
	num = 0;
	ft_skip_spacenl(str, &i);
	if (invalid_lvl(str) != ERR_SUCCESS)
		return (0);
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
		num = num * 10 + (str[i++] - '0');
	return (num * sign);
}

int	shell_nesting_increment(t_env *env)
{
	int		shell_level;
	char	*shell_lvl;
	char	*level_value;

	level_value = get_env_value("SHLVL", env);
	if (ft_strcmp(level_value, "") == 0)
		return (free(level_value), ERR_ENV_VAR_NOT_FOUND);
	shell_level = get_lvl(level_value) + 1;
	free(level_value);
	while (env)
	{
		if (ft_strncmp(env->value, "SHLVL=", 6) == 0)
		{
			free(env->value);
			shell_lvl = ft_itoa(shell_level);
			if (!shell_lvl)
				return (ERR_GENERAL);
			env->value = ft_strjoin("SHLVL=", shell_lvl);
			if (!env->value)
				return (free(shell_lvl), ERR_GENERAL);
			return (free(shell_lvl), ERR_SUCCESS);
		}
		env = env->next;
	}
	return (ERR_ENV_VAR_NOT_FOUND);
}
