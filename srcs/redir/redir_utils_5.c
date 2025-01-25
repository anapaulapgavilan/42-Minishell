/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils_5.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 19:14:21 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/16 19:19:30 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_command_resources(t_exec_context *ctx, t_command *cmd)
{
	free(ctx->exec_args);
	restore_std(ctx->saved_in, ctx->saved_out);
	if (cmd->fd_in != -1)
		close(cmd->fd_in);
	if (cmd->fd_out != -1)
		close(cmd->fd_out);
}

void	cleanup_path_resources(t_exec_context *ctx)
{
	if (ctx->path)
	{
		free(ctx->path);
		ctx->path = NULL;
	}
	if (ctx->bin_paths)
	{
		free_tab(ctx->bin_paths);
		ctx->bin_paths = NULL;
	}
}

int	check_all_bin_paths(char **bin_paths, const char *command,
		char **result_path)
{
	int	i;
	int	result;

	i = 0;
	while (bin_paths[i])
	{
		result = check_path_command(bin_paths[i], command, result_path);
		if (result == ERR_SUCCESS)
			return (result);
		i++;
	}
	return (ERR_CMD_NOT_FOUND);
}

int	resolve_command_path(t_command *cmd, t_mini *mini,
		t_exec_context *ctx)
{
	t_env	*path_env;
	int		result;

	path_env = find_path_env(mini->env);
	if (!path_env || !(path_env->value + 5))
		return (handle_path_not_set(ctx));
	ctx->bin_paths = ft_split(path_env->value + 5, ':');
	if (ft_strchr(cmd->command, '/'))
	{
		result = check_direct_command(cmd, ctx);
		if (result == ERR_SUCCESS)
			return (result);
	}
	else if (ctx->bin_paths)
	{
		result = check_all_bin_paths(ctx->bin_paths, cmd->command, &ctx->path);
		if (result == ERR_SUCCESS)
			return (result);
	}
	return (handle_command_not_found(cmd, ctx));
}
