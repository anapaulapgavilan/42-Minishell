/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 20:14:10 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/17 11:32:27 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_in(t_input *in, t_command *list)
{
	int	fd;

	close(list->fd_in);
	fd = -1;
	if (in->condition)
	{
		list->fd_in = heredoc(in->file);
		if (list->fd_in == -1)
			return (ERR_HEREDOC_FAILURE);
	}
	else
	{
		fd = open(in->file, O_RDONLY);
		if (fd == -1)
		{
			if (access(in->file, F_OK) == -1)
				return (ERR_INPUT_FILE_NOT_FOUND);
			if (access(in->file, R_OK) == -1)
				return (ERR_INPUT_PERMISSION_DENIED);
		}
		list->fd_in = fd;
	}
	return (ERR_SUCCESS);
}

int	open_out(t_input *out, t_command *list)
{
	int	fd;

	close(list->fd_out);
	fd = -1;
	if (out->condition)
		fd = open(out->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(out->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		if (access(out->file, W_OK) == -1)
			return (ERR_OUTPUT_PERMISSION_DENIED);
		return (ERR_OUTPUT_FILE_CREATION);
	}
	list->fd_out = fd;
	return (ERR_SUCCESS);
}

int	redirect(t_command *list, char **err)
{
	int	redir_status;

	redir_status = handle_redirection(open_in, list->input, list, err);
	if (redir_status != ERR_SUCCESS)
		return (redir_status);
	redir_status = handle_redirection(open_out, list->output, list, err);
	if (redir_status != ERR_SUCCESS)
		return (redir_status);
	return (ERR_SUCCESS);
}

static int	open_output_file(t_input *out, char **err)
{
	int	flags;
	int	fd;

	if (out->condition)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(out->file, flags, 0644);
	if (fd == -1)
	{
		*err = out->file;
		if (access(out->file, W_OK) == -1)
			return (ERR_OUTPUT_PERMISSION_DENIED);
		return (ERR_OUTPUT_FILE_CREATION);
	}
	return (fd);
}

int	redirect_output_pipe(t_command *cmd, char **err)
{
	t_input	*out;
	int		fd;

	out = cmd->output;
	fd = -1;
	while (out)
	{
		fd = open_output_file(out, err);
		if (fd < 0)
			return (fd);
		if (cmd->fd_out != -1)
		{
			close(cmd->fd_out);
		}
		cmd->fd_out = fd;
		out = out->next;
	}
	return (ERR_SUCCESS);
}
