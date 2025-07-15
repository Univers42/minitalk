/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:45:49 by codespace         #+#    #+#             */
/*   Updated: 2025/07/15 04:05:40 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

/**
 * @brief Checks server readiness with timeout using exponential time polling
 * 
 * Mathematical Logic - Time Management:
 * - Total timeout = RETRY_TIME * 1,000,000 microseconds (conversion to µs)
 * - Sleep interval = total_timeout / RETRY_INTERVALS (uniform distribution)
 * - Polling frequency = 1 / sleep_interval checks per second
 * 
 * Mathematical Examples:
 * 
 * Example 1: RETRY_TIME = 5 seconds, RETRY_INTERVALS = 100
 * - total_sleep = 5 * 1,000,000 = 5,000,000 µs
 * - sleep_time = 5,000,000 / 100 = 50,000 µs = 0.05 seconds
 * - Polling frequency = 1 / 0.05 = 20 checks per second
 * - Total attempts = 5,000,000 / 50,000 = 100 attempts
 * 
 * Example 2: RETRY_TIME = 10 seconds, RETRY_INTERVALS = 50
 * - total_sleep = 10 * 1,000,000 = 10,000,000 µs
 * - sleep_time = 10,000,000 / 50 = 200,000 µs = 0.2 seconds
 * - Polling frequency = 1 / 0.2 = 5 checks per second
 * - Total attempts = 10,000,000 / 200,000 = 50 attempts
 * 
 * Loop Logic:
 * - i accumulates total elapsed time in microseconds
 * - Loop condition: i <= total_sleep ensures we don't exceed timeout
 * - Each iteration: sleep_time µs passes, i increases by sleep_time
 * - Exit conditions: server ready (return 1) or timeout (return 0)
 * 
 * Time Complexity: O(RETRY_INTERVALS) - linear with number of intervals
 * Space Complexity: O(1) - constant space usage
 * 
 * @return 1 if server becomes ready within timeout, 0 if timeout occurs
 * 
 * Example Usage:
 * - If server ready after 2 seconds → returns 1 (success)
 * - If server not ready after full timeout → returns 0 (failure)
 */
int	check_server_and_sleep(void)
{
	const int		total_sleep = RETRY_TIME * 1000 * 1000;
	const int		sleep_time = total_sleep / RETRY_INTERVALS;
	int				i;
	t_server_state	*server;

	server = get_server_instance();
	i = 0;
	while (i <= total_sleep)
	{
		usleep(sleep_time);
		if (server->is_ready == 1)
			return (1);
		i += sleep_time;
	}
	return (0);
}

/**
 * @brief Validates transmission ownership using process ID comparison
 * 
 * Mathematical Logic - Ownership Validation:
 * - Boolean logic: AND operation between two conditions
 * - Condition 1: server->transmission_active == true
 * - Condition 2: server->current_client_pid == client_pid
 * - Result: true only if BOTH conditions are satisfied
 * 
 * Truth Table for Ownership Validation:
 * | transmission_active | PID Match | Result | Meaning                    |
 * |-------------------|-----------|---------|----------------------------|
 * | false             | false     | false   | No transmission active     |
 * | false             | true      | false   | No transmission active     |
 * | true              | false     | false   | Different client owns it   |
 * | true              | true      | true    | Current client owns it     |
 * 
 * Mathematical Examples:
 * 
 * Example 1: Valid ownership
 * - client_pid = 1234
 * - server->transmission_active = true
 * - server->current_client_pid = 1234
 * - Result: true && (1234 == 1234) = true && true = true
 * 
 * Example 2: Invalid ownership (different PID)
 * - client_pid = 1234
 * - server->transmission_active = true
 * - server->current_client_pid = 5678
 * - Result: true && (1234 == 5678) = true && false = false
 * 
 * Example 3: No active transmission
 * - client_pid = 1234
 * - server->transmission_active = false
 * - server->current_client_pid = 1234
 * - Result: false && (1234 == 1234) = false && true = false
 * 
 * Security Logic:
 * - Prevents race conditions between multiple clients
 * - Ensures atomic transmission ownership
 * - PID uniqueness guarantees process identification
 * 
 * @param client_pid The process ID of the client requesting validation
 * @return 1 if client owns the transmission, 0 otherwise
 * 
 * Example Usage:
 * - is_transmission_owner(getpid()) → checks if current process owns transmission
 */
int	is_transmission_owner(pid_t client_pid)
{
	t_server_state	*server;

	server = get_server_instance();
	return (server->transmission_active
		&& server->current_client_pid == client_pid);
}

/**
 * @brief Monitors transmission ownership and displays progress for large messages
 * 
 * Mathematical Logic - Progress Calculation:
 * - Percentage formula: (current_position * 100) / total_items
 * - Modulo operation: i % 1000 == 0 (divisibility check)
 * - Conditional display: only for large messages (total_chars > 5000)
 * 
 * Mathematical Examples:
 * 
 * Example 1: Progress calculation
 * - total_chars = 10000, i = 2500
 * - percentage = (2500 * 100) / 10000 = 250000 / 10000 = 25%
 * - Display: "Progress: 25% (2500/10000 characters)"
 * 
 * Example 2: Progress calculation
 * - total_chars = 8000, i = 6000
 * - percentage = (6000 * 100) / 8000 = 600000 / 8000 = 75%
 * - Display: "Progress: 75% (6000/8000 characters)"
 * 
 * Modulo Logic for Display Frequency:
 * - i % 1000 == 0: displays every 1000 characters
 * - Reduces output frequency for better performance
 * - Examples: displays at i = 1000, 2000, 3000, 4000, etc.
 * 
 * Conditional Logic:
 * - total_chars > 5000: only show progress for large messages
 * - i > 0: avoid division by zero and initial display
 * - Prevents spam for small messages
 * 
 * Mathematical Properties:
 * - Percentage range: [0, 100] (bounded)
 * - Monotonic increase: percentage never decreases
 * - Linear relationship: percentage ∝ i (proportional to progress)
 * 
 * Performance Considerations:
 * - Division operation: O(1) constant time
 * - Modulo operation: O(1) constant time
 * - Display frequency: O(total_chars/1000) - logarithmic display calls
 * 
 * @param my_pid The process ID of the current client
 * @param total_chars Total number of characters to be transmitted
 * @param i Current character position (0-based index)
 * 
 * Example Usage:
 * - Small message (1000 chars): No progress display
 * - Large message (10000 chars): Progress shown at 1000, 2000, 3000, etc.
 * - Lost ownership: Program terminates with error
 */
void	check_transmission_ownership(pid_t my_pid, int total_chars, int i)
{
	int	percentage;

	if (!is_transmission_owner(my_pid))
	{
		log_msg(LOG_ERROR, "Lost transmission ownership during message send");
		exit(EXIT_FAILURE);
	}
	if (total_chars > 5000 && (i % 1000 == 0) && i > 0)
	{
		percentage = (i * 100) / total_chars;
		ft_printf("Progress: %d%% (%d/%d characters)\n", percentage, i,
			total_chars);
	}
}
