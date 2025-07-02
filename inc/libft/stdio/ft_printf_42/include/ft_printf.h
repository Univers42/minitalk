/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 00:48:53 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/06/04 14:09:40 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include "ft_types.h"
# include "../../../libft.h"
# include "ft_converters.h"
# include "structs.h"
/**
 * ════════════════════════════════════════════════════════════════════════════
 * ║                              CORE FUNCTIONS                              ║
 * ════════════════════════════════════════════════════════════════════════════
 */

void	ft_init_tables(void);
int		ft_printf(const char *format, ...);
int		ft_process_format(const char *format, va_list args);
t_flags	ft_flags_init(void);

/**
 * ═══════════════════════════════════════════════════════════════════════════
 * ║                Format handlers - Each handles one format specifier      ║
 * ═══════════════════════════════════════════════════════════════════════════
 */

int		ft_print_char(int c, t_flags flags);
int		ft_print_str(const char *str, t_flags flags);
int		ft_print_ptr(unsigned long n, t_flags flags);
int		ft_print_int(int n, t_flags flags);
int		ft_print_unsigned(unsigned int n, t_flags flags);
int		ft_print_hex_format(unsigned int num, int uppercase, t_flags flags);
int		ft_print_percent(t_flags flags);

/**
 * ════════════════════════════════════════════════════════════════════════════
 * ║                           FLAG HANDLING FUNCTIONS                        ║ 
 * ════════════════════════════════════════════════════════════════════════════
 */

void	ft_handle_width(int width, t_flags *flags);
int		ft_istype(int c);
void	set_flag_bit(int flag_char, t_flags *flags);
void	handle_minus(t_flags *flags, va_list args);
void	handle_zero(t_flags *flags, va_list args);
void	handle_dot(t_flags *flags, va_list args);
void	handle_hash(t_flags *flags, va_list args);
void	handle_plus(t_flags *flags, va_list args);
void	handle_space(t_flags *flags, va_list args);
void	handle_star(t_flags *flags, va_list args);

/**
 * ════════════════════════════════════════════════════════════════════════════
 * ║                        WIDTH HANDLING FUNCTIONS                          ║
 * ════════════════════════════════════════════════════════════════════════════
 */

int		ft_print_width(int width, int content_len, int zero);
int		print_aligned_str(const char *str, int len, t_flags flags);
int		ft_print_hex_zero_regular(t_flags flags);
int		ft_print_hex_zero_with_dot(unsigned int num, t_flags flags, int upper);
int		ft_print_hex_left(unsigned int num, int hex_len,
			int prefix_len, t_flags flags);
int		ft_print_hex_right(unsigned int num, int hex_len,
			int prefix_len, t_flags flags);
int		ft_print_null_ptr(t_flags flags);
int		ft_print_null_ptr_left(t_flags flags);
int		ft_print_null_ptr_right(t_flags flags);
int		ft_print_ptr_left(unsigned long n, t_flags flags, int ptr_len);
int		ft_print_ptr_right(unsigned long n, t_flags flags,
			int ptr_len, int hex_len);
int		ft_print_ptr_right_zero(unsigned long n, t_flags flags, int hex_len);
int		ft_print_ptr_right_std(unsigned long n, t_flags flags, int ptr_len);
int		ft_print_int_ralign_std(int n, t_flags fl, unsigned int un, int len);
int		ft_print_int_ralign_zero(int n, t_flags fl, unsigned int un, int len);
int		ft_print_int_lalign(int n, t_flags fl, unsigned int un, int len);
int		ft_print_int_ralign(int n, t_flags fl, unsigned int un, int len);

/**
 * ════════════════════════════════════════════════════════════════════════════
 * ║                      PRECISION HANDLING FUNCTIONS                        ║
 * ════════════════════════════════════════════════════════════════════════════
 */

int		ft_print_strprec(const char *str, int precision);
void	get_print_info(const char *str, t_flags flags,
			const char **print_str, int *print_len);
int		ft_get_unsigned_len(unsigned int n, t_flags flags);
int		ft_handle_zero_precision(int n, t_flags flags);
int		ft_print_unsigned_precision(unsigned int n, int precision);
int		ft_print_hex_precision(unsigned int num, int precision, int uppercase);
int		ft_print_ptr_hex_precision(t_size n, int precision);
int		ft_print_hex_value(t_size n, int uppercase);
int		ft_print_ptr_hex(t_size n);

/**
 * ════════════════════════════════════════════════════════════════════════════
 * ║                              UTILITY FUNCTIONS                           ║
 * ════════════════════════════════════════════════════════════════════════════
 */

t_ssize	ft_print_hex_prefix(int uppercase);
int		ft_print_c(char c);
int		ft_print_sign(int n, t_flags flags);
int		ft_print_unsigned_nbr(unsigned int n);
int		ft_calc_sign_len(int n, t_flags flags);
int		ft_nbr_len(t_size n, int base);
int		ft_handle_invalid_format(char type, t_flags flags);

#endif
