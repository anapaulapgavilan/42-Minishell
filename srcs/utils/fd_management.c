/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_management.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvaro <alvaro@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:22:43 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/14 01:03:03 by alvaro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	mini_init(t_mini *mini)
{
	mini->input = dup(STDIN);
	mini->output = dup(STDOUT);
	mini->exit_flag = 0;
	mini->return_code = 0;
	mini->skip_exec = 0;
	mini->charge = 0;
	mini->parent = 0;
	mini->last = 0;
	init_fds(mini);
}

void	close_fds(t_mini *mini)
{
	if (mini->fd_input > 0)
	{
		close(mini->fd_input);
	}
	if (mini->fd_output > 0)
	{
		close(mini->fd_output);
	}
	if (mini->pipe_input > 0)
	{
		close(mini->pipe_input);
	}
	if (mini->pipe_output > 0)
	{
		close(mini->pipe_output);
	}
}

void	init_fds(t_mini *mini)
{
	mini->fd_input = -1;
	mini->fd_output = -1;
	mini->pipe_input = -1;
	mini->pipe_output = -1;
	mini->pid = -1;
}
