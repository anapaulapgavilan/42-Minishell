/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 19:07:21 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/16 19:08:04 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_redirection(t_command *cmd, int saved_in, int saved_out,
		char **err)
{
	int	redir_status;

	redir_status = redirect(cmd, err);
	if (redir_status != ERR_SUCCESS)
	{
		return (handle_redirection_error(redir_status, *err, saved_in,
				saved_out));
	}
	if (cmd->fd_in != -1 && dup2(cmd->fd_in, STDIN_FILENO) == -1)
	{
		restore_std(saved_in, saved_out);
		return (g_sig.exit_status = ERR_INPUT_PERMISSION_DENIED);
	}
	if (cmd->fd_out != -1 && dup2(cmd->fd_out, STDOUT_FILENO) == -1)
	{
		restore_std(saved_in, saved_out);
		return (g_sig.exit_status = ERR_OUTPUT_PERMISSION_DENIED);
	}
	return (ERR_SUCCESS);
}

char	**allocate_exec_args(t_command *cmd)
{
	char	**exec_args;

	exec_args = (char **)malloc(sizeof(char *) * (cmd->args_size + 2));
	if (!exec_args)
		return (NULL);
	return (exec_args);
}

int	populate_exec_args(char **exec_args, t_command *cmd)
{
	size_t	i;

	exec_args[0] = cmd->command;
	i = 0;
	while (i < cmd->args_size)
	{
		exec_args[i + 1] = cmd->args[i];
		i++;
	}
	exec_args[cmd->args_size + 1] = NULL;
	return (ERR_SUCCESS);
}

int	initialize_exec_context(t_exec_context *ctx)
{
	ctx->saved_in = -1;
	ctx->saved_out = -1;
	ctx->path = NULL;
	ctx->bin_paths = NULL;
	ctx->env_array = NULL;
	ctx->exec_args = NULL;
	return (ERR_SUCCESS);
}

int	prepare_command_execution(t_command *cmd, t_exec_context *ctx,
		char **err)
{
	int	result;

	initialize_exec_context(ctx);
	result = save_std_fds(&ctx->saved_in, &ctx->saved_out);
	if (result != ERR_SUCCESS)
		return (g_sig.exit_status = result);
	result = setup_redirection(cmd, ctx->saved_in, ctx->saved_out, err);
	if (result != ERR_SUCCESS)
		return (g_sig.exit_status = result);
	ctx->exec_args = allocate_exec_args(cmd);
	if (!ctx->exec_args)
	{
		cleanup_exec_resources(ctx->path, ctx->bin_paths, ctx->exec_args,
			ctx->env_array);
		restore_std(ctx->saved_in, ctx->saved_out);
		return (g_sig.exit_status = ERR_GENERAL);
	}
	result = populate_exec_args(ctx->exec_args, cmd);
	if (result != ERR_SUCCESS)
	{
		free(ctx->exec_args);
		restore_std(ctx->saved_in, ctx->saved_out);
		return (g_sig.exit_status = result);
	}
	return (ERR_SUCCESS);
}
