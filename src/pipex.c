/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alehamad <alehamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 16:25:17 by alehamad          #+#    #+#             */
/*   Updated: 2026/01/12 11:23:21 by alehamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	main(int ac, char **av, char **env)
{
	t_data	pipex;
	int		status;

	if (ac != 5)
		return (1);
	pipex.in = -1;
	pipex.out = -1;
	pipex.fd[0] = -1;
	pipex.fd[1] = -1;
	pipex.cmd_one = NULL;
	pipex.cmd_two = NULL;
	pipex.path = NULL;
	if (init_data(&pipex, av, env))
		return (1);
	status = ft_pipex(&pipex, env);
	free_data(&pipex);
	return (status);
}
