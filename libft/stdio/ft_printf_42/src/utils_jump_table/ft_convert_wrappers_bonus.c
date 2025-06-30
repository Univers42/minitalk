/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert_wrappers_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:02:54 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/05/27 18:25:59 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

/**
 * The funny syntax you're referring to is the use of (void) to explicitly 
 * cast the unused parameters type and args to void. 
 * This is done to avoid compiler warnings about unused variables. 
 * By casting them to void, it indicates to the compiler that 
 * the variables are intentionally unused and prevents
 * the warnings from being generated.
 * This concept is called "casting to void" or "type casting to void".
 * As for why all the functions have the same prototype,
 * it is because they are part of a design pattern called the "Jump Table" 
 * or "Function Pointer Table". In this pattern, a table of 
 * function pointers is used to map different format specifiers to their 
 * corresponding conversion functions. The functions in the table have 
 * the same prototype to ensure consistency and ease of use when 
 * accessing them through the table.
 * By having the same prototype, it becomes easier to iterate over the table 
 * and call the appropriate conversion function based on the format specifier. 
 * It also allows for a more modular and extensible design, 
 * as new conversion functions can be added to the table 
 * without modifying the calling code.
 */

/**
 * Converts a percent sign (%) format specifier.
 *
 * This function is responsible for converting a percent sign format specifier
 * and printing it according to the provided flags.
 *
 * @param type The format specifier type (ignored in this function).
 * @param flags The flags to apply during the conversion and printing.
 * @param args The variable argument list (ignored in this function).
 * @return The number of characters printed.
 */
int	ft_convert_percent(char type, t_flags flags, va_list args)
{
	(void)type;
	(void)args;
	return (ft_print_percent(flags));
}

/**
 * Converts a character argument to a string and prints it according 
 * to the given flags.
 *
 * @param type The type specifier character.
 * @param flags The flags to apply during printing.
 * @param args The variable argument list.
 * @return The number of characters printed.
 */
int	ft_convert_char(char type, t_flags flags, va_list args)
{
	(void)type;
	return (ft_print_char((unsigned char)va_arg(args, int), flags));
}

/**
 * Converts a string argument and prints it according to the given flags.
 *
 * @param type The conversion specifier character.
 * @param flags The flags to apply during the conversion and printing.
 * @param args The variable argument list containing the string argument.
 * @return The number of characters printed.
 */
int	ft_convert_str(char type, t_flags flags, va_list args)
{
	(void)type;
	return (ft_print_str(va_arg(args, char *), flags));
}

/**
 * Converts an integer argument based on the provided type and flags.
 *
 * @param type The conversion type.
 * @param flags The formatting flags.
 * @param args The variable argument list.
 * @return The number of characters printed.
 */
int	ft_convert_int(char type, t_flags flags, va_list args)
{
	(void)type;
	return (ft_print_int(va_arg(args, int), flags));
}

/**
 * Converts an unsigned integer to a string and prints it according 
 * to the given flags.
 *
 * @param type The type specifier character.
 * @param flags The flags to apply during printing.
 * @param args The variable argument list.
 * @return The number of characters printed.
 */
int	ft_convert_unsigned(char type, t_flags flags, va_list args)
{
	(void)type;
	return (ft_print_unsigned(va_arg(args, unsigned int), flags));
}
