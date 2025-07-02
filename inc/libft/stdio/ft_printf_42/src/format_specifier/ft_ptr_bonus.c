/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ptr_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 00:50:32 by syzygy            #+#    #+#             */
/*   Updated: 2025/05/27 18:27:04 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

/**
 * @brief Print the null pointer with formatting
 * @param n The number to be printed
 * @param flags the format flags
 * @return the number of characters printed.
 * @return 2 if the pointer is null
 * @note we have two variables `hex_len` and `ptr_len`
 * @note First we start the algo with a base case that 
 * @note handle null pointer. to print the pointer we first need to get
 * @note the number of digits in base 16. We adjust if precision is set
 * @note to be greater than the number of digits. We then calculate the
 * @note total length of the pointer. If the left flag is set we print
 * @note the pointer with the left flag. Otherwise we print the pointer
 * @note with the right flag.
 * @example printf("%p", (void *)0x1234);
 * printf("%p", (void *)0);
 * n = 1234 ==> not NULL ==> skip base case
 * ft_nbr_len(1234, 16) = 4 ==> hex_len = 4
 * ptr_len = 2 + 4 = 6;
 * alignment check : 
 * if left flag is set ==> call ft_print_ptr_left(0x1234, flags, 6)
 * else ==> call ft_print_ptr_right(0x1234, flags, 6, 4)
 * 
 * If the pointer is NULL
 * n = 0 ==> NULL ==> return ft_print_null_ptr(flags)
 * return 2;
 * print (nil)
 */
int	ft_print_ptr(unsigned long n, t_flags flags)
{
	int	hex_len;
	int	ptr_len;

	if (n == 0)
		return (ft_print_null_ptr(flags));
	hex_len = ft_nbr_len(n, BASE_HEX);
	if ((flags.flags_bits & DOT_FLAG) && flags.precision > hex_len)
		hex_len = flags.precision;
	ptr_len = 2 + hex_len;
	if (flags.flags_bits & LEFT_FLAG)
		return (ft_print_ptr_left(n, flags, ptr_len));
	else
		return (ft_print_ptr_right(n, flags, ptr_len, hex_len));
}
