/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_flags_init_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:24:20 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/05/27 18:27:28 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"
/**
 * Initialize flags structure with default values.
 * Using the bit-field approach for O(1) operations.
 * Zero out the entire struct and set precision to -1.
 * 
 * Setting precision to -1 avoids ambiguity:
 * - Distinguishes between unspecified precision (default)
 * - Explicitly specified precision of .0, .5, etc.
 * 
 * @return Initialized t_flags structure
 */
t_flags	ft_flags_init(void)
{
	t_flags	flags;

	ft_memset(&flags, 0, sizeof(flags));
	flags.precision = -1;
	return (flags);
}
