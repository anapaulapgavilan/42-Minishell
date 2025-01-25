/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:23:17 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/17 10:31:10 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		args[i] = NULL;
		i++;
	}
	free(args);
	args = NULL;
}

void	free_command_node(t_command *cmd)
{
	if (cmd)
	{
		if (cmd->command)
		{
			free(cmd->command);
			cmd->command = NULL;
		}
		if (cmd->args)
			free_args(cmd->args);
		if (cmd->input)
			free_input_list(&cmd->input);
		if (cmd->output)
			free_input_list(&cmd->output);
		free(cmd);
		cmd = NULL;
	}
}

void	free_command_resources(t_command *cmd)
{
	if (cmd->input)
		free_input_list(&cmd->input);
	if (cmd->output)
		free_input_list(&cmd->output);
	if (cmd->fd_in != -1)
	{
		close(cmd->fd_in);
		cmd->fd_in = -1;
	}
	if (cmd->fd_out != -1)
	{
		close(cmd->fd_out);
		cmd->fd_out = -1;
	}
}

void	free_args_list(char **args, size_t args_size)
{
	size_t	i;

	if (!args)
		return ;
	i = 0;
	while (i < args_size)
	{
		if (args[i])
		{
			free(args[i]);
			args[i] = NULL;
		}
		i++;
	}
	free(args);
	args = NULL;
}

void	free_command_list(t_command *cmd)
{
	t_command	*tmp;

	while (cmd)
	{
		tmp = cmd;
		cmd = cmd->next;
		if (tmp->command)
		{
			free(tmp->command);
			tmp->command = NULL;
		}
		if (tmp->args)
			free_args_list(tmp->args, tmp->args_size);
		free_command_resources(tmp);
		free(tmp);
	}
}
