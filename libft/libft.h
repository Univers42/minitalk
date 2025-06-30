/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:38:48 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/30 20:33:58 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

/* Feature test macros - define before any system includes */
# ifndef _POSIX_C_SOURCE
#  define _POSIX_C_SOURCE 200809L
# endif

# ifndef _DEFAULT_SOURCE
#  define _DEFAULT_SOURCE
# endif

# ifndef _BSD_SOURCE
#  define _BSD_SOURCE
# endif

# include "strings/ft_string.h"
# include "memory/ft_memory.h"
# include "ctype/ft_ctype.h"
# include "data_structures/data_structures.h"
# include "math/ft_math.h"
# include "stdlib/ft_stdlib.h"
# include "stdio/ft_stdio.h"
# include "render/ft_render.h"
# include "debug/ft_debug.h"
# include "render/ft_colors.h"
# include "render/ft_stddef.h"
# include "sort/ft_sort.h"
# include <unistd.h>
# include <signal.h>
# include <time.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <stdio.h>
# include <sys/stat.h>
# include <errno.h>

typedef char*				t_string;
typedef unsigned char		t_uint8;
typedef struct sigaction	t_sigaction;
// used for inter-thread or signal communication
typedef volatile int		t_sig_atomic;
typedef int*				t_array;
typedef int					t_pid;
// Good name for char** typedef:
typedef char**				t_strings;
typedef int					t_flag;
#endif