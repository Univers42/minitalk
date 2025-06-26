NAME=minitalk.a
PROG=client server
PROG_BONUS=client_bonus server_bonus
CC=cc
CFLAGS=-Wall -Wextra -Werror -I. -I$(LIBFT_DIR) -D_GNU_SOURCE
#CFLAGS+=-g ##Uncomment for debugging
AR = ar rcs
RM = rm -f
SRCS = client.c server.c utils.c
SRCS_BONUS=client_bonus.c server_bonus.c utils_bonus.c
OBJ_DIR=objs
OBJS_CLIENT=$(OBJ_DIR)/client.o $(OBJ_DIR)/utils.o
OBJS_SERVER=$(OBJ_DIR)/server.o $(OBJ_DIR)/utils.o
OBJS_CLIENT_BONUS=$(OBJ_DIR)/client_bonus.o $(OBJ_DIR)/utils_bonus.o
OBJS_SERVER_BONUS=$(OBJ_DIR)/server_bonus.o $(OBJ_DIR)/utils_bonus.o
LIBFT_DIR=libft
LIBFT=$(LIBFT_DIR)/libft.a

MAKEFLAGS += --no-print-directory
all: client server

client: $(OBJS_CLIENT) $(LIBFT)
	$(CC) $(CFLAGS) -o client $(OBJS_CLIENT) -L$(LIBFT_DIR) -lft

server: $(OBJS_SERVER) $(LIBFT)
	$(CC) $(CFLAGS) -o server $(OBJS_SERVER) -L$(LIBFT_DIR) -lft

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

bonus: client_bonus server_bonus

client_bonus: $(OBJS_CLIENT_BONUS) $(LIBFT)
	$(CC) $(CFLAGS) -o client_bonus $(OBJS_CLIENT_BONUS) -L$(LIBFT_DIR) -lft

server_bonus: $(OBJS_SERVER_BONUS) $(LIBFT)
	$(CC) $(CFLAGS) -o server_bonus $(OBJS_SERVER_BONUS) -L$(LIBFT_DIR) -lft

build_libft:
	make -C $(LIBFT_DIR)
clean: 
	$(RM) $(OBJ_DIR)/*.o
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) client server $(LIBFT)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re client server bonus client_bonus server_bonus
.SILENT:
