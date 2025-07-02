/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 16:36:17 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/02 18:34:42 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

/* Constants for bit transmission */
#define BYTE_SIZE 8
#define BIT_DELAY_US 100
#define ACK_TIMEOUT_US 10000
#define MAX_RETRIES 3

/* Signal definitions for bit transmission */
#define SIGNAL_BIT_ONE SIGUSR1
#define SIGNAL_BIT_ZERO SIGUSR2
#define SIGNAL_END_TRANSMISSION SIGTERM

/* Transmission result codes */
typedef enum e_transmission_result
{
	TRANS_SUCCESS = 0,
	TRANS_SIGNAL_FAILED = -1,
	TRANS_ACK_TIMEOUT = -2,
	TRANS_INVALID_PARAMS = -3,
	TRANS_SERVER_DEAD = -4
}	t_transmission_result;

/* ========== UTILITY FUNCTIONS ========== */

static int	is_valid_transmission_params(pid_t pid, int signal)
{
	return (pid > 0 && signal > 0 && signal < NSIG);
}

static int	wait_for_acknowledgment(t_client *client, int timeout_us)
{
	int	elapsed_us;

	if (!client)
		return (TRANS_INVALID_PARAMS);
	elapsed_us = 0;
	while (elapsed_us < timeout_us && !client->ack_received)
	{
		usleep(BIT_DELAY_US);
		elapsed_us += BIT_DELAY_US;
	}
	if (!client->ack_received)
		return (TRANS_ACK_TIMEOUT);
	client->ack_received = 0;
	return (TRANS_SUCCESS);
}

/* ========== BIT TRANSMISSION ========== */

static int	send_signal_to_server(pid_t pid, int signal)
{
	if (!is_valid_transmission_params(pid, signal))
		return (TRANS_INVALID_PARAMS);
	if (!is_process_alive(pid))
	{
		LOG_CLIENT_ERROR("Server process is no longer alive");
		return (TRANS_SERVER_DEAD);
	}
	if (kill(pid, signal) == -1)
	{
		if (errno == ESRCH)
			return (TRANS_SERVER_DEAD);
		LOG_CLIENT_ERROR("Failed to send signal to server");
		return (TRANS_SIGNAL_FAILED);
	}
	return (TRANS_SUCCESS);
}

static int	send_bit_with_retry(t_client *client, int bit_value)
{
	int	signal;
	int	result;
	int	retry_count;

	if (!client || !client_has_valid_pid(client))
		return (TRANS_INVALID_PARAMS);
	signal = (bit_value) ? SIGNAL_BIT_ONE : SIGNAL_BIT_ZERO;
	retry_count = 0;
	while (retry_count < MAX_RETRIES)
	{
		result = send_signal_to_server(client->pid_server, signal);
		if (result != TRANS_SUCCESS)
			return (result);
		result = wait_for_acknowledgment(client, ACK_TIMEOUT_US);
		if (result == TRANS_SUCCESS)
			return (TRANS_SUCCESS);
		retry_count++;
		if (retry_count < MAX_RETRIES)
			usleep(BIT_DELAY_US * 2);
	}
	LOG_CLIENT_ERROR("Failed to send bit after maximum retries");
	return (TRANS_ACK_TIMEOUT);
}

/* ========== BYTE TRANSMISSION ========== */

static int	extract_bit_from_byte(unsigned char byte, int bit_position)
{
	if (bit_position < 0 || bit_position >= BYTE_SIZE)
		return (0);
	return ((byte >> bit_position) & 1);
}

static int	send_byte_bits(t_client *client, unsigned char byte)
{
	int	bit_position;
	int	bit_value;
	int	result;

	if (!client)
		return (TRANS_INVALID_PARAMS);
	bit_position = BYTE_SIZE - 1;
	while (bit_position >= 0)
	{
		bit_value = extract_bit_from_byte(byte, bit_position);
		result = send_bit_with_retry(client, bit_value);
		if (result != TRANS_SUCCESS)
			return (result);
		bit_position--;
	}
	return (TRANS_SUCCESS);
}

static int	send_single_byte(t_client *client, unsigned char byte)
{
	int	result;

	if (!client || !client_has_valid_pid(client))
		return (TRANS_INVALID_PARAMS);
	DEBUG_PRINT("Sending byte: 0x%02X ('%c')", byte, 
		(byte >= 32 && byte <= 126) ? byte : '?');
	result = send_byte_bits(client, byte);
	if (result != TRANS_SUCCESS)
	{
		LOG_CLIENT_ERROR("Failed to send byte");
		return (result);
	}
	return (TRANS_SUCCESS);
}

