/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 16:21:56 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/15 16:25:28 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*env_value_cd(const char *key, t_env *env)
{
	size_t	key_len;

	key_len = ft_strlen(key);
	while (env)
	{
		if (ft_strncmp(env->value, key, key_len) == 0
			&& env->value[key_len] == '=')
		{
			return (ft_strdup(env->value + key_len + 1));
		}
		env = env->next;
	}
	return (NULL);
}

char	*get_env_value_cd(const char *key, t_env *env)
{
	char	*value;

	value = env_value_cd(key, env);
	if (value && *value == '\0')
	{
		free(value);
		return (NULL);
	}
	return (value);
}

int	get_current_directory(char *cwd)
{
	if (!getcwd(cwd, PATH_MAX))
	{
		ft_putstr_fd("cd: failed to get current working directory\n",
			STDERR_FILENO);
		return (ERR_GENERAL);
	}
	return (ERR_SUCCESS);
}

char	*resolve_target_path(char **args, t_env *env)
{
	char	*target_path;

	target_path = NULL;
	if (!args[0] || ft_strcmp(args[0], "~") == 0)
	{
		target_path = get_env_value_cd("HOME", env);
		if (!target_path)
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
	}
	else if (ft_strcmp(args[0], "-") == 0)
	{
		target_path = get_env_value_cd("OLDPWD", env);
		if (!target_path)
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
		else
			ft_putendl_fd(target_path, STDOUT_FILENO);
	}
	else
		target_path = ft_strdup(args[0]);
	if (!target_path)
		ft_putstr_fd("cd: memory allocation failed\n", STDERR_FILENO);
	return (target_path);
}

char	*resolve_and_validate_path(const char *target_path, const char *cwd)
{
	char	*resolved_path;

	resolved_path = resolve_path(target_path, cwd);
	if (!resolved_path)
		ft_putstr_fd("cd: failed to resolve path\n", STDERR_FILENO);
	return (resolved_path);
}
