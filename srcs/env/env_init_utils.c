/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 18:17:21 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/15 18:39:23 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_key_with_equals(const char *value, char *key_with_equals)
{
	char	*key;
	int		key_len;

	key = ft_strdup(value);
	if (!key)
		return (ERR_GENERAL);
	key_len = 0;
	while (key[key_len] && key[key_len] != '=')
		key_len++;
	if (key_len + 1 >= BUFF_SIZE)
	{
		free(key);
		return (ERR_GENERAL);
	}
	ft_memcpy(key_with_equals, key, key_len);
	key_with_equals[key_len] = '=';
	key_with_equals[key_len + 1] = '\0';
	free(key);
	return (ERR_SUCCESS);
}

int	is_in_env(t_env *env, const char *key)
{
	char	key_with_equals[BUFF_SIZE];
	int		result;

	result = get_key_with_equals(key, key_with_equals);
	if (result != ERR_SUCCESS)
		return (result);
	while (env)
	{
		if (ft_strncmp(env->value, key_with_equals,
				ft_strlen(key_with_equals)) == 0)
			return (ERR_SUCCESS);
		env = env->next;
	}
	return (ERR_ENV_VAR_NOT_FOUND);
}

static int	update_env_node(t_env *env, const char *key_with_equals,
		const char *value)
{
	while (env)
	{
		if (ft_strncmp(env->value, key_with_equals,
				ft_strlen(key_with_equals)) == 0)
		{
			free(env->value);
			env->value = ft_strdup(value);
			if (!env->value)
				return (ERR_GENERAL);
			return (ERR_SUCCESS);
		}
		env = env->next;
	}
	return (ERR_ENV_VAR_NOT_FOUND);
}

int	env_update(t_env *env, const char *value)
{
	char	key_with_equals[BUFF_SIZE];
	int		result;

	result = get_key_with_equals(value, key_with_equals);
	if (result != ERR_SUCCESS)
		return (result);
	return (update_env_node(env, key_with_equals, value));
}
