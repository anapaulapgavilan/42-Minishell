/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 08:49:42 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/17 09:53:00 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_pipe(int *pipe_fd)
{
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (g_sig.exit_status = ERR_PIPE_CREATION, g_sig.exit_status);
	}
	return (ERR_SUCCESS);
}

int	write_heredoc_line(int pipe_write_fd, const char *line)
{
	if (write(pipe_write_fd, line, ft_strlen(line)) == -1
		|| write(pipe_write_fd, "\n", 1) == -1)
	{
		perror("Failed to write to heredoc pipe");
		return (ERR_HEREDOC_FAILURE);
	}
	return (ERR_SUCCESS);
}

int	process_heredoc_input(int *pipe_fd, const char *delim)
{
	char	*line;

	while (!g_sig.sigint)
	{
		line = readline("heredoc> ");
		if (!line || ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		if (write_heredoc_line(pipe_fd[1], line) != ERR_SUCCESS)
		{
			free(line);
			return (ERR_HEREDOC_FAILURE);
		}
		free(line);
	}
	return (ERR_SUCCESS);
}

int	create_heredoc_pipe(int *pipe_fd)
{
	if (pipe(pipe_fd) == -1)
	{
		perror("Failed to create heredoc pipe");
		return (g_sig.exit_status = ERR_PIPE_CREATION, ERR_PIPE_CREATION);
	}
	return (ERR_SUCCESS);
}

int	handle_heredoc(const char *delim)
{
	int	pipe_fd[2];

	if (create_heredoc_pipe(pipe_fd) != ERR_SUCCESS)
		return (g_sig.exit_status);
	if (process_heredoc_input(pipe_fd, delim) != ERR_SUCCESS)
	{
		close(pipe_fd[1]);
		close(pipe_fd[0]);
		return (g_sig.exit_status);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}
