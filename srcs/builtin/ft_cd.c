/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 18:32:27 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/15 16:23:32 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	attempt_directory_change(const char *resolved_path)
{
	if (chdir(resolved_path) < 0)
	{
		perror("cd: failed to change directory");
		return (ERR_DIRECTORY_NOT_FOUND);
	}
	return (ERR_SUCCESS);
}

int	update_directory_variables(t_env *env, const char *cwd,
		const char *resolved_path)
{
	if (update_env_var(env, "OLDPWD=", cwd) != ERR_SUCCESS
		|| update_env_var(env, "PWD=", resolved_path) != ERR_SUCCESS)
	{
		return (ERR_GENERAL);
	}
	return (ERR_SUCCESS);
}

int	change_directory_and_update_env(char *resolved_path, char *cwd, t_env *env)
{
	int	result;

	result = attempt_directory_change(resolved_path);
	if (result != ERR_SUCCESS)
		return (result);
	result = update_directory_variables(env, cwd, resolved_path);
	return (result);
}

int	ft_cd(char **args, t_env *env)
{
	char	cwd[PATH_MAX];
	char	*target_path;
	char	*resolved_path;
	int		result;

	target_path = NULL;
	resolved_path = NULL;
	result = get_current_directory(cwd);
	if (result != ERR_SUCCESS)
		return (result);
	target_path = resolve_target_path(args, env);
	if (!target_path)
		return (ERR_DIRECTORY_NOT_FOUND);
	resolved_path = resolve_and_validate_path(target_path, cwd);
	free(target_path);
	if (!resolved_path)
		return (ERR_GENERAL);
	result = change_directory_and_update_env(resolved_path, cwd, env);
	free(resolved_path);
	return (result);
}
