/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 18:54:16 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/16 20:48:53 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	save_std_fds(int *saved_in, int *saved_out)
{
	*saved_in = dup(STDIN_FILENO);
	*saved_out = dup(STDOUT_FILENO);
	if (*saved_in == -1 || *saved_out == -1)
		return (ERR_GENERAL);
	return (ERR_SUCCESS);
}

void	print_input_error(int redir_status, char *err)
{
	if (redir_status == ERR_INPUT_FILE_NOT_FOUND)
	{
		ft_putstr_fd(err, STDERR);
		ft_putendl_fd(": No such file or directory", STDERR);
	}
	else if (redir_status == ERR_INPUT_PERMISSION_DENIED)
	{
		ft_putstr_fd(err, STDERR);
		ft_putendl_fd(": Permission denied", STDERR);
	}
	else if (redir_status == ERR_HEREDOC_FAILURE)
	{
		ft_putstr_fd("Error processing heredoc: ", STDERR);
		ft_putendl_fd(err, STDERR);
	}
}

void	print_output_error(int redir_status, char *err)
{
	if (redir_status == ERR_OUTPUT_PERMISSION_DENIED)
	{
		ft_putstr_fd(err, STDERR);
		ft_putendl_fd(": Permission denied", STDERR);
	}
	else if (redir_status == ERR_OUTPUT_FILE_CREATION)
	{
		ft_putstr_fd(err, STDERR);
		ft_putendl_fd(": Failed to create or open output file", STDERR);
	}
	else
	{
		ft_putendl_fd(": Unknown redirection error", STDERR);
	}
}

void	print_redirection_error(int redir_status, char *err)
{
	if (redir_status == ERR_INPUT_FILE_NOT_FOUND
		|| redir_status == ERR_INPUT_PERMISSION_DENIED
		|| redir_status == ERR_HEREDOC_FAILURE)
	{
		print_input_error(redir_status, err);
	}
	else
	{
		print_output_error(redir_status, err);
	}
}

int	handle_redirection_error(int redir_status, char *err, int saved_in,
		int saved_out)
{
	print_redirection_error(redir_status, err);
	restore_std(saved_in, saved_out);
	return (g_sig.exit_status = redir_status);
}
