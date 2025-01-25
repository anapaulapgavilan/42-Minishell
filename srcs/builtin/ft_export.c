/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 18:32:56 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/15 12:17:09 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_valid_export(char *s)
{
	int	i;

	if (!ft_isalpha(*s) && *s != '_')
		return (false);
	i = 0;
	while (ft_isalnum(s[i]) || s[i] == '_')
		i++;
	if (s[i] != 0 && s[i] != '=')
		return (false);
	return (true);
}

void	change_var(t_env *lst, char *str, bool *done)
{
	free(lst->value);
	lst->value = ft_strdup(str);
	*done = true;
}

void	add_new_var(t_env **env, char *str)
{
	t_env	*new;
	t_env	*tmp;

	new = malloc(sizeof(t_env));
	tmp = *env;
	if (!new)
		return ;
	new->value = ft_strdup(str);
	new->next = NULL;
	if (!*env)
		*env = new;
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	change_or_add_var(t_mini *mini, char *var, t_command *c, int i)
{
	bool	done;
	t_env	*tmp;

	done = false;
	tmp = mini->env;
	while (tmp)
	{
		if (ft_strncmp(tmp->value, var, ft_strlen(var)) == 0
			&& tmp->value[ft_strlen(var)] == '=')
		{
			change_var(tmp, c->args[i], &done);
			break ;
		}
		tmp = tmp->next;
	}
	if (!done)
		add_new_var(&mini->env, c->args[i]);
}

int	ft_export(t_command *c, t_mini *mini)
{
	char	var[BUFF_SIZE];
	int		i;
	int		ret;

	ret = ERR_SUCCESS;
	i = -1;
	while (c->args[++i])
	{
		if (!check_valid_export(c->args[i]))
		{
			print_err_msg_export(c->args[i]);
			ret |= ERR_INVALID_ARG;
		}
		else
		{
			get_env_name(var, c->args[i]);
			change_or_add_var(mini, var, c, i);
		}
	}
	return (ret);
}
