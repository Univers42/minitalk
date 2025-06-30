/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_precision_hex_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 06:16:36 by dlesieur          #+#    #+#             */
/*   Updated: 2025/05/27 18:26:21 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

/**
 * Applies precision to hexadecimal number printing
 * 
 * @param num The unsigned integer to print in hex
 * @param precision The minimum number of characters to print
 * @param uppercase Flag to determine if hex should use uppercase letters
 * @return Number of characters printed
 */
int	ft_print_hex_precision(unsigned int num, int precision, int uppercase)
{
	int	count;

	count = 0;
	if (num == 0 && precision == -1)
		return (0);
	if (num == 0 && precision > 0)
	{
		count += ft_print_width(precision, 0, 1);
		return (count);
	}
	if (precision > 0)
		count += ft_print_width(precision, ft_nbr_len(num, BASE_HEX), 1);
	count += ft_print_hex_value(num, uppercase);
	return (count);
}

/**
 * Applies precision to pointer hex value printing
 * 
 * @param n The unsigned long to print as a pointer hex value
 * @param precision The minimum number of characters to print
 * @return Number of characters printed
 */
int	ft_print_ptr_hex_precision(size_t n, int precision)
{
	int	count;
	int	hex_len;

	count = 0;
	hex_len = ft_nbr_len(n, BASE_HEX);
	if (precision > hex_len)
		count += ft_print_width(precision, hex_len, 1);
	count += ft_print_hex_value(n, 0);
	return (count);
}
