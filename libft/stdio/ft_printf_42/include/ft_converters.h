/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_converters.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 15:09:32 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/06/04 14:09:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_CONVERTERS_H
# define FT_CONVERTERS_H

# include <stddef.h>
# include "ft_printf.h"

/* Define t_size for platform-independent type handling */
typedef unsigned long	t_size;

# define LEN_STR_NULL 6
# define HEX_DIGIT_UPPER "0123456789ABCDEF"
# define HEX_DIGIT_LOWER "0123456789abcdef"
# define BASE_HEX 16
# define BASE_DEC 10
/**
 * ════════════════════════════════════════════════════════════════════════════
 * ║FORWARD DECLARATIONS FOR CONVERSION FUNCTIONS                             ║
 * ════════════════════════════════════════════════════════════════════════════
 */

int		ft_convert_percent(char type, t_flags flags, va_list args);
int		ft_convert_char(char type, t_flags flags, va_list args);
int		ft_convert_str(char type, t_flags flags, va_list args);
int		ft_convert_int(char type, t_flags flags, va_list args);
int		ft_convert_unsigned(char type, t_flags flags, va_list args);
int		ft_convert_hex_lower(char type, t_flags flags, va_list args);
int		ft_convert_hex_upper(char type, t_flags flags, va_list args);
int		ft_convert_ptr(char type, t_flags flags, va_list args);

/**
 * ════════════════════════════════════════════════════════════════════════════
 * ║FORWARD DECLARATIONS FOR FLAG HANDLERS                                    ║
 * ════════════════════════════════════════════════════════════════════════════
 */

void	handle_minus(t_flags *flags, va_list args);
void	handle_zero(t_flags *flags, va_list args);
void	handle_dot(t_flags *flags, va_list args);
void	handle_hash(t_flags *flags, va_list args);
void	handle_space(t_flags *flags, va_list args);
void	handle_plus(t_flags *flags, va_list args);
void	handle_star(t_flags *flags, va_list args);

#endif
