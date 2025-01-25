/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 11:02:22 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/17 11:32:26 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redirection(int (*open_func)(t_input *, t_command *),
		t_input *redir, t_command *list, char **err)
{
	int	redir_status;

	while (redir && !g_sig.sigint && !g_sig.sigquit)
	{
		redir_status = open_func(redir, list);
		if (redir_status != ERR_SUCCESS)
		{
			*err = redir->file;
			return (redir_status);
		}
		redir = redir->next;
	}
	return (ERR_SUCCESS);
}

static int	read_and_write_heredoc_line(int write_fd, const char *delim)
{
	char	*line;

	line = readline("heredoc> ");
	if (!line || ft_strcmp(line, delim) == 0)
	{
		free(line);
		return (1);
	}
	if (write(write_fd, line, ft_strlen(line)) == -1 || write(write_fd, "\n",
			1) == -1)
	{
		free(line);
		return (-1);
	}
	free(line);
	return (0);
}

int	heredoc(const char *delim)
{
	int	pipe_fd[2];
	int	result;

	if (pipe(pipe_fd) == -1)
		return (ERR_PIPE_CREATION);
	g_sig.in_heredoc = 1;
	while (!g_sig.sigint)
	{
		result = read_and_write_heredoc_line(pipe_fd[1], delim);
		if (result == 1)
			break ;
		if (result == -1)
		{
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			g_sig.in_heredoc = 0;
			return (ERR_PIPE_EXECUTION);
		}
	}
	close(pipe_fd[1]);
	g_sig.in_heredoc = 0;
	return (pipe_fd[0]);
}
