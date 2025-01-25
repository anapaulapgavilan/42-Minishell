/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils_4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 19:10:37 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/16 19:11:25 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*find_path_env(t_env *env)
{
	while (env && ft_strncmp(env->value, "PATH=", 5) != 0)
		env = env->next;
	return (env);
}

int	handle_path_not_set(t_exec_context *ctx)
{
	ft_putstr_fd("minishell: PATH not set\n", STDERR);
	restore_std(ctx->saved_in, ctx->saved_out);
	free(ctx->bin_paths);
	free(ctx->exec_args);
	return (g_sig.exit_status = ERR_CMD_NOT_FOUND);
}

int	handle_command_not_found(t_command *cmd, t_exec_context *ctx)
{
	ft_putstr_fd("minishell: ", STDERR);
	ft_putstr_fd(cmd->command, STDERR);
	ft_putendl_fd(": command not found", STDERR);
	restore_std(ctx->saved_in, ctx->saved_out);
	free_tab(ctx->bin_paths);
	free(ctx->exec_args);
	return (g_sig.exit_status = ERR_CMD_NOT_FOUND);
}

int	check_direct_command(t_command *cmd, t_exec_context *ctx)
{
	if (access(cmd->command, X_OK) == 0)
	{
		ctx->path = ft_strdup(cmd->command);
		return (ERR_SUCCESS);
	}
	return (ERR_CMD_NOT_FOUND);
}

int	check_path_command(const char *bin_path, const char *command,
		char **result_path)
{
	char	*temp;

	temp = path_join(bin_path, command);
	if (access(temp, X_OK) == 0)
	{
		*result_path = temp;
		return (ERR_SUCCESS);
	}
	free(temp);
	return (ERR_CMD_NOT_FOUND);
}
