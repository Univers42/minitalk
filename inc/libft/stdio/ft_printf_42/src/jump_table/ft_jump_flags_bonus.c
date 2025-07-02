/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_jump_flags_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 06:16:26 by dlesieur          #+#    #+#             */
/*   Updated: 2025/05/27 18:26:28 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

/* Internal helper function to set a flag bit */
/**
 * Sets the corresponding flag bit in the given flags structure 
 * based on the flag character.
 *
 * @param flag_char The flag character to set the bit for.
 * @param flags     The flags structure to update.
 * @note The value `0x7F` is used to mask down the flag character to a 7-bit
 * @note ASCII character. This is done to ensure that the flag character is
 * @note within the valid range of the flag map array.
 * 
 * ### Example
 * 
 * @note if the flag character is `1` (ASCII value 49), the `0x7F` mask will
 * @note reduce it to `1` (ASCII value 1), which is within the valid range of
 * @note the flag map array.
 * 01100001 & 01111111 = 01100001 (within the range)
 * 10000000 & 01111111 = 00000000 (out of range)
 */
static void	set_flag_bit_internal(int flag_char, t_flags *flags)
{
	static const unsigned int	flag_map[128] = {
	['0'] = ZERO_FLAG,
	['-'] = LEFT_FLAG,
	['.'] = DOT_FLAG,
	['#'] = HASH_FLAG,
	[' '] = SPACE_FLAG,
	['+'] = PLUS_FLAG,
	['*'] = STAR_FLAG
	};

	flags->flags_bits |= flag_map[flag_char & 0x7F];
}

/**
 * Sets the flag bit for the given flag character in the specified 
 * flags structure.
 *
 * @param flag_char The flag character to set the bit for.
 * @param flags     The flags structure to modify.
 */
void	set_flag_bit(int flag_char, t_flags *flags)
{
	set_flag_bit_internal(flag_char, flags);
}

/**
 * Handles the minus flag.
 * This function sets the minus flag bit in the given flags structure 
 * and clears the zero flag bit.
 *
 * @param flags The flags structure to modify.
 * @param args  The va_list of arguments (unused in this function).
 */
void	handle_minus(t_flags *flags, va_list args)
{
	(void)args;
	set_flag_bit_internal('-', flags);
	flags->flags_bits &= (unsigned int) ~ZERO_FLAG;
}

/**
 * Handles the '0' flag for the ft_printf function.
 *
 * This function sets the '0' flag bit in the given flags structure.
 * The '0' flag specifies that the output should be padded with zeros.
 *
 * @param flags The flags structure.
 * @param args  The variable argument list.
 */
void	handle_zero(t_flags *flags, va_list args)
{
	(void)args;
	set_flag_bit_internal('0', flags);
}

/**
 * Handles the dot flag in the ft_printf function.
 * This flag is used to specify precision for floating-point numbers.
 *
 * @param flags The pointer to the flags structure.
 * @param args The variable argument list.
 */
void	handle_dot(t_flags *flags, va_list args)
{
	(void)args;
	set_flag_bit_internal('.', flags);
	flags->flags_bits &= (unsigned int) ~ZERO_FLAG;
	flags->precision = 0;
}
