/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_more.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 02:10:33 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 02:10:38 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "auto_parser.h"

t_format_state	action_flags(t_parser_ctx *ctx, char c)
{
	t_flag_action	action;

	if (ctx->tables->char_class_table[(unsigned char)c] & CHAR_FLAG)
	{
		action = ctx->tables->flag_table[(unsigned char)c];
		if (action)
			action(&ctx->spec);
		return (STATE_FLAGS);
	}
	return (action_percent(ctx, c));
}

t_format_state	action_width(t_parser_ctx *ctx, char c)
{
	if (ctx->tables->char_class_table[(unsigned char)c] & CHAR_DIGIT)
	{
		ctx->temp_num = ctx->temp_num * 10 + (c - '0');
		return (STATE_WIDTH);
	}
	ctx->spec.width = ctx->temp_num;
	if (ctx->tables->char_class_table[(unsigned char)c] & CHAR_DOT)
	{
		ctx->spec.has_precision = 1;
		ctx->temp_num = 0;
		return (STATE_PRECISION);
	}
	return (action_percent(ctx, c));
}

t_format_state	action_error(t_parser_ctx *ctx, char c)
{
	buffer_append_char(ctx->buffer, ctx->pos, '%');
	buffer_append_char(ctx->buffer, ctx->pos, c);
	return (STATE_NORMAL);
}
