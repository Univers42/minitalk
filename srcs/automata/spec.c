/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 13:08:07 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/03 02:59:51 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "auto_parser.h"
#include <stdarg.h>

void	spec_string(t_parser_ctx *ctx)
{
	char	*str;

	str = va_arg(*ctx->args, char *);
	if (!str)
		str = "(null)";
	buffer_append_str_formatted(ctx->buffer, ctx->pos, str, &ctx->spec);
}

void	spec_decimal(t_parser_ctx *ctx)
{
	int	num;

	num = va_arg(*ctx->args, int);
	ctx->spec.specifier = 'd';
	buffer_append_int_formatted(ctx->buffer, ctx->pos, num, &ctx->spec);
}

void	spec_integer(t_parser_ctx *ctx)
{
	int	num;

	num = va_arg(*ctx->args, int);
	ctx->spec.specifier = 'i';
	buffer_append_int_formatted(ctx->buffer, ctx->pos, num, &ctx->spec);
}

void	spec_unsigned(t_parser_ctx *ctx)
{
	unsigned int	num;

	num = va_arg(*ctx->args, unsigned int);
	ctx->spec.specifier = 'u';
	buffer_append_int_formatted(ctx->buffer, ctx->pos, num, &ctx->spec);
}

void	spec_hex_lower(t_parser_ctx *ctx)
{
	unsigned int	num;

	num = va_arg(*ctx->args, unsigned int);
	ctx->spec.specifier = 'x';
	buffer_append_int_formatted(ctx->buffer, ctx->pos, num, &ctx->spec);
}
