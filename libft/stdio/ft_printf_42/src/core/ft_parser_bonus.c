/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 14:33:45 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/05/27 18:27:25 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

/**
 * Handles width and precision values efficiently using
 * pointer arithmetic
 * @param format the first bytes the pointer point out
 * @param i the index of the current character
 * @param flags the flags structure to update
 * @return the updated index
 * @note This function is called when a digit is found
 * @note its purpose is to distinguish between with and
 * @note precision values and update the corresponding
 * @note if DOT_FLAG is set, it updates the precision
 * @note otherwise, it updates the width
 * @note it convert number from characters to integers
 */
static int	ft_process_number(const char *format, int i, t_flags *flags)
{
	int	*target;

	if (flags->flags_bits & DOT_FLAG)
		target = &flags->precision;
	else
		target = &flags->width;
	*target = 0;
	while (ft_isdigit(format[i]))
	{
		*target = *target * 10 + (format[i] - '0');
		i++;
	}
	return (i);
}

/**
 * Uses lookup tables for efficient flag processing
 * and delegates number parsing to `ft_process_number`
 * @note: This function is splitted into two parts
 * @note to keep the code clean and readable
 * @note it gets the current character and checks if 
 * @note there's a flag handler for it. If there is a flag
 * @note handler, it calls the corresponding function
 * @note otherwise, it checkes if the character is a digit
 * @note to determine if it's a width or precision value
 * @note process the number and update the index
 * @note finally, it returns the updated index
 */
static int	ft_parse_flags(const char *format, int i, t_flags *flags,
							va_list args)
{
	t_lookup_tables	*tables;
	unsigned char	c;

	tables = get_tables();
	while (format[i] && !ft_istype(format[i]))
	{
		c = (unsigned char)format[i];
		if (flags->flags_bits & DOT_FLAG && c == '*')
		{
			flags->precision = va_arg(args, int);
			if (flags->precision < 0)
				flags->flags_bits &= (unsigned int) ~DOT_FLAG;
			i++;
			continue ;
		}
		if (tables->flag_table[c])
			(tables->flag_table[c](flags, args), i++);
		else if (ft_isdigit(c))
			i = ft_process_number(format, i, flags);
		else
			i++;
	}
	return (i);
}

/* Provides flexible conversion handling through function pointers*/
/**
 * This function processes the conversion specifier
 * and delegates the conversion to the corresponding
 * function pointer
 * @param type the conversion specifier
 * @param flags the flags structure
 * @param args the va_list
 * @return the number of characters printed
 * @note it gets the lookup tables and the corresponding
 * @note conversion function pointer
 * @note if the conversion function pointer is NULL
 * @note it calls the invalid format handler
 * @note otherwise, it calls the conversion function
 */
static int	ft_process_conversion(char type, t_flags flags, va_list args)
{
	t_lookup_tables	*tables;
	t_convert_func	converter;

	tables = get_tables();
	converter = tables->convert_table[(unsigned char)type];
	if (!converter)
		return (ft_handle_invalid_format(type, flags));
	return (converter(type, flags, args));
}

/**
 * This function process the format literal string
 * and delegates the conversion to the corresponding
 * function pointer
 * @param format the format string
 * @param args the va_list
 * @return the number of characters printed
 * @note its purpose is to loop through the format string
 * @note and process each character, if it's a '%' character
 * @note it calls the flag parser and the conversion parser if
 * @note the current character is recognized as a conversion specifier
 * @note otherwise, it writes the character to the standard output
 * @note this ways we ensure to wrap up all use case, also if
 * @note we're looking for to print a simple string without any flags
 */
int	ft_process_format(const char *format, va_list args)
{
	int		i;
	int		len;
	t_flags	flags;

	i = 0;
	len = 0;
	while (format[i])
	{
		if (format[i] == '%' && format[i + 1])
		{
			flags = ft_flags_init();
			i++;
			i = ft_parse_flags(format, i, &flags, args);
			if (ft_istype(format[i]))
				len += ft_process_conversion(format[i], flags, args);
		}
		else
			len += ft_print_c(format[i]);
		i++;
	}
	return (len);
}
