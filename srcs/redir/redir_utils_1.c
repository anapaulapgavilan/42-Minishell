/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 10:54:33 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/16 18:00:11 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_dir(char *bin, char *command)
{
	DIR				*folder;
	struct dirent	*item;
	char			*path;

	path = NULL;
	folder = opendir(bin);
	if (!folder)
		return (NULL);
	item = readdir(folder);
	while (item)
	{
		if (ft_strcmp(item->d_name, command) == 0)
		{
			path = path_join(bin, item->d_name);
			break ;
		}
	}
	closedir(folder);
	return (path);
}

void	free_tab(char **tab)
{
	size_t	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		tab[i] = NULL;
		i++;
	}
	free(tab);
	tab = NULL;
}

char	*env_to_str(t_env *env)
{
	char	*result;
	char	*temp;

	result = ft_strdup("");
	while (env)
	{
		temp = ft_strjoin(result, env->value);
		free(result);
		result = ft_strjoin(temp, "\n");
		free(temp);
		env = env->next;
	}
	return (result);
}

char	*path_join(const char *bin, const char *command)
{
	char	*tmp;
	char	*full_path;

	tmp = ft_strjoin(bin, "/");
	if (!tmp)
		return (NULL);
	full_path = ft_strjoin(tmp, command);
	free(tmp);
	return (full_path);
}

void	cleanup_exec_resources(char *path, char **bin_paths, char **exec_args,
		char **env_array)
{
	if (path)
		free(path);
	if (bin_paths)
		free_tab(bin_paths);
	if (exec_args)
		free_tab(exec_args);
	if (env_array)
		free_tab(env_array);
}
