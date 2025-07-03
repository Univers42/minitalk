/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bufferization.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 19:32:46 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/03 01:33:01 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "auto_parser.h"
#include <string.h>

void	buffer_append_char(t_buffer buffer, int *pos, char c)
{
	if (*pos < 1023)
		buffer[(*pos)++] = c;
}

void	buffer_append_str(t_buffer buffer, int *pos, const char *str)
{
	while (*str && *pos < 1023)
		buffer[(*pos)++] = *str++;
}

static void	append_padding(t_buffer buffer, int *pos, int padding)
{
	int	i;

	i = -1;
	while (++i < padding && *pos < 1023)
		buffer[(*pos)++] = ' ';
}

static void	append_chars(t_buffer buffer, int *pos, const char *str, int max)
{
	int	i;

	i = -1;
	while (++i < max && *pos < 1023)
		buffer[(*pos)++] = str[i];
}

void	buffer_append_str_formatted(t_buffer buffer, int *pos,
	const char *str, t_format_spec *spec)
{
	int	len;
	int	padding;
	int	max_chars;

	len = 0;
	if (str)
		len = strlen(str);
	max_chars = len;
	if (spec->has_precision && spec->precision < len)
		max_chars = spec->precision;
	padding = 0;
	if (spec->width > max_chars)
		padding = spec->width - max_chars;
	if (!spec->left_align && padding > 0)
		append_padding(buffer, pos, padding);
	append_chars(buffer, pos, str, max_chars);
	if (spec->left_align && padding > 0)
		append_padding(buffer, pos, padding);
}
