/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bits.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 02:11:03 by codespace         #+#    #+#             */
/*   Updated: 2025/07/15 04:03:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

/**
 * @brief Sends a signal to a server process with error handling and logging
 * 
 * This function performs two critical checks before sending a signal:
 * 1. Verifies the target process exists using kill(pid, 0)
 * 2. Sends the actual signal and handles any transmission errors
 * 
 * Mathematical Logic:
 * - Signal mapping: SIGUSR1 represents binary 0, SIGUSR2 represents binary 1
 * - Process validation: kill(pid, 0) returns 0 if process exists, -1 if not
 * - Signal transmission: kill(pid, signal) returns 0 on success, -1 on failure
 * 
 * @param pid The process ID of the target server
 * @param signal The signal to send (SIGUSR1 or SIGUSR2)
 * 
 * Example:
 * - send_signal(1234, SIGUSR1) → sends "0" bit to process 1234
 * - send_signal(1234, SIGUSR2) → sends "1" bit to process 1234
 */
void	send_signal(pid_t pid, int signal)
{
	const char	*signal_name;

	if (signal == SIGUSR1)
		signal_name = "SIGUSR1 (0)";
	else
		signal_name = "SIGUSR2 (1)";
	if (kill(pid, 0) == -1)
	{
		ft_printf("Error: Server process PID %d no longer exists\n", pid);
		log_msg(LOG_ERROR, "Server process PID %d disappeared", pid);
		exit(EXIT_FAILURE);
	}
	if (kill(pid, signal) == -1)
	{
		ft_printf("Error: Signal sending failed\n");
		log_msg(LOG_ERROR, "Failed to send signal %d to PID %d", signal, pid);
		exit(EXIT_FAILURE);
	}
	log_msg(LOG_DEBUG, "Sent signal %s to server PID %d", signal_name, pid);
}

/**
 * @brief Converts a bit value to the appropriate signal and sends it
 * 
 * Mathematical Logic:
 * - Binary encoding: 0 → CHAR_0 (SIGUSR1), 1 → CHAR_1 (SIGUSR2)
 * - Boolean logic: if(bit_value) evaluates to true for any non-zero value
 * 
 * Truth Table:
 * | bit_value | Signal Sent | Binary Representation |
 * |-----------|-------------|----------------------|
 * | 0         | CHAR_0      | 0                    |
 * | 1         | CHAR_1      | 1                    |
 * | >1        | CHAR_1      | 1                    |
 * 
 * @param bit_value The bit value to send (0 or 1)
 * @param info Client information structure containing server PID
 * 
 * Example:
 * - send_data_bit(0, info) → sends CHAR_0 (represents binary 0)
 * - send_data_bit(1, info) → sends CHAR_1 (represents binary 1)
 */
void	send_data_bit(int bit_value, t_client *info)
{
	if (bit_value)
		send_signal(info->server_pid, CHAR_1);
	else
		send_signal(info->server_pid, CHAR_0);
}

/**
 * @brief Extracts and sends a specific bit from a value using
 * bitwise operations
 * 
 * Mathematical Logic - Bit Extraction:
 * - Uses bitwise AND (&) with a bit mask to extract specific bits
 * - Bit mask: (1ULL << i) creates a mask with only bit i set to 1
 * - Left shift (<<): moves 1 to position i
 * - Bitwise AND: isolates the bit at position i
 * 
 * Mathematical Examples:
 * 
 * Example 1: Extract bit 2 from value 13 (binary: 1101)
 * - value = 13 = 1101₂
 * - i = 2
 * - mask = (1ULL << 2) = 4 = 0100₂
 * - result = 13 & 4 = 1101₂ & 0100₂ = 0100₂ = 4 (non-zero → bit = 1)
 * 
 * Example 2: Extract bit 1 from value 13 (binary: 1101)
 * - value = 13 = 1101₂
 * - i = 1
 * - mask = (1ULL << 1) = 2 = 0010₂
 * - result = 13 & 2 = 1101₂ & 0010₂ = 0000₂ = 0 (zero → bit = 0)
 * 
 * Bit Position Mapping (for 8-bit value):
 * Position: 7 6 5 4 3 2 1 0
 * Value 13: 0 0 0 0 1 1 0 1
 * 
 * @param value The value containing the bit to extract
 * @param i The bit position (0 is rightmost/least significant bit)
 * @param info Client information structure
 * 
 * Example Usage:
 * - send_bit(13, 2, info) → extracts bit 2 from 13, sends 1
 * - send_bit(13, 1, info) → extracts bit 1 from 13, sends 0
 */
void	send_bit(unsigned long long value, int i, t_client *info)
{
	int	bit_value;

	bit_value = 0;
	if (value &(1ULL << i))
		bit_value = 1;
	log_msg(LOG_DEBUG, "Sending bit %d: %d to server PID %d", i, bit_value,
		info->server_pid);
	send_data_bit(bit_value, info);
	wait_for_server_ack();
}

/**
 * @brief Sends data by transmitting each bit sequentially
 * from MSB to LSB
 * 
 * Mathematical Logic - Data Transmission:
 * - Supports 8-bit (char) and 32-bit (int) data types
 * - Transmits bits from Most Significant Bit (MSB) to
 * Least Significant Bit (LSB)
 * - Uses type casting to interpret raw data as specific integer types
 * 
 * Mathematical Examples:
 * 
 * Example 1: Send 8-bit value 'A' (ASCII 65)
 * - data = 'A' = 65₁₀ = 01000001₂
 * - bit_length = 8
 * - Transmission order: bit 7→6→5→4→3→2→1→0
 * - Sent sequence: 0,1,0,0,0,0,0,1
 * 
 * Example 2: Send 32-bit value 1000
 * - data = 1000₁₀ = 1111101000₂ = 00000000000000000000001111101000₂
 * - bit_length = 32
 * - Transmission order: bit 31→30→...→2→1→0
 * - First bits sent: 
 * 		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,1,0,0,0
 * 
 * Type Casting Logic:
 * - (unsigned char *)data: interprets data as pointer to unsigned char
 * - *((unsigned char *)data): dereferences to get 8-bit value
 * - (unsigned int *)data: interprets data as pointer to unsigned int
 * - *((unsigned int *)data): dereferences to get 32-bit value
 * 
 * Loop Logic:
 * - i starts at (bit_length - 1) → MSB position
 * - i decrements to 0 → LSB position
 * - Ensures big-endian transmission (MSB first)
 * 
 * @param data Pointer to the data to be transmitted
 * @param bit_length Number of bits to transmit (8 or 32)
 * @param info Client information structure
 * 
 * Example Usage:
 * - char c = 'A'; send_signals(&c, 8, info) → sends 8 bits of 'A'
 * - int num = 1000; send_signals(&num, 32, info) → sends 32 bits of 1000
 */
void	send_signals(void *data, size_t bit_length, t_client *info)
{
	unsigned long long	value;
	int					i;

	value = 0;
	if (bit_length == 8)
		value = *((unsigned char *)data);
	else if (bit_length == 32)
		value = *((unsigned int *)data);
	log_msg(LOG_INFO, "Sending %zu-bit value: %llu (0x%llx) to server PID %d",
		bit_length,
		value, value, info->server_pid);
	i = bit_length - 1;
	while (i >= 0)
	{
		send_bit(value, i, info);
		i--;
	}
	log_msg(LOG_SUCCESS, "Successfully sent %zu bits", bit_length);
}
