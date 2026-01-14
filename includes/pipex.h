/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alehamad <alehamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 10:50:29 by alehamad          #+#    #+#             */
/*   Updated: 2026/01/14 03:27:22 by alehamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdio.h>
# include "../libft/libft.h"

typedef struct s_data
{
	int		fd[2];
	int		in;
	int		out;
	int		status;
	char	*outfile;
	char	**cmd_one;
	char	**cmd_two;
	char	**path;
}	t_data;

char	**make_path(char **env);
char	*find_path_valid(char *av, char **path);
int		init_data(t_data *data, char **av, char **env);
void	exec_child(t_data *p, char **cmd, char **env);
void	exec_child_two(t_data *data, char **cmd, char **env);
void	child_fail(t_data *data, int code_error);
int		ft_pipex(t_data *p, char **env);
void	free_data(t_data *data);
void	free_tab(char **tab);

// ft_split_quote
int		ft_isquote(char c);
void	ft_quote(char c, char *q);
int		ft_quotes_closed(char const *s);
void	ft_free_quote(char **strs, int i);
int		ft_count_quote(char const *str, char sep);
int		ft_strsize_quote(char const *str, char sep);
int		ft_strlencpy_quote(char *dest, char const *src, int len);
char	**ft_split_quote(char const *s, char c);

#endif
