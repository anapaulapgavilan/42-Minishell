/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 18:33:13 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/15 12:38:30 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_err_msg_unset(char *str)
{
	write(2, "minishell: unset: ", 18);
	ft_putstr_fd(str, 2);
	write(2, ": not a valid identifier\n", 25);
}

bool	check_valid_unset(char *s)
{
	int	i;

	if (!ft_isalpha(*s) && *s != '_')
		return (false);
	i = 0;
	while (ft_isalnum(s[i]) || s[i] == '_')
		i++;
	return (s[i] == 0);
}

void	delete_envar(t_env **env, char *str)
{
	t_env	*tmp;
	t_env	*prev;
	char	var[BUFF_SIZE];

	prev = NULL;
	if (!*env)
		return ;
	get_env_name(var, str);
	tmp = *env;
	while (tmp)
	{
		if (ft_strncmp(tmp->value, var, ft_strlen(var)) == 0
			&& tmp->value[ft_strlen(var)] == '=')
		{
			if (prev)
				prev->next = tmp->next;
			else
				*env = tmp->next;
			free(tmp->value);
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

int	ft_unset(t_command *c, t_mini *mini)
{
	int	i;
	int	exit_code;

	i = 0;
	exit_code = ERR_SUCCESS;
	while (c->args[i])
	{
		if (!check_valid_unset(c->args[i]))
		{
			print_err_msg_unset(c->args[i]);
			exit_code = ERR_BUILTIN_USAGE;
		}
		else
		{
			delete_envar(&(mini->env), c->args[i]);
		}
		i++;
	}
	return (exit_code);
}
