/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_ptr_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 06:16:08 by dlesieur          #+#    #+#             */
/*   Updated: 2025/05/27 18:26:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

/**
 * Optimized version that handles both pointer and regular hex printing
 * 
 * @param n The number to print in hex
 * @param uppercase Flag to use uppercase letters for hex
 * @return Number of characters printed
 */
int	ft_print_hex_value(size_t n, int uppercase)
{
	int		count;
	char	*hex_digits;

	count = 0;
	if (uppercase)
		hex_digits = HEX_DIGIT_UPPER;
	else
		hex_digits = HEX_DIGIT_LOWER;
	if (n >= BASE_HEX)
		count += ft_print_hex_value(n / BASE_HEX, uppercase);
	count += ft_print_c(hex_digits[n % BASE_HEX]);
	return (count);
}

/**
 * Prints a pointer value in hexadecimal format
 * 
 * @param n The pointer value to print
 * @return The number of characters printed
 */
int	ft_print_ptr_hex(unsigned long n)
{
	return (ft_print_hex_value(n, 0));
}

/**
 * Prints the hexadecimal prefix ("0x" or "0X")
 */
t_ssize	ft_print_hex_prefix(int uppercase)
{
	if (uppercase)
		return (write(1, "0X", 2));
	else
		return (write(1, "0x", 2));
}
