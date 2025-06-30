# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/30 17:17:44 by dlesieur          #+#    #+#              #
#    Updated: 2025/06/30 18:08:47 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Project info
PROG_NAME = server client
PROJECT_DIR = .
CC = cc
CFLAGS = -Wall -Wextra -Werror

# Directories
DIR_MANDATORY = $(PROJECT_DIR)/mandatory
DIR_BONUS = $(PROJECT_DIR)/bonus
LIBFT_DIR := libft
LIBFT_MAKE := $(MAKE) -C $(LIBFT_DIR)

# Headers
INCLUDES = minitalk.h libft/libft.h

# Sources
SRCS = $(DIR_MANDATORY)/client.c $(DIR_MANDATORY)/server.c $(DIR_MANDATORY)/utils.c
SRCS_BONUS = $(DIR_BONUS)/client_bonus.c $(DIR_BONUS)/server_bonus.c $(DIR_BONUS)/utils_bonus.c

# Object files
OBJS = $(SRCS:.c=.o)
OBJS_BONUS = $(SRCS_BONUS:.c=.o)

# Select actual objects depending on BONUS
ifdef BONUS
	OBJS_ACTUAL := $(OBJS_BONUS)
else
	OBJS_ACTUAL := $(OBJS)
endif

# Tools
AR = ar rcs
RM = rm -rf

# Output
NAME = minitalk.a

# Default target
all: build $(NAME) $(PROG_NAME)

# Bonus build
bonus: BONUS=1
bonus: all

# Rule to build the static library
$(NAME): $(OBJS_ACTUAL)
	$(AR) $(NAME) $(OBJS_ACTUAL)

# Program rules (server/client)
$(PROG_NAME): %: %.o $(NAME)
	$(CC) $(CFLAGS) -o $@ $^ -L. -L$(LIBFT_DIR) -lft

# Pattern rule to compile .c → .o with header deps
%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -I. -I$(LIBFT_DIR) -c $< -o $@

# Build libft
build:
	$(LIBFT_MAKE)

# Clean rules
clean:
	$(RM) $(OBJS) $(OBJS_BONUS)
	$(LIBFT_MAKE) clean

fclean: clean
	$(RM) $(NAME)
	$(RM) $(PROG_NAME)
	$(LIBFT_MAKE) fclean

re: fclean all

.PHONY: all bonus build clean fclean re $(PROG_NAME)
