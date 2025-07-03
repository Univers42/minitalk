/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_state.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 13:27:36 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/03 00:53:08 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "auto_parser.h"
#include <string.h>

void	flag_left_align(t_format_spec *spec)
{
	spec->left_align = 1;
}

void	flag_show_sign(t_format_spec *spec)
{
	spec->show_sign = 1;
}

void	flag_space_prefix(t_format_spec *spec)
{
	spec->space_prefix = 1;
}

void	flag_alt_form(t_format_spec *spec)
{
	spec->alt_form = 1;
}

void	flag_zero_pad(t_format_spec *spec)
{
	spec->zero_pad = 1;
}
