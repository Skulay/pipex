/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quote_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alehamad <alehamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 19:14:36 by alehamad          #+#    #+#             */
/*   Updated: 2025/12/30 19:33:43 by alehamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	ft_isquote(char c)
{
	return (c == '\'' || c == '"');
}

void	ft_quote(char c, char *quote)
{
	if (!ft_isquote(c))
		return ;
	if (*quote == 0)
		*quote = c;
	else if (*quote == c)
		*quote = 0;
}

int	ft_quotes_closed(char const *s)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (s[i])
	{
		ft_quote(s[i], &quote);
		i++;
	}
	return (quote == 0);
}

void	ft_free_quote(char **strs, int i)
{
	while (i >= 0)
	{
		free(strs[i]);
		i--;
	}
	free(strs);
}
