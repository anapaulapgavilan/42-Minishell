/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 15:19:48 by ana-pper          #+#    #+#             */
/*   Updated: 2025/01/25 12:18:45 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_in_child(t_exec_context *ctx, t_mini *mini)
{
	char	*env_str;

	env_str = env_to_str(mini->env);
	ctx->env_array = ft_split(env_str, '\n');
	ft_memdel(env_str);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	execve(ctx->path, ctx->exec_args, ctx->env_array);
	if (execve(ctx->path, ctx->exec_args, ctx->env_array) == -1)
	{
		cleanup_exec_resources(ctx->path, ctx->bin_paths, ctx->exec_args,
			ctx->env_array);
		restore_std(ctx->saved_in, ctx->saved_out);
		return (g_sig.exit_status = ERR_EXEC_FAILED);
	}
	cleanup_exec_resources(ctx->path, ctx->bin_paths, ctx->exec_args,
		ctx->env_array);
	exit(errno);
}

void	update_exit_status(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		g_sig.exit_status = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status))
	{
		g_sig.exit_status = 128 + WTERMSIG(status);
	}
	else
	{
		g_sig.exit_status = ERR_GENERAL;
	}
}

int	redir_and_execute(t_command *cmd, t_mini *mini)
{
	t_exec_context	ctx;
	char			*err;
	int				result;
	pid_t			pid;

	err = NULL;
	result = prepare_command_execution(cmd, &ctx, &err);
	if (result != ERR_SUCCESS)
		return (g_sig.exit_status = result);
	if (is_builtin(cmd->command) && !g_sig.sigint)
		g_sig.exit_status = execute_builtin(cmd, mini);
	else if (!g_sig.sigint)
	{
		result = resolve_command_path(cmd, mini, &ctx);
		if (result != ERR_SUCCESS)
			return (g_sig.exit_status = result);
		pid = fork();
		if (pid == 0)
			execute_in_child(&ctx, mini);
		else if (pid > 0)
			update_exit_status(pid);
		cleanup_path_resources(&ctx);
	}
	cleanup_command_resources(&ctx, cmd);
	return (g_sig.exit_status);
}
