/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 16:17:17 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/17 10:11:07 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_parent_process_context(t_parent_process_context *ctx,
		t_command *current, int pid, t_pipeline_context *pipeline_ctx)
{
	ctx->current = current;
	ctx->pid = pid;
	ctx->pipe_fd = pipeline_ctx->pipe_fd;
	ctx->in_fd = &pipeline_ctx->in_fd;
	ctx->child_pids = pipeline_ctx->child_pids;
	ctx->child_count = &pipeline_ctx->child_count;
	ctx->status = &pipeline_ctx->status;
}

int	handle_parent_process_pipe(t_parent_process_context *ctx)
{
	signal(SIGINT, SIG_IGN);
	ctx->child_pids[(*ctx->child_count)++] = ctx->pid;
	if (ctx->current->has_pipe)
	{
		close(ctx->pipe_fd[1]);
	}
	if (*(ctx->in_fd) != STDIN_FILENO)
	{
		close(*(ctx->in_fd));
	}
	if (ctx->current->has_pipe)
		*(ctx->in_fd) = ctx->pipe_fd[0];
	else
		*(ctx->in_fd) = STDIN_FILENO;
	waitpid(ctx->pid, ctx->status, 0);
	setup_signals();
	if (WIFEXITED(*(ctx->status)) && WEXITSTATUS(*(ctx->status)) != 0)
		return (ERR_GENERAL);
	return (ERR_SUCCESS);
}

int	handle_pipeline_setup(t_command *current,
		t_pipeline_setup_context *setup_ctx)
{
	t_redirection_context		redir_ctx;
	t_input_redirection_context	input_ctx;
	char						*err;

	err = NULL;
	input_ctx.in_fd = setup_ctx->in_fd;
	input_ctx.saved_stdin = setup_ctx->saved_stdin;
	input_ctx.saved_stdout = setup_ctx->saved_stdout;
	if (handle_input_redirection_pipe(current, &input_ctx) != ERR_SUCCESS)
		return (g_sig.exit_status);
	init_redirection_context(&redir_ctx, setup_ctx->in_fd);
	set_redirection_context(&redir_ctx, setup_ctx->saved_stdin,
		setup_ctx->saved_stdout, &err);
	if (handle_output_redirection_pipe(current, &redir_ctx) != ERR_SUCCESS)
		return (g_sig.exit_status);
	if (current->has_pipe)
	{
		if (handle_pipe_creation(setup_ctx->pipe_fd, *(setup_ctx->in_fd),
				setup_ctx->saved_stdin, setup_ctx->saved_stdout) != ERR_SUCCESS)
			return (g_sig.exit_status);
	}
	return (ERR_SUCCESS);
}

int	redir_and_execute_pipe(t_command *cmd, t_mini *mini)
{
	t_exec_context	ctx;
	int				result;

	initialize_exec_context(&ctx);
	if (setup_file_descriptors(cmd, &ctx) != ERR_SUCCESS)
		return (g_sig.exit_status);
	if (setup_execution_args(cmd, &ctx) != ERR_SUCCESS)
		return (g_sig.exit_status);
	if (is_builtin(cmd->command) && !g_sig.sigint)
		g_sig.exit_status = execute_builtin(cmd, mini);
	else if (!g_sig.sigint)
	{
		result = resolve_command_path(cmd, mini, &ctx);
		if (result != ERR_SUCCESS)
			return (result);
		result = execute_command(&ctx, mini);
		if (result != ERR_SUCCESS)
			return (result);
		close_saved_file_descriptors(&ctx);
	}
	cleanup_exec_resources(ctx.path, ctx.bin_paths, ctx.exec_args,
		ctx.env_array);
	return (g_sig.exit_status = ERR_SUCCESS, g_sig.exit_status);
}
