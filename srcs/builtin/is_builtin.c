/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 18:33:19 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/16 18:08:30 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *command)
{
	if (ft_strcmp(command, "echo") == 0)
		return (1);
	if (ft_strcmp(command, "env") == 0)
		return (1);
	if (ft_strcmp(command, "cd") == 0)
		return (1);
	if (ft_strcmp(command, "pwd") == 0)
		return (1);
	if (ft_strcmp(command, "export") == 0)
		return (1);
	if (ft_strcmp(command, "unset") == 0)
		return (1);
	if (ft_strcmp(command, "exit") == 0)
		return (1);
	return (0);
}

int	execute_builtin(t_command *cmd, t_mini *mini)
{
	if (ft_strcmp(cmd->command, "echo") == 0)
		return (ft_echo(cmd->args));
	else if (ft_strcmp(cmd->command, "env") == 0)
		return (ft_env(mini->env));
	else if (ft_strcmp(cmd->command, "cd") == 0)
		return (ft_cd(cmd->args, mini->env));
	else if (ft_strcmp(cmd->command, "pwd") == 0)
		return (ft_pwd());
	else if (ft_strcmp(cmd->command, "export") == 0)
		return (ft_export(cmd, mini));
	else if (ft_strcmp(cmd->command, "unset") == 0)
		return (ft_unset(cmd, mini));
	else if (ft_strcmp(cmd->command, "exit") == 0)
		return (ft_exit(cmd));
	else
	{
		return (ERR_CMD_NOT_FOUND);
	}
}

void	save_and_restore_std(int *saved_in, int *saved_out, int fd_in,
		int fd_out)
{
	*saved_in = dup(STDIN_FILENO);
	*saved_out = dup(STDOUT_FILENO);
	if (fd_in != -1)
		dup2(fd_in, STDIN_FILENO);
	if (fd_out != -1)
		dup2(fd_out, STDOUT_FILENO);
}

void	restore_std(int saved_in, int saved_out)
{
	if (dup2(saved_in, STDIN_FILENO) == -1
		|| dup2(saved_out, STDOUT_FILENO) == -1)
		exit(ERR_GENERAL);
	close(saved_in);
	close(saved_out);
}
