/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:52:33 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 03:18:32 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "auto_parser.h"

t_format_state	action_normal(t_parser_ctx *ctx, char c)
{
	if (c == '%')
	{
		ft_memset(&ctx->spec, 0, sizeof(t_format_spec));
		return (STATE_PERCENT);
	}
	buffer_append_char(ctx->buffer, ctx->pos, c);
	return (STATE_NORMAL);
}

static t_format_state	handle_percent_char(t_parser_ctx *ctx)
{
	buffer_append_char(ctx->buffer, ctx->pos, '%');
	return (STATE_NORMAL);
}

static t_format_state	handle_flag_char(t_parser_ctx *ctx, char c)
{
	t_flag_action	action;

	action = ctx->tables->flag_table[(unsigned char)c];
	if (action)
		action(&ctx->spec);
	return (STATE_FLAGS);
}

static t_format_state	handle_other_chars(t_parser_ctx *ctx, char c)
{
	if (ctx->tables->char_class_table[(unsigned char)c] & CHAR_DIGIT)
	{
		ctx->temp_num = c - '0';
		return (STATE_WIDTH);
	}
	if (ctx->tables->char_class_table[(unsigned char)c] & CHAR_DOT)
	{
		ctx->spec.has_precision = 1;
		ctx->temp_num = 0;
		return (STATE_PRECISION);
	}
	if (ctx->tables->char_class_table[(unsigned char)c] & CHAR_LENGTH)
	{
		ctx->spec.length_mod = c;
		return (STATE_LENGTH);
	}
	if (ctx->tables->char_class_table[(unsigned char)c] & CHAR_SPEC)
	{
		ctx->spec.specifier = c;
		return (STATE_SPECIFIER);
	}
	return (STATE_ERROR);
}

t_format_state	action_percent(t_parser_ctx *ctx, char c)
{
	if (ctx->tables->char_class_table[(unsigned char)c] & CHAR_PERCENT)
		return (handle_percent_char(ctx));
	if (ctx->tables->char_class_table[(unsigned char)c] & CHAR_FLAG)
		return (handle_flag_char(ctx, c));
	return (handle_other_chars(ctx, c));
}