/* ========== MESSAGE TRANSMISSION ========== */

static int	send_message_content(t_client *client)
{
	const char	*current_byte;
	int			result;
	size_t		bytes_sent;

	if (!client || !client_has_valid_message(client))
		return (TRANS_INVALID_PARAMS);
	current_byte = client->msg;
	bytes_sent = 0;
	while (*current_byte)
	{
		result = send_single_byte(client, (unsigned char)*current_byte);
		if (result != TRANS_SUCCESS)
		{
			LOG_CLIENT_ERROR("Transmission failed after %zu bytes", 
				bytes_sent);
			return (result);
		}
		current_byte++;
		bytes_sent++;
		if (client->config.verbose && bytes_sent % 10 == 0)
			LOG_CLIENT_INFO("Sent %zu bytes", bytes_sent);
	}
	LOG_CLIENT_SUCCESS("Message content sent successfully (%zu bytes)", 
		bytes_sent);
	return (TRANS_SUCCESS);
}

static int	send_end_of_transmission(t_client *client)
{
	int	result;

	if (!client)
		return (TRANS_INVALID_PARAMS);
	DEBUG_PRINT("Sending end-of-transmission marker");
	result = send_single_byte(client, 0);
	if (result != TRANS_SUCCESS)
	{
		LOG_CLIENT_ERROR("Failed to send end-of-transmission marker");
		return (result);
	}
	LOG_CLIENT_SUCCESS("End-of-transmission marker sent");
	return (TRANS_SUCCESS);
}

/* ========== PUBLIC API ========== */

int	send_message_to_server(t_client *client)
{
	int	result;

	if (!client)
		return (TRANS_INVALID_PARAMS);
	if (!client_is_ready(client))
	{
		LOG_CLIENT_ERROR("Client is not ready for transmission");
		return (TRANS_INVALID_PARAMS);
	}
	LOG_CLIENT_INFO("Starting message transmission to PID %d", 
		client->pid_server);
	start_timer();
	result = send_message_content(client);
	if (result != TRANS_SUCCESS)
		return (result);
	result = send_end_of_transmission(client);
	if (result != TRANS_SUCCESS)
		return (result);
	LOG_CLIENT_SUCCESS("Message transmission completed in %ld ms", 
		get_elapsed_time_ms());
	return (TRANS_SUCCESS);
}

int	send_bit_by_bit(pid_t server_pid, const char *message)
{
	t_client	client;
	int			result;

	if (server_pid <= 0 || !message)
		return (TRANS_INVALID_PARAMS);
	ft_memset(&client, 0, sizeof(t_client));
	client.pid_server = server_pid;
	client.pid_client = getpid();
	client.msg = message;
	client.msg_len = ft_strlen(message);
	init_default_client_config(&client.config);
	result = send_message_to_server(&client);
	return (result);
}

/* ========== UTILITY AND DEBUG FUNCTIONS ========== */

size_t	calculate_transmission_time(const char *message)
{
	size_t	total_bits;
	size_t	estimated_time_ms;

	if (!message)
		return (0);
	total_bits = (ft_strlen(message) + 1) * BYTE_SIZE;
	estimated_time_ms = (total_bits * (BIT_DELAY_US + ACK_TIMEOUT_US)) / 1000;
	return (estimated_time_ms);
}

const char	*get_transmission_error_string(int error_code)
{
	static const char	*error_messages[] = {
		"Transmission successful",
		"Signal transmission failed",
		"Acknowledgment timeout",
		"Invalid parameters",
		"Server process is dead"
	};

	if (error_code > 0 || error_code < -4)
		return ("Unknown transmission error");
	return (error_messages[-error_code]);
}

int	is_transmission_success(int result)
{
	return (result == TRANS_SUCCESS);
}

void	print_transmission_stats(const t_client *client)
{
	size_t	estimated_time;

	if (!client || !client_has_valid_message(client))
		return ;
	estimated_time = calculate_transmission_time(client->msg);
	ft_printf("=== Transmission Statistics ===\n");
	ft_printf("Message length: %zu bytes\n", client->msg_len);
	ft_printf("Total bits: %zu\n", client->msg_len * BYTE_SIZE);
	ft_printf("Estimated time: %zu ms\n", estimated_time);
	ft_printf("Server PID: %d\n", client->pid_server);
	ft_printf("===============================\n");
}