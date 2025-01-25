/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 18:33:03 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/15 12:20:29 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, PATH_MAX))
	{
		ft_putendl_fd(cwd, STDOUT);
		return (ERR_SUCCESS);
	}
	else
	{
		ft_putendl_fd("pwd: error retrieving current directory", STDERR);
		return (ERR_DIRECTORY_NOT_FOUND);
	}
}
