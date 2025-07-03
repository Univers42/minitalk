/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 02:17:02 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 17:13:07 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

int	validate_pid_string(const char *str)
{
	int		i;
	pid_t	pid;

	if (!str || !str[0])
		return (0);
	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	pid = ft_atoi(str);
	if (pid <= 0)
		return (0);
	if (kill(pid, 0) == -1)
		return (0);
	return (1);
}

void	print_error_and_exit(t_parser_result result)
{
	if (result == PARSER_INVALID_ARGC)
	{
		ft_printf("Error: %s\n", USAGE);
		log_msg(LOG_ERROR, "Invalid number of arguments");
	}
	else if (result == PARSER_INVALID_PID)
	{
		ft_printf("Error: %s\n", BAD_SIGNAL);
		log_msg(LOG_ERROR, "PID validation failed");
	}
	else if (result == PARSER_EMPTY_MESSAGE)
	{
		ft_printf("Error: Message cannot be empty\n");
		log_msg(LOG_ERROR, "Empty message provided");
	}
	exit(EXIT_FAILURE);
}

t_parser_result	parse_arguments(int argc, char **argv)
{
	if (argc != 3)
		return (PARSER_INVALID_ARGC);
	if (!validate_pid_string(argv[1]))
		return (PARSER_INVALID_PID);
	if (!argv[2] || !argv[2][0])
		return (PARSER_EMPTY_MESSAGE);
	log_msg(LOG_SUCCESS, "Arguments validated successfully");
	return (PARSER_SUCCESS);
}
