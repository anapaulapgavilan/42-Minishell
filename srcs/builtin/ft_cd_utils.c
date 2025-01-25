/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 16:18:02 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/15 16:23:59 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_and_attach_new_node(t_env *current, const char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (ERR_GENERAL);
	new_node->value = ft_strdup(value);
	if (!new_node->value)
	{
		free(new_node);
		return (ERR_GENERAL);
	}
	new_node->next = NULL;
	current->next = new_node;
	return (ERR_SUCCESS);
}

t_env	*find_or_update_node(t_env *env, const char *value, int *result)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strncmp(current->value, value, ft_strlen(value)) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			if (current->value)
				*result = ERR_SUCCESS;
			else
				*result = ERR_GENERAL;
			return (NULL);
		}
		if (!current->next)
			break ;
		current = current->next;
	}
	*result = ERR_SUCCESS;
	return (current);
}

int	env_add(const char *value, t_env *env)
{
	t_env	*current;
	int		result;

	if (!value || !env)
		return (ERR_INVALID_ARG);
	current = find_or_update_node(env, value, &result);
	if (result != ERR_SUCCESS || current == NULL)
		return (result);
	result = create_and_attach_new_node(current, value);
	return (result);
}

char	*resolve_path(const char *path, const char *cwd)
{
	char	*resolved_path;
	char	*full_path;

	if (!path || !cwd)
		return (NULL);
	if (path[0] == '/')
		return (ft_strdup(path));
	resolved_path = ft_strjoin(cwd, "/");
	if (!resolved_path)
		return (NULL);
	full_path = ft_strjoin(resolved_path, path);
	free(resolved_path);
	return (full_path);
}

int	update_env_var(t_env *env, const char *key, const char *value)
{
	char	*env_var;
	int		result;

	env_var = ft_strjoin(key, value);
	if (!env_var)
		return (ERR_GENERAL);
	if (is_in_env(env, key))
	{
		result = env_update(env, env_var);
	}
	else
	{
		result = env_add(env_var, env);
	}
	free(env_var);
	return (result);
}
