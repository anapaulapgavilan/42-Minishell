/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 18:47:08 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/15 18:48:33 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	g_sig.sigint = 1;
	g_sig.exit_status = ERR_SIGINT;
	if (rl_line_buffer && *rl_line_buffer)
	{
		write(STDOUT_FILENO, "\033[2K\r", 4);
		rl_on_new_line();
	}
	rl_replace_line("", 0);
	if (g_sig.in_heredoc)
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
	else
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	handle_eof(t_mini *mini)
{
	if (isatty(STDIN_FILENO))
		printf("exit\n");
	free_env(mini->env);
	free_env(mini->secret_env);
	exit(0);
}

void	handle_sigquit(int sig)
{
	(void)sig;
	g_sig.exit_status = ERR_SIGQUIT;
}

void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
	{
		perror("sigaction SIGINT");
		exit(ERR_RETURN_BASE);
	}
	sa_quit.sa_handler = handle_sigquit;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
	{
		perror("sigaction SIGQUIT");
		exit(ERR_RETURN_BASE);
	}
	g_sig.exit_status = 0;
	g_sig.in_heredoc = 0;
	g_sig.sigint = 0;
	g_sig.sigquit = 0;
}
