/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stddef.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:37:45 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/04 18:31:15 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STDDEF_H
# define FT_STDDEF_H

typedef long				t_ssize;
typedef unsigned long int	t_size;

# define STDOUT_FILENO 1
# define STDERR_FILENO 2
# define STDIN_FILENO 0
//# define INT_MAX 2147483647
//# define INT_MIN (-2147483648)
//# define NULL ((void *)0)
# define TRUE 1
# define FALSE 0
# define SUCCESS 0
# define FAILURE 1
# define ERROR 1
# define OK 0

#endif