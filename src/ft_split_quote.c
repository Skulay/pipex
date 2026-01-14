/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alehamad <alehamad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 15:39:21 by alehamad          #+#    #+#             */
/*   Updated: 2026/01/03 05:42:29 by alehamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	ft_count_quote(char const *str, char sep)
{
	int		i;
	int		count;
	char	quote;

	i = 0;
	count = 0;
	quote = 0;
	while (str[i])
	{
		while (str[i] == sep)
			i++;
		if (!str[i])
			break ;
		count++;
		while (str[i])
		{
			if (quote == 0 && str[i] == sep)
				break ;
			ft_quote(str[i], &quote);
			i++;
		}
	}
	return (count);
}

int	ft_strsize_quote(char const *str, char sep)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (str[i])
	{
		if (quote == 0 && str[i] == sep)
			break ;
		ft_quote(str[i], &quote);
		i++;
	}
	return (i);
}

int	ft_strlencpy_quote(char *dest, char const *src, int len)
{
	int		i;
	int		j;
	char	quote;
	char	prev;

	i = 0;
	j = 0;
	quote = 0;
	while (src[i] && i < len)
	{
		prev = quote;
		ft_quote(src[i], &quote);
		if (!(ft_isquote(src[i]) && (prev == 0 || prev == src[i])))
			dest[j++] = src[i];
		i++;
	}
	dest[j] = '\0';
	return (len);
}

char	**ft_split_quote(char const *s, char c)
{
	char	**strs;
	int		count;
	int		i;
	int		pos;
	int		len;

	if (!s || !ft_quotes_closed(s))
		return (NULL);
	count = ft_count_quote(s, c);
	strs = (char **)ft_calloc((count + 1), sizeof(char *));
	if (!strs)
		return (NULL);
	i = -1;
	pos = 0;
	while (++i < count)
	{
		while (s[pos] == c)
			pos++;
		len = ft_strsize_quote(&s[pos], c);
		strs[i] = (char *)malloc((len + 1) * sizeof(char));
		if (!strs[i])
			return (ft_free_quote(strs, i - 1), NULL);
		pos += ft_strlencpy_quote(strs[i], &s[pos], len);
	}
	return (strs);
}
