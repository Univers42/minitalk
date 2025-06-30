/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:06:58 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/26 14:12:11 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_stdlib.h"

void	*ft_realloc(void *ptr, t_size old, t_size new)
{
	void	*new_ptr;
	t_size	copy_size;

	if (new == 0)
		return (free(ptr), NULL);
	new_ptr = malloc(new);
	if (!new_ptr)
		return (NULL);
	if (ptr)
	{
		if (old < new)
			copy_size = old;
		else
			copy_size = new;
		(ft_memcpy(new_ptr, ptr, copy_size), free(ptr));
	}
	return (new_ptr);
}
