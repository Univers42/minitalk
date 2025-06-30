/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_types.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 21:00:00 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/06/04 14:09:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TYPES_H
# define FT_TYPES_H

/**
 *  Flag bit positions
 *  LEFT_FLAG: '-' flag
 * ZERO_FLAG: '0' flag
 * STAR_FLAG: '*' flag
 * DOT_FLAG: '.' flag
 * HASH_FLAG: '#' flag
 * SPACE_FLAG: ' ' flag
 * PLUS_FLAG: '+' flag
 * Additional flags can be added here
 * ### represntation HEX: 
 * 0000 0001  LEFT_FLAG
 * 0000 0010  ZERO_FLAG
 * 0000 0100  STAR_FLAG
 * 0000 1000  DOT_FLAG
 * 0001 0000  HASH_FLAG
 * 0010 0000  SPACE_FLAG
 * 0100 0000  PLUS_FLAG
 * ### Example:
 * 		0000 0001
 * & 	0000 0010
 * 	   ------------
 * 		0000 0000 
 * return(3) => which means 0 flags are set
 *	 0001 0000
 * | 0000 1000
 *  ------------
 * 0001 1000 => which means 2 flags are set
 * if flags_bit == 1
 * 		0000 0001
 * |	0000 0010
 * 	   -----------
 * 		0000 0011
 */

# define LEFT_FLAG    0x001
# define ZERO_FLAG    0x002
# define STAR_FLAG    0x004 
# define DOT_FLAG     0x008
# define HASH_FLAG    0x010 
# define SPACE_FLAG   0x020 
# define PLUS_FLAG    0x040
/* Additional flags can be added here */

/**
 * ════════════════════════════════════════════════════════════════════════════
 * ║                FLAG STRUCTURE TO STORE FORMAT OPTIONS                    ║
 * ════════════════════════════════════════════════════════════════════════════
 */

/** All flags are stored in a single int for O(1) operations
 *  Optimized structure using bit flags 
*/
typedef struct s_flags
{
	unsigned int	flags_bits;
	char			spec;
	int				width;
	int				precision;
}					t_flags;

/**
 * Helper structure to handle formatted output length calculations
 * content_len: Length of the main content
 * prefix_len: Length of any prefix characters
 * prefix_char: Single prefix char (like sign)
 * prefix_str: Multi-char prefix (like "0x")
 * prefix_str_len: Length of prefix_str
 */
typedef struct s_format_len
{
	int		content_len;
	int		prefix_len;
	char	prefix_char;
	char	*prefix_str;
	int		prefix_str_len;
}	t_format_len;

#endif
