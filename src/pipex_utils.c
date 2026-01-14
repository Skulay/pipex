/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alehamad <alehamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 15:36:45 by alehamad          #+#    #+#             */
/*   Updated: 2026/01/14 03:59:34 by alehamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

static void	exec_or_error(t_data *data, char *path, char **cmd, char **env)
{
	execve(path, cmd, env);
	perror(cmd[0]);
	free(path);
	child_fail(data, 126);
}

int	init_data(t_data *data, char **av, char **env)
{
	data->status = 0;
	data->in = open(av[1], O_RDONLY);
	if (data->in < 0)
	{
		perror(av[1]);
		data->in = open("/dev/null", O_RDONLY);
	}
	data->out = -1;
	data->outfile = av[4];
	data->cmd_one = ft_split_quote(av[2], ' ');
	if (!data->cmd_one)
		return (free_data(data), 1);
	data->cmd_two = ft_split_quote(av[3], ' ');
	if (!data->cmd_two)
		return (free_data(data), 1);
	data->path = make_path(env);
	return (0);
}

void	exec_child(t_data *data, char **cmd, char **env)
{
	char	*path;

	if (!cmd || !cmd[0] || !cmd[0][0])
		child_fail(data, 127);
	close(data->fd[0]);
	if (dup2(data->in, STDIN_FILENO) < 0
		|| dup2(data->fd[1], STDOUT_FILENO) < 0)
	{
		perror("dup2");
		child_fail(data, 1);
	}
	close(data->in);
	close(data->fd[1]);
	path = find_path_valid(cmd[0], data->path);
	if (!path)
	{
		ft_putstr_fd(cmd[0], 2);
		ft_putendl_fd(": command not found", 2);
		child_fail(data, 127);
	}
	exec_or_error(data, path, cmd, env);
}

void	exec_child_two(t_data *data, char **cmd, char **env)
{
	char	*path;

	if (!cmd || !cmd[0] || !cmd[0][0])
		child_fail(data, 127);
	if ((data->out = open(data->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)
	{
		perror(data->outfile);
		child_fail(data, 1);
	}
	close(data->fd[1]);
	if (dup2(data->fd[0], STDIN_FILENO) < 0
		|| dup2(data->out, STDOUT_FILENO) < 0)
	{
		perror("dup2");
		child_fail(data, 1);
	}
	close(data->fd[0]);
	path = find_path_valid(cmd[0], data->path);
	if (!path)
	{
		ft_putstr_fd(cmd[0], 2);
		ft_putendl_fd(": command not found", 2);
		child_fail(data, 127);
	}
	exec_or_error(data, path, cmd, env);
}

int	ft_pipex(t_data *data, char **env)
{
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(data->fd) == -1)
		return (1);
	pid1 = fork();
	if (pid1 < 0)
		return (1);
	if (pid1 == 0)
		exec_child(data, data->cmd_one, env);
	pid2 = fork();
	if (pid2 < 0)
		return (1);
	if (pid2 == 0)
		exec_child_two(data, data->cmd_two, env);
	close(data->fd[0]);
	close(data->fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &data->status, 0);
	if (WIFEXITED(data->status))
		return (WEXITSTATUS(data->status));
	if (WIFSIGNALED(data->status))
		return (128 + WTERMSIG(data->status));
	return (1);
}
