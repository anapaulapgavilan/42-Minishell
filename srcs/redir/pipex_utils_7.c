/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_7.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 10:07:57 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/17 10:08:26 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_file_descriptors(t_command *cmd, t_exec_context *ctx)
{
	ctx->saved_in = dup(STDIN_FILENO);
	ctx->saved_out = dup(STDOUT_FILENO);
	if (ctx->saved_in == -1 || ctx->saved_out == -1)
	{
		return (g_sig.exit_status = ERR_GENERAL);
	}
	if (cmd->fd_in != -1)
	{
		if (handle_input_redirection(cmd->fd_in, ctx) != ERR_SUCCESS)
			return (g_sig.exit_status);
		cmd->fd_in = -1;
	}
	if (cmd->fd_out != -1)
	{
		if (handle_output_redirection(cmd->fd_out, ctx) != ERR_SUCCESS)
			return (g_sig.exit_status);
	}
	return (ERR_SUCCESS);
}

size_t	determine_args_size(t_command *cmd)
{
	if (cmd->args_size == 0 && cmd->input)
		return (1);
	return (cmd->args_size);
}

int	setup_execution_args(t_command *cmd, t_exec_context *ctx)
{
	size_t	args_size;

	args_size = cmd->args_size;
	if (args_size == 0 && cmd->input)
		args_size = 1;
	ctx->exec_args = allocate_exec_args(cmd);
	if (!ctx->exec_args)
	{
		cleanup_exec_resources(ctx->path, ctx->bin_paths, ctx->exec_args,
			ctx->env_array);
		restore_std(ctx->saved_in, ctx->saved_out);
		return (ERROR);
	}
	if (populate_exec_args(ctx->exec_args, cmd) != ERR_SUCCESS)
	{
		free(ctx->exec_args);
		restore_std(ctx->saved_in, ctx->saved_out);
		return (g_sig.exit_status = ERR_GENERAL);
	}
	return (ERR_SUCCESS);
}

int	execute_command(t_exec_context *ctx, t_mini *mini)
{
	char	*env_str;

	env_str = env_to_str(mini->env);
	ctx->env_array = ft_split(env_str, '\n');
	ft_memdel(env_str);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (execve(ctx->path, ctx->exec_args, ctx->env_array) == -1)
	{
		cleanup_exec_resources(ctx->path, ctx->bin_paths, ctx->exec_args,
			ctx->env_array);
		restore_std(ctx->saved_in, ctx->saved_out);
		return (g_sig.exit_status = ERR_EXEC_FAILED, g_sig.exit_status);
	}
	return (ERR_SUCCESS);
}

void	close_saved_file_descriptors(t_exec_context *ctx)
{
	if (ctx->saved_in != -1)
	{
		close(ctx->saved_in);
		ctx->saved_in = -1;
	}
	if (ctx->saved_out != -1)
	{
		close(ctx->saved_out);
		ctx->saved_out = -1;
	}
}
