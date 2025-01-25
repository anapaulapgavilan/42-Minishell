/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 09:55:57 by ana-pper          #+#    #+#             */
/*   Updated: 2025/01/25 11:57:15 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_output_redirection_error(char *err, int output_status,
		int saved_stdin, int saved_stdout)
{
	if (output_status == ERR_OUTPUT_PERMISSION_DENIED)
	{
		ft_putstr_fd("minishell: ", STDERR);
		ft_putstr_fd(err, STDERR);
		ft_putendl_fd(": Permission denied", STDERR);
	}
	else if (output_status == ERR_OUTPUT_FILE_CREATION)
	{
		ft_putstr_fd("minishell: ", STDERR);
		ft_putstr_fd(err, STDERR);
		ft_putendl_fd(": Failed to create output file", STDERR);
	}
	close(saved_stdin);
	close(saved_stdout);
}

int	handle_pipe_creation(int *pipe_fd, int in_fd, int saved_stdin,
		int saved_stdout)
{
	if (pipe(pipe_fd) == -1)
	{
		close(in_fd);
		close(saved_stdin);
		close(saved_stdout);
		return (g_sig.exit_status = ERR_PIPE_CREATION, g_sig.exit_status);
	}
	return (ERR_SUCCESS);
}

void	execute_child_process_pipe(t_command *current, t_mini *mini, int in_fd,
		int *pipe_fd)
{
	int	exec_status;

	if (in_fd != STDIN_FILENO)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
			exit(ERR_GENERAL);
		close(in_fd);
	}
	if (current->has_pipe)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			exit(ERR_GENERAL);
		close(pipe_fd[1]);
		close(pipe_fd[0]);
	}
	exec_status = redir_and_execute_pipe(current, mini);
	exit(exec_status);
}

int	initialize_pipeline_context(t_pipeline_context *ctx)
{
	ctx->in_fd = STDIN_FILENO;
	ctx->child_count = 0;
	ctx->status = 0;
	if (save_std_fds_pipe(&ctx->saved_stdin, &ctx->saved_stdout) != ERR_SUCCESS)
		return (ERR_PIPE_CREATION);
	return (ERR_SUCCESS);
}

int	process_parent_for_command(t_command *current, pid_t pid,
		t_pipeline_context *ctx)
{
	t_parent_process_context	parent_ctx;

	init_parent_process_context(&parent_ctx, current, pid, ctx);
	return (handle_parent_process_pipe(&parent_ctx));
}
