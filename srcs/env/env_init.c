/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:19:52 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/15 18:39:26 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_env_node(t_env **node, const char *value)
{
	*node = malloc(sizeof(t_env));
	if (!*node)
		return (ERR_GENERAL);
	(*node)->value = ft_strdup(value);
	if (!(*node)->value)
	{
		free(*node);
		return (ERR_GENERAL);
	}
	(*node)->next = NULL;
	return (ERR_SUCCESS);
}

int	append_env_node(t_env **env, const char *value)
{
	t_env	*new_node;
	int		result;

	result = create_env_node(&new_node, value);
	if (result != ERR_SUCCESS)
		return (result);
	(*env)->next = new_node;
	*env = new_node;
	return (ERR_SUCCESS);
}

int	initialize_first_node(t_mini *mini, char **env_array)
{
	t_env	*first_node;
	int		result;

	result = create_env_node(&first_node, env_array[0]);
	if (result != ERR_SUCCESS)
		return (result);
	mini->env = first_node;
	return (ERR_SUCCESS);
}

int	env_init(t_mini *mini, char **env_array)
{
	t_env	*current;
	int		result;
	int		i;

	if (!env_array || !env_array[0])
		return (ERR_GENERAL);
	result = initialize_first_node(mini, env_array);
	if (result != ERR_SUCCESS)
		return (result);
	current = mini->env;
	i = 0;
	while (env_array[++i])
	{
		result = append_env_node(&current, env_array[i]);
		if (result != ERR_SUCCESS)
		{
			free_env(mini->env);
			return (ERR_GENERAL);
		}
	}
	return (ERR_SUCCESS);
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->value);
		free(tmp);
	}
}
