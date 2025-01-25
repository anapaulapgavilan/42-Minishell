/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 18:32:41 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/15 12:08:15 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_env *env)
{
	if (!env)
	{
		ft_putstr_fd("minishell: env: environment is empty\n", STDERR_FILENO);
		return (ERR_ENV_VAR_NOT_FOUND);
	}
	while (env)
	{
		ft_putendl_fd(env->value, STDOUT_FILENO);
		env = env->next;
	}
	return (ERR_SUCCESS);
}
