/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvaro <alvaro@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:20:56 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/14 01:22:14 by alvaro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_value_len(const char *env)
{
	int	i;
	int	size_name;

	size_name = 0;
	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	i += 1;
	while (env[i])
	{
		i++;
		size_name++;
	}
	return (size_name);
}

char	*env_value(char *env)
{
	int		i;
	int		j;
	int		size_alloc;
	char	*env_value;

	i = 0;
	j = 0;
	size_alloc = env_value_len(env) + 1;
	env_value = malloc(size_alloc);
	if (!env_value)
		return (NULL);
	while (env[i] && env[i] != '=')
		i++;
	i++;
	while (env[i])
		env_value[j++] = env[i++];
	env_value[j] = '\0';
	return (env_value);
}

char	*get_env_name(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i] && src[i] != '=' && ft_strlen(src) < BUFF_SIZE)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*get_env_value(char *arg, t_env *env)
{
	char	env_name[BUFF_SIZE];
	char	*env_val;

	env_val = NULL;
	if (ft_strcmp(arg, "?") == 0)
		return (ft_itoa(g_sig.exit_status));
	while (env && env->value)
	{
		get_env_name(env_name, env->value);
		if (ft_strcmp(arg, env_name) == 0)
		{
			if (env_val)
				ft_memdel(env_val);
			env_val = env_value(env->value);
			if (!env_val)
				return (NULL);
			return (env_val);
		}
		env = env->next;
	}
	return (ft_strdup(""));
}
