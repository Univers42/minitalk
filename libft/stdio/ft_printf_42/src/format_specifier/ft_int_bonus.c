/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_int_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 00:50:23 by syzygy            #+#    #+#             */
/*   Updated: 2025/05/27 18:27:12 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

/**
 * @brief Print the integer number with left alignment
 * @param n The number to be printed
 * @param flags The format flags
 * @return the number of characters printed
 * @note This function print the byte representation of the number
 * @note in the left alignment format
 * @note here we have different condition to verify
 * @note an int_format can rely on those following flags:
 * @note LEFT_FLAG; ZERO_FLAG; DOT_FLAG; PLUS_FLAG; SPACE_FLAG
 */
int	ft_print_int(int n, t_flags flags)
{
	unsigned int	unsigned_n;
	int				count;
	int				num_len;

	count = 0;
	if (n < 0)
		unsigned_n = (unsigned int)-n;
	else
		unsigned_n = (unsigned int)n;
	num_len = ft_nbr_len(unsigned_n, BASE_DEC);
	if ((flags.flags_bits & DOT_FLAG)
		&& (flags.precision == 0 || flags.precision == -1) && n == 0)
		return (ft_handle_zero_precision(n, flags));
	if ((flags.flags_bits & DOT_FLAG) && flags.precision > num_len)
		num_len = flags.precision;
	if (flags.flags_bits & LEFT_FLAG)
		count = ft_print_int_lalign(n, flags, unsigned_n, num_len);
	else
		count = ft_print_int_ralign(n, flags, unsigned_n, num_len);
	return (count);
}
