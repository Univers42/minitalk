NAME=minitalk.a
PROG=client server
PROG_BONUS=client server
CC=cc
CFLAGS=-Wall -Wextra -Werror -I. -I$(LIBFT_DIR) -D_GNU_SOURCE
CFLAGS_DEBUG=$(CFLAGS) -DMINITALK_DEBUG=1
DEBUG_FLAG=-DMINITALK_DEBUG=1
#CFLAGS+=-g ##Uncomment for debugging
AR = ar rcs
RM = rm -f
SRCS = client_temp.c server_temp.c utils.c log.c server_utils.c server_utils2.c
SRCS_BONUS=client_bonus.c server_bonus.c utils_bonus.c free.c log.c
# log.h is included in the project for all builds
OBJ_DIR=objs
OBJ_DIR_DEBUG=objs_debug
OBJS_CLIENT=$(OBJ_DIR)/client.o $(OBJ_DIR)/utils.o $(OBJ_DIR)/log.o
OBJS_SERVER=$(OBJ_DIR)/server.o $(OBJ_DIR)/utils.o $(OBJ_DIR)/log.o $(OBJ_DIR)/server_utils.o $(OBJ_DIR)/server_utils2.o
OBJS_CLIENT_BONUS=$(OBJ_DIR)/client_bonus.o $(OBJ_DIR)/utils_bonus.o $(OBJ_DIR)/free.o $(OBJ_DIR)/log.o
OBJS_SERVER_BONUS=$(OBJ_DIR)/server_bonus.o $(OBJ_DIR)/utils_bonus.o $(OBJ_DIR)/free.o $(OBJ_DIR)/log.o
OBJS_CLIENT_DEBUG=$(OBJ_DIR_DEBUG)/client.o $(OBJ_DIR_DEBUG)/utils.o $(OBJ_DIR_DEBUG)/log.o
OBJS_SERVER_DEBUG=$(OBJ_DIR_DEBUG)/server.o $(OBJ_DIR_DEBUG)/utils.o $(OBJ_DIR_DEBUG)/log.o $(OBJ_DIR)/server_utils.o $(OBJ_DIR)/server_utils2.o
OBJS_CLIENT_BONUS_DEBUG=$(OBJ_DIR_DEBUG)/client_bonus.o $(OBJ_DIR_DEBUG)/utils_bonus.o $(OBJ_DIR_DEBUG)/free.o $(OBJ_DIR_DEBUG)/log.o
OBJS_SERVER_BONUS_DEBUG=$(OBJ_DIR_DEBUG)/server_bonus.o $(OBJ_DIR_DEBUG)/utils_bonus.o $(OBJ_DIR_DEBUG)/free.o $(OBJ_DIR_DEBUG)/log.o
LIBFT_DIR=libft
LIBFT=$(LIBFT_DIR)/libft.a

MAKEFLAGS += --no-print-directory
all: client server

client: $(OBJS_CLIENT) $(LIBFT)
	$(CC) $(CFLAGS) -o client $(OBJS_CLIENT) -L$(LIBFT_DIR) -lft

server: $(OBJS_SERVER) $(LIBFT)
	$(CC) $(CFLAGS) -o server $(OBJS_SERVER) -L$(LIBFT_DIR) -lft

debug: debug_client debug_server

debug_client: $(OBJS_CLIENT_DEBUG) $(LIBFT)
	$(CC) $(CFLAGS_DEBUG) -o client $(OBJS_CLIENT_DEBUG) -L$(LIBFT_DIR) -lft

debug_server: $(OBJS_SERVER_DEBUG) $(LIBFT)
	$(CC) $(CFLAGS_DEBUG) -o server $(OBJS_SERVER_DEBUG) -L$(LIBFT_DIR) -lft

bonus: bonus_client bonus_server

bonus_client: $(OBJS_CLIENT_BONUS) $(LIBFT)
	$(CC) $(CFLAGS) -o client $(OBJS_CLIENT_BONUS) -L$(LIBFT_DIR) -lft

bonus_server: $(OBJS_SERVER_BONUS) $(LIBFT)
	$(CC) $(CFLAGS) -o server $(OBJS_SERVER_BONUS) -L$(LIBFT_DIR) -lft

bonus_debug: bonus_debug_client bonus_debug_server

bonus_debug_client: $(OBJS_CLIENT_BONUS_DEBUG) $(LIBFT)
	$(CC) $(CFLAGS_DEBUG) -o client $(OBJS_CLIENT_BONUS_DEBUG) -L$(LIBFT_DIR) -lft

bonus_debug_server: $(OBJS_SERVER_BONUS_DEBUG) $(LIBFT)
	$(CC) $(CFLAGS_DEBUG) -o server $(OBJS_SERVER_BONUS_DEBUG) -L$(LIBFT_DIR) -lft

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR_DEBUG)/%.o: %.c
	@mkdir -p $(OBJ_DIR_DEBUG)
	$(CC) $(CFLAGS_DEBUG) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

build_libft:
	make -C $(LIBFT_DIR)
clean: 
	$(RM) $(OBJ_DIR)/*.o
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) client server $(LIBFT)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re client server debug debug_client debug_server bonus bonus_client bonus_server bonus_debug bonus_debug_client bonus_debug_server build_libft
.SILENT:
