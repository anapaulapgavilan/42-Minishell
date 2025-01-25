/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 09:54:50 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/17 09:55:19 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_input_redirections(t_command *cmd)
{
	t_input	*input;
	int		fd;
	int		result;

	if (!cmd || !cmd->input)
		return (ERR_SUCCESS);
	input = cmd->input;
	fd = -1;
	while (input)
	{
		if (fd != -1)
			close(fd);
		result = handle_input_condition(input, &fd);
		if (result != ERR_SUCCESS)
			return (result);
		input = input->next;
	}
	update_fd_in(cmd, fd);
	return (ERR_SUCCESS);
}

void	close_unused_pipes(int pipe_fd[2], int has_pipe)
{
	if (has_pipe)
	{
		close(pipe_fd[1]);
	}
	close(pipe_fd[0]);
}

int	save_std_fds_pipe(int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdin == -1 || *saved_stdout == -1)
	{
		return (ERR_PIPE_CREATION);
	}
	return (ERR_SUCCESS);
}

int	handle_input_redirection_pipe(t_command *current,
		t_input_redirection_context *ctx)
{
	int	redir_status;

	if (current->input)
	{
		redir_status = process_input_redirections(current);
		if (redir_status != ERR_SUCCESS)
		{
			close(ctx->saved_stdin);
			close(ctx->saved_stdout);
			return (g_sig.exit_status = redir_status, redir_status);
		}
		if (current->fd_in != -1)
		{
			*(ctx->in_fd) = current->fd_in;
		}
	}
	return (ERR_SUCCESS);
}

void	update_in_fd(t_command *current, int *in_fd)
{
	if (current->fd_in != -1)
		*in_fd = current->fd_in;
}
