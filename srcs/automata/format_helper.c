/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 01:40:00 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 03:00:04 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "auto_parser.h"

static void	reverse_string(char *str, int len)
{
	int		start;
	int		end;
	char	temp;

	start = 0;
	end = len - 1;
	while (start < end)
	{
		temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		start++;
		end--;
	}
}

int	convert_to_string(long num, char *temp, int base, int uppercase)
{
	int		temp_pos;
	char	*digits;

	if (uppercase)
		digits = "0123456789ABCDEF";
	else
		digits = "0123456789abcdef";
	temp_pos = 0;
	if (num == 0)
		temp[temp_pos++] = '0';
	else
	{
		while (num > 0)
		{
			temp[temp_pos++] = digits[num % base];
			num /= base;
		}
	}
	return (temp_pos);
}

static void	add_sign_prefix(char *temp, int *temp_pos, t_format_spec *spec,
		int is_negative)
{
	if (is_negative)
		temp[(*temp_pos)++] = '-';
	else if (spec->show_sign)
		temp[(*temp_pos)++] = '+';
	else if (spec->space_prefix)
		temp[(*temp_pos)++] = ' ';
}

static void	format_number(t_buffer buffer, int *pos, long long num,
		t_format_spec *spec, int base)
{
	char	temp[64];
	int		temp_pos;
	int		is_negative;
	int		uppercase;

	uppercase = (spec->specifier == 'X');
	is_negative = 0;
	if (num < 0 && (spec->specifier == 'd' || spec->specifier == 'i'))
	{
		is_negative = 1;
		num = -num;
	}
	temp_pos = convert_to_string(num, temp, base, uppercase);
	add_sign_prefix(temp, &temp_pos, spec, is_negative);
	reverse_string(temp, temp_pos);
	temp[temp_pos] = '\0';
	buffer_append_str(buffer, pos, temp);
}

void	buffer_append_int_formatted(t_buffer buffer, int *pos,
		long long num, t_format_spec *spec)
{
	int	base;

	base = 10;
	if (spec->specifier == 'x' || spec->specifier == 'X'
		|| spec->specifier == 'p')
		base = 16;
	else if (spec->specifier == 'o')
		base = 8;
	format_number(buffer, pos, num, spec, base);
}
