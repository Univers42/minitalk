# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: codespace <codespace@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/02 19:00:00 by dlesieur          #+#    #+#              #
#    Updated: 2025/07/03 08:02:17 by codespace        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME_SERVER = server
NAME_CLIENT = client

FLAG_DEBUG = 1
CC = cc
CFLAGS = -Wall -Wextra -Werror -I./inc -I./inc/libft
CFLAGS += -DMINITALK_DEBUG=$(FLAG_DEBUG)
RM = rm -f

# Directories
LIBFT_DIR = inc/libft
SERVER_DIR = srcs/server
CLIENT_DIR = srcs/client
AUTOMATA_DIR = srcs/automata

# Source files
SERVER_SRCS = $(SERVER_DIR)/main.c $(SERVER_DIR)/server.c $(SERVER_DIR)/signals.c $(SERVER_DIR)/server_loop.c $(SERVER_DIR)/server_message.c $(SERVER_DIR)/server_cleanup.c

CLIENT_SRCS = $(CLIENT_DIR)/main.c $(CLIENT_DIR)/client.c $(CLIENT_DIR)/ping.c $(CLIENT_DIR)/parser.c $(CLIENT_DIR)/singleton.c $(CLIENT_DIR)/ping_utils.c

AUTOMATA_SRCS = $(AUTOMATA_DIR)/singleton.c $(AUTOMATA_DIR)/log.c $(AUTOMATA_DIR)/format_state.c \
				$(AUTOMATA_DIR)/spec.c $(AUTOMATA_DIR)/conversion.c $(AUTOMATA_DIR)/bufferization.c \
				$(AUTOMATA_DIR)/verify.c $(AUTOMATA_DIR)/format_helper.c $(AUTOMATA_DIR)/action.c \
				$(AUTOMATA_DIR)/action_extra.c $(AUTOMATA_DIR)/char_table.c  $(AUTOMATA_DIR)/utils.c \
				$(AUTOMATA_DIR)/spec2.c $(AUTOMATA_DIR)/singleton_helper.c $(AUTOMATA_DIR)/action_more.c

# Object files
SERVER_OBJS = $(SERVER_SRCS:.c=.o) $(AUTOMATA_SRCS:.c=.o)
CLIENT_OBJS = $(CLIENT_SRCS:.c=.o) $(AUTOMATA_SRCS:.c=.o)

# Default rule
all: $(NAME_SERVER) $(NAME_CLIENT)

# Build libft
$(LIBFT_DIR)/libft.a:
	$(MAKE) -C $(LIBFT_DIR)

# Build server
$(NAME_SERVER): $(SERVER_OBJS) $(LIBFT_DIR)/libft.a
	$(CC) $(CFLAGS) -o $@ $(SERVER_OBJS) -L$(LIBFT_DIR) -lft

# Build client
$(NAME_CLIENT): $(CLIENT_OBJS) $(LIBFT_DIR)/libft.a
	$(CC) $(CFLAGS) -o $@ $(CLIENT_OBJS) -L$(LIBFT_DIR) -lft

# Compile .c to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	$(RM) $(SERVER_OBJS) $(CLIENT_OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME_SERVER) $(NAME_CLIENT)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re