/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_6.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 10:06:51 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/17 10:07:17 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_pipeline_commands(t_command *cmd_list, t_mini *mini,
		t_pipeline_context *ctx)
{
	t_command	*current;
	int			result;

	current = cmd_list;
	while (current)
	{
		result = process_single_command(current, mini, ctx);
		if (result != ERR_SUCCESS)
			return (result);
		current = current->next;
	}
	return (ERR_SUCCESS);
}

int	finalize_pipeline_context(t_pipeline_context *ctx)
{
	int	i;

	if (dup2(ctx->saved_stdin, STDIN_FILENO) == -1)
		close(ctx->saved_stdin);
	if (dup2(ctx->saved_stdout, STDOUT_FILENO) == -1)
		close(ctx->saved_stdout);
	i = 0;
	while (i < ctx->child_count)
	{
		waitpid(ctx->child_pids[i], &ctx->status, 0);
		i++;
	}
	if (WIFEXITED(ctx->status))
		return (g_sig.exit_status = WEXITSTATUS(ctx->status),
			g_sig.exit_status);
	return (-1);
}

int	execute_pipeline(t_command *cmd_list, t_mini *mini)
{
	t_pipeline_context	ctx;

	if (initialize_pipeline_context(&ctx) != ERR_SUCCESS)
		return (ERR_PIPE_CREATION);
	if (process_pipeline_commands(cmd_list, mini, &ctx) != ERR_SUCCESS)
	{
		finalize_pipeline_context(&ctx);
		return (g_sig.exit_status);
	}
	return (finalize_pipeline_context(&ctx));
}

int	handle_input_redirection(int fd_in, t_exec_context *ctx)
{
	if (dup2(fd_in, STDIN_FILENO) == -1)
	{
		restore_std(ctx->saved_in, ctx->saved_out);
		return (g_sig.exit_status = ERR_INPUT_PERMISSION_DENIED);
	}
	close(fd_in);
	return (ERR_SUCCESS);
}

int	handle_output_redirection(int fd_out, t_exec_context *ctx)
{
	if (dup2(fd_out, STDOUT_FILENO) == -1)
	{
		restore_std(ctx->saved_in, ctx->saved_out);
		return (g_sig.exit_status = ERR_OUTPUT_PERMISSION_DENIED);
	}
	close(fd_out);
	return (ERR_SUCCESS);
}
