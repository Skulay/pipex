/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alehamad <alehamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 16:25:42 by alehamad          #+#    #+#             */
/*   Updated: 2026/01/14 04:06:07 by alehamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

char	**make_path(char **env)
{
	char	**strs;
	char	*tmp;
	int		i;

	i = 0;
	while (env[i] && ft_strncmp("PATH=", env[i], 5))
		i++;
	if (!env[i])
		return (NULL);
	strs = ft_split(env[i], ':');
	if (!strs)
		return (NULL);
	tmp = strs[0];
	strs[0] = ft_strtrim(tmp, "PATH=");
	free(tmp);
	i = -1;
	while (strs[++i])
	{
		tmp = strs[i];
		strs[i] = ft_strjoin(tmp, "/");
		free(tmp);
	}
	return (strs);
}

char	*find_path_valid(char *av, char **path)
{
	int		i;
	char	*valid_path;

	if (!path || !av)
		return (NULL);
	i = 0;
	if (ft_strchr(av, '/'))
	{
		if (access(av, F_OK) == 0)
			return (ft_strdup(av));
		return (NULL);
	}
	while (path[i])
	{
		valid_path = ft_strjoin(path[i], av);
		if (access(valid_path, X_OK) == 0)
			return (valid_path);
		free(valid_path);
		i++;
	}
	return (NULL);
}
