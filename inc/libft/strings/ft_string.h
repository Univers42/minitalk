/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:33:14 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/08 12:34:08 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STRING_H
# define FT_STRING_H

# include <stddef.h>
# include "../render/ft_stddef.h"

t_size	ft_strlen(const char *s);
t_size	ft_strlcpy(char *dst, const char *src, t_size dsize);
t_size	ft_strlcat(char *dst, const char *src, t_size dsize);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
char	*ft_strstr(const char *haystack, const char *needle);
char	*ft_strnstr(const char *haystack, const char *needle, t_size len);
char	*ft_strjoin(const char *s1, const char *s2);
char	*ft_substr(const char *s, unsigned int start, t_size len);
char	*ft_strtrim(const char *s1, const char *set);
char	**ft_split(const char *s, char c);
char	*ft_strmapi(const char *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
int		ft_strncmp(const char *s1, const char *s2, t_size n);
char	*ft_strnstr(const char *big, const char *little, t_size len);
char	*ft_strdup(const char *s);
t_size	ft_strclen(const char *s, int c);
char	*ft_strndup(const char *s, t_size n);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_countwords(const char *s1, const char limiter);
int		ft_countwords_space(const char *s1);
long	ft_atol(const char *str);
char	*ft_strnjoin(const char *first, ...);
char	*ft_strncpy(char *dest, const char *src, size_t n);
char	*ft_strstr(const char *haystack, const char *needle);
char	*ft_strtok(char *str, const char *delim);
#endif