/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alehamad <alehamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 20:24:40 by alehamad          #+#    #+#             */
/*   Updated: 2026/01/14 03:25:28 by alehamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	child_fail(t_data *data, int code_error)
{
	free_data(data);
	exit(code_error);
}

void	free_tab(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	free_data(t_data *data)
{
	if (data->fd[0] >= 0)
		close(data->fd[0]);
	if (data->fd[1] >= 0)
		close(data->fd[1]);
	if (data->in >= 0)
		close(data->in);
	if (data->out >= 0)
		close(data->out);
	if (data->cmd_one)
		free_tab(data->cmd_one);
	if (data->cmd_two)
		free_tab(data->cmd_two);
	if (data->path)
		free_tab(data->path);
}
