/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_5.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 09:57:21 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/17 10:03:47 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_pipeline_setup_context(t_pipeline_setup_context *setup_ctx,
		t_pipeline_context *ctx)
{
	setup_ctx->in_fd = &ctx->in_fd;
	setup_ctx->saved_stdin = ctx->saved_stdin;
	setup_ctx->saved_stdout = ctx->saved_stdout;
	setup_ctx->pipe_fd = ctx->pipe_fd;
}

void	init_fork_error_data(t_fork_error_init *init_data, t_command *current,
		t_pipeline_context *ctx)
{
	init_data->current = current;
	init_data->pipe_fd = ctx->pipe_fd;
	init_data->in_fd = ctx->in_fd;
	init_data->saved_stdin = ctx->saved_stdin;
	init_data->saved_stdout = ctx->saved_stdout;
}

static void	setup_fork_error_context(t_fork_error_context *fork_ctx,
		t_fork_error_init *init_data, t_command *current,
		t_pipeline_context *ctx)
{
	init_fork_error_data(init_data, current, ctx);
	init_fork_error_context(fork_ctx, init_data);
}

static void	execute_child_process(t_command *current, t_mini *mini,
		t_pipeline_context *ctx)
{
	if (current->has_pipe)
		execute_child_process_pipe(current, mini, ctx->in_fd, ctx->pipe_fd);
	else
		execute_child_process_pipe(current, mini, ctx->in_fd, NULL);
}

int	process_single_command(t_command *current, t_mini *mini,
		t_pipeline_context *ctx)
{
	int							result;
	pid_t						pid;
	t_fork_error_context		fork_ctx;
	t_fork_error_init			init_data;
	t_pipeline_setup_context	setup_ctx;

	init_pipeline_setup_context(&setup_ctx, ctx);
	if (handle_pipeline_setup(current, &setup_ctx) != ERR_SUCCESS)
		return (g_sig.exit_status);
	pid = fork();
	if (pid == -1)
	{
		setup_fork_error_context(&fork_ctx, &init_data, current, ctx);
		return (handle_fork_error(&fork_ctx));
	}
	if (pid == 0)
		execute_child_process(current, mini, ctx);
	else
		result = process_parent_for_command(current, pid, ctx);
	return (result);
}
