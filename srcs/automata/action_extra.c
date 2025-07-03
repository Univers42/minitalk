/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_extra.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 01:50:00 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 02:05:11 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "auto_parser.h"

t_format_state	action_precision(t_parser_ctx *ctx, char c)
{
	if (ctx->tables->char_class_table[(unsigned char)c] & CHAR_DIGIT)
	{
		ctx->temp_num = ctx->temp_num * 10 + (c - '0');
		return (STATE_PRECISION);
	}
	ctx->spec.precision = ctx->temp_num;
	return (action_percent(ctx, c));
}

t_format_state	action_length(t_parser_ctx *ctx, char c)
{
	return (action_percent(ctx, c));
}

t_format_state	action_specifier(t_parser_ctx *ctx, char c)
{
	t_spec_action	action;

	(void)c;
	action = ctx->tables->spec_table[(unsigned char)ctx->spec.specifier];
	if (action)
		action(ctx);
	else
	{
		buffer_append_char(ctx->buffer, ctx->pos, '%');
		buffer_append_char(ctx->buffer, ctx->pos, ctx->spec.specifier);
	}
	return (STATE_NORMAL);
}
