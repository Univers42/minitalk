/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hex_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 00:50:18 by syzygy            #+#    #+#             */
/*   Updated: 2025/05/27 18:27:15 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

int	ft_print_hex_zero(unsigned int num, t_flags flags, int uppercase)
{
	if (flags.flags_bits & DOT_FLAG)
		return (ft_print_hex_zero_with_dot(num, flags, uppercase));
	return (ft_print_hex_zero_regular(flags));
}

/**
 * ##### Summary
 * @param num The number to be printed
 * @param uppercase 1 if the hex should be uppercase, 0 otherwise
 * @param flags the format flags
 * @return the number of charactesr printed
 * @note This functions prints the hexadecimal number in the left aligned format
 * @note here we have different condition to verify 
 * @note an hex_format can rely on those following flags:
 * @note LEFT_FLAG; ZERO_FLAG; DOT_FLAG; HASH_FLAG
 * @note 
 */
int	ft_print_hex_format(unsigned int num, int uppercase, t_flags flags)
{
	int	count;
	int	hex_len;
	int	prefix_len;

	count = 0;
	if (uppercase)
		flags.spec = 'X';
	else
		flags.spec = 'x';
	prefix_len = 0;
	if ((flags.flags_bits & HASH_FLAG) && num != 0)
		prefix_len = 2;
	hex_len = ft_nbr_len(num, 16);
	if (num == 0)
		return (ft_print_hex_zero(num, flags, uppercase));
	if ((flags.flags_bits & DOT_FLAG) && flags.precision > hex_len)
		hex_len = flags.precision;
	if (flags.flags_bits & LEFT_FLAG)
		count += ft_print_hex_left(num, hex_len, prefix_len, flags);
	else
		count += ft_print_hex_right(num, hex_len, prefix_len, flags);
	return (count);
}
