/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 02:39:45 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 03:20:07 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "auto_parser.h"

// Helper utility functions for automata system

void	reset_format_spec(t_format_spec *spec)
{
	ft_memset(spec, 0, sizeof(t_format_spec));
}

int	is_valid_specifier(char c)
{
	t_parser_tables	*tables;

	tables = get_parser_tables();
	return (tables->char_class_table[(unsigned char)c] & CHAR_SPEC);
}

int	is_flag_char(char c)
{
	t_parser_tables	*tables;

	tables = get_parser_tables();
	return (tables->char_class_table[(unsigned char)c] & CHAR_FLAG);
}
