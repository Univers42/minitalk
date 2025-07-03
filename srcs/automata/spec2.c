/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:43:42 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 03:03:24 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "auto_parser.h"
#include <stdarg.h>

void	spec_hex_upper(t_parser_ctx *ctx)
{
	unsigned int	num;

	num = va_arg(*ctx->args, unsigned int);
	ctx->spec.specifier = 'X';
	buffer_append_int_formatted(ctx->buffer, ctx->pos, num, &ctx->spec);
}

void	spec_octal(t_parser_ctx *ctx)
{
	unsigned int	num;

	num = va_arg(*ctx->args, unsigned int);
	ctx->spec.specifier = 'o';
	buffer_append_int_formatted(ctx->buffer, ctx->pos, num, &ctx->spec);
}

void	spec_char(t_parser_ctx *ctx)
{
	char	ch;

	ch = (char)va_arg(*ctx->args, int);
	buffer_append_char(ctx->buffer, ctx->pos, ch);
}

void	spec_float(t_parser_ctx *ctx)
{
	double	num;
	int		int_part;

	num = va_arg(*ctx->args, double);
	int_part = (int)num;
	ctx->spec.specifier = 'f';
	buffer_append_int_formatted(ctx->buffer, ctx->pos, int_part, &ctx->spec);
}

void	spec_pointer(t_parser_ctx *ctx)
{
	void			*ptr;
	unsigned long	addr;

	ptr = va_arg(*ctx->args, void *);
	addr = (unsigned long)ptr;
	buffer_append_str(ctx->buffer, ctx->pos, "0x");
	ctx->spec.specifier = 'p';
	buffer_append_int_formatted(ctx->buffer, ctx->pos, addr, &ctx->spec);
}
