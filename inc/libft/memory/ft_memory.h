/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memory.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:28:23 by dlesieur          #+#    #+#             */
/*   Updated: 2025/05/28 17:19:06 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MEMORY_H
# define FT_MEMORY_H
# include "../render/ft_stddef.h"
# include <stdlib.h>

typedef struct s_mem_tracker
{
	void		**ptrs;
	int			count;
	int			capacity;
}				t_mem_tracker;

void	cleanup_all(t_mem_tracker *tracker);
void	*safe_malloc(t_mem_tracker *tracker, size_t size);
void	init_tracker(t_mem_tracker *tracker);
int		ft_tolower(int c);
int		ft_toupper(int c);
void	ft_swap(void *a, void *b, t_size size);
void	ft_reverse_string(char *str);
void	ft_bzero(void *s, t_size n);
void	*ft_memset(void *ptr, int value, t_size num);
void	*ft_memcpy(void *dest, const void *src, t_size n);
void	*ft_memmove(void *dest, const void *src, t_size n);
void	*ft_memchr(const void *s, int c, t_size n);
int		ft_memcmp(const void *s1, const void *s2, t_size n);
void	*ft_print_memory(void *addr, unsigned int size);
void	ft_print_hex(unsigned char *p, unsigned int size, unsigned long addr);
void	ft_print_ascii(unsigned char *p, unsigned int size);

#endif