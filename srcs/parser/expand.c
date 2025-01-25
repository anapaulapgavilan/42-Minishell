/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 20:05:14 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/17 10:40:55 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_delim(char c)
{
	return (c == '{' || c == '}' || c == 0 || c == '$' || c == '\'' || c == '"'
		|| c == '/');
}

char	*construct_new_str(char *str, int i, int len, char *s2)
{
	char	*s1;
	char	*new_str;
	char	*rest;
	char	*temp;

	s1 = ft_strndup(str, i);
	if (s2)
		new_str = ft_strjoin(s1, s2);
	else
		new_str = ft_strdup(s1);
	if (new_str)
	{
		rest = str + i + len;
		temp = new_str;
		new_str = ft_strjoin(new_str, rest);
		free(temp);
	}
	free(s1);
	return (new_str);
}

void	replace(char **str, int *i, int len, t_env *env)
{
	char	*var;
	char	*s2;
	char	*new_str;

	if (len <= 1)
		return ;
	var = ft_strndup((*str) + (*i), len);
	if (var)
		s2 = get_env_value(var + 1, env);
	else
		s2 = NULL;
	new_str = construct_new_str(*str, *i, len, s2);
	free(var);
	free(*str);
	*str = new_str;
	if (s2)
		*i += ft_strlen(s2) - 1;
	free(s2);
}

char	*delete_quotes(char *str, int len)
{
	char	*new;
	int		i;
	int		j;
	char	in_q;

	i = 0;
	j = 0;
	in_q = 0;
	new = (char *)malloc(sizeof(char) * (len + 1));
	if (!new || !str)
		exit(ERR_GENERAL);
	while (str[j])
	{
		if (!in_q && (str[j] == '"' || str[j] == '\''))
			in_q = str[j++];
		else if (in_q && str[j] == in_q)
		{
			in_q = 0;
			j++;
		}
		else
			new[i++] = str[j++];
	}
	return (new[i] = '\0', new);
}

char	*expand(char *str, t_env *env)
{
	char	*ret;
	int		i;
	char	in_q;
	int		len;

	i = 0;
	in_q = 0;
	while (str && str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			in_q += (in_q == 0) * str[i] - (str[i] == in_q) * str[i];
		if (str[i] == '$' && in_q != '\'' && (i == 0 || str[i - 1] != '\\'))
		{
			len = 1;
			while (!ft_isspace(str[len + i]) && !is_delim(str[len + i]))
				len++;
			replace(&str, &i, len, env);
		}
		i++;
	}
	ret = delete_quotes(str, i);
	free(str);
	if (!ret)
		exit(ERR_GENERAL);
	return (ret);
}
