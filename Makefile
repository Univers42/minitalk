# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/30 17:17:44 by dlesieur          #+#    #+#              #
#    Updated: 2025/07/01 10:49:51 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Project Metadata
PROJECT_NAME := minitalk
PROG_NAME := server client
PROJECT_DIR := .
NAME := $(PROJECT_NAME).a

# Compiler & Tools
CC := cc
CFLAGS := -Wall -Wextra -Werror
ifeq ($(MINITALK_DEBUG),1)
	CFLAGS += -DMINITALK_DEBUG=1
else
	CFLAGS += -DMINITALK_DEBUG=0
endif
AR := ar rcs
RM := rm -rf

# Directories
MANDATORY_DIR := $(PROJECT_DIR)/mandatory
BONUS_DIR := $(PROJECT_DIR)/bonus
LIBFT_DIR := libft

# Includes
INCLUDES := minitalk.h $(LIBFT_DIR)/libft.h
INCLUDE_FLAGS := -I. -I$(LIBFT_DIR)

# Source files
UTILS := logs.c
SRCS := $(MANDATORY_DIR)/client.c \
	$(MANDATORY_DIR)/server.c \
	$(MANDATORY_DIR)/utils.c \
	$(MANDATORY_DIR)/server_utils.c\
	$(MANDATORY_DIR)/server_utils2.c\
	$(UTILS)

SRCS_BONUS := $(BONUS_DIR)/client_bonus.c \
	$(BONUS_DIR)/server_bonus.c \
	$(BONUS_DIR)/utils_bonus.c \
	$(UTILS)

# Object files
OBJS := $(SRCS:.c=.o)
OBJS_BONUS := $(SRCS_BONUS:.c=.o)

# Handle bonus switch
ifdef BONUS
 OBJS_ACTUAL := $(OBJS_BONUS)
 SRCS_ACTUAL := $(SRCS_BONUS)
else
 OBJS_ACTUAL := $(OBJS)
 SRCS_ACTUAL := $(SRCS)
endif

# Determine object paths dynamically from OBJS_ACTUAL
OBJS_PATH := $(dir $(firstword $(OBJS_ACTUAL)))

# Define macro for libft make
define LIBFT_MAKE
	$(MAKE) -C $(LIBFT_DIR) $(1)
endef

# Default rule
all: build $(NAME) $(PROG_NAME)

# Bonus support
bonus:
	$(MAKE) BONUS=1 DEBUG=1 all

# Static library
$(NAME): $(OBJS_ACTUAL)
	$(AR) $(NAME) $(OBJS_ACTUAL)

# Build binaries
server: $(OBJS_PATH)server$(if $(BONUS),_bonus).o $(NAME)
	$(CC) $(CFLAGS) -o $@ $< $(NAME) -L. -L$(LIBFT_DIR) -lft

client: $(OBJS_PATH)client$(if $(BONUS),_bonus).o $(NAME)
	$(CC) $(CFLAGS) -o $@ $< $(NAME) -L. -L$(LIBFT_DIR) -lft

# Compile .c → .o with header deps (only for actual sources)
$(OBJS_ACTUAL): %.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

# Build libft
build:
	$(call LIBFT_MAKE)

# Clean up
clean:
	$(RM) $(OBJS) $(OBJS_BONUS)
	$(call LIBFT_MAKE,clean)

fclean: clean
	$(RM) $(NAME) $(PROG_NAME)
	$(call LIBFT_MAKE,fclean)

re: fclean all

.PHONY: all bonus build clean fclean re $(PROG_NAME)