/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 09:53:54 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/17 09:54:19 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_file_error(char *prefix, char *file, char *suffix)
{
	ft_putstr_fd(prefix, STDERR);
	ft_putstr_fd(file, STDERR);
	ft_putendl_fd(suffix, STDERR);
}

int	process_heredoc(char *file)
{
	int	fd;

	fd = handle_heredoc(file);
	if (fd == -1)
	{
		print_file_error("minishell: ", file, ": Failed to process heredoc");
		return (-1);
	}
	return (fd);
}

int	open_input_file(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		if (access(file, F_OK) == -1)
		{
			print_file_error("minishell: ", file,
				": No such file or directory");
			g_sig.exit_status = ERR_INPUT_FILE_NOT_FOUND;
		}
		else if (access(file, R_OK) == -1)
		{
			print_file_error("minishell: ", file, ": Permission denied");
			g_sig.exit_status = ERR_INPUT_PERMISSION_DENIED;
		}
		return (-1);
	}
	return (fd);
}

void	update_fd_in(t_command *cmd, int new_fd)
{
	if (new_fd != -1)
	{
		if (cmd->fd_in != -1)
		{
			close(cmd->fd_in);
		}
		cmd->fd_in = new_fd;
	}
}

int	handle_input_condition(t_input *input, int *fd)
{
	if (input->condition)
	{
		*fd = process_heredoc(input->file);
		if (*fd == -1)
			return (ERR_HEREDOC_FAILURE);
	}
	else
	{
		*fd = open_input_file(input->file);
		if (*fd == -1)
			return (g_sig.exit_status);
	}
	return (ERR_SUCCESS);
}
