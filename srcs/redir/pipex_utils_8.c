/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_8.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 10:08:57 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/17 10:09:19 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_redirection_context(t_redirection_context *ctx, int *in_fd)
{
	ctx->saved_stdin = -1;
	ctx->saved_stdout = -1;
	ctx->in_fd = in_fd;
	ctx->err = NULL;
}

void	set_redirection_context(t_redirection_context *ctx, int saved_stdin,
		int saved_stdout, char **err)
{
	ctx->saved_stdin = saved_stdin;
	ctx->saved_stdout = saved_stdout;
	ctx->err = err;
}

int	handle_output_redirection_pipe(t_command *current,
		t_redirection_context *ctx)
{
	int	output_status;

	*(ctx->err) = NULL;
	if (!current->output)
		return (ERR_SUCCESS);
	output_status = redirect_output_pipe(current, ctx->err);
	if (output_status != ERR_SUCCESS)
	{
		handle_output_redirection_error(*(ctx->err), output_status,
			ctx->saved_stdin, ctx->saved_stdout);
		return (g_sig.exit_status = output_status);
	}
	update_in_fd(current, ctx->in_fd);
	return (ERR_SUCCESS);
}

void	init_fork_error_context(t_fork_error_context *ctx,
		t_fork_error_init *init_data)
{
	ctx->current = init_data->current;
	ctx->pipe_fd = init_data->pipe_fd;
	ctx->in_fd = init_data->in_fd;
	ctx->saved_stdin = init_data->saved_stdin;
	ctx->saved_stdout = init_data->saved_stdout;
}

int	handle_fork_error(t_fork_error_context *ctx)
{
	if (ctx->current->has_pipe)
	{
		close(ctx->pipe_fd[0]);
		close(ctx->pipe_fd[1]);
	}
	close(ctx->in_fd);
	close(ctx->saved_stdin);
	close(ctx->saved_stdout);
	return (g_sig.exit_status = ERR_GENERAL, 1);
}
