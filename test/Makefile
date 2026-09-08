# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                            :+:      :+:    :+:  #
#                                                      +:+ +:+         +:+     #
#    By: jchartie <jchartie@student.42.fr>           +#+  +:+       +#+       #
#                                                  +#+#+#+#+#+   +#+           #
#    Created: 2026/09/08 15:33:35 by jchartie           #+#    #+#             #
#    Updated: 2026/09/08 15:36:10 by jchartie          ###   ########.fr       #
#                                                                              #
# **************************************************************************** #

# Compiler
CC = cc
CFLAGS = -Wall -Wextra -Werror -DTESTING -g3 -O0
NAME = test_runner

# Directories
SRC_DIR = ../src
TEST_DIR = .
OBJ_DIR = obj

LIBFT_DIR = ../lib/libft
GNL_DIR = ../lib/gnl

INCLUDES = -I../include -I$(LIBFT_DIR) -I$(GNL_DIR) -I.

# Project sources, excluding main.c, plus every test/*.c file
SRC_FILES = $(filter-out main.c, $(notdir $(wildcard $(SRC_DIR)/*.c)))
TEST_FILES = $(notdir $(wildcard $(TEST_DIR)/*.c))

SRC_OBJS = $(addprefix $(OBJ_DIR)/src/, $(SRC_FILES:.c=.o))
TEST_OBJS = $(addprefix $(OBJ_DIR)/test/, $(TEST_FILES:.c=.o))
OBJS = $(SRC_OBJS) $(TEST_OBJS)

# GNL and libft, same as the project Makefile
GNL_SOURCES = get_next_line.c get_next_line_utils.c
GNL_OBJ = $(addprefix $(GNL_DIR)/, $(GNL_SOURCES:.c=.o))
LIBFT = $(LIBFT_DIR)/libft.a

# Default rule
all: $(NAME)

$(NAME): $(OBJS) $(GNL_OBJ) $(LIBFT)
	$(CC) $(OBJS) $(GNL_OBJ) $(LIBFT) -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(GNL_DIR)/%.o: $(GNL_DIR)/%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES)

# Project object files (everything except main.c)
$(OBJ_DIR)/src/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)/src
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES)

# Test object files
$(OBJ_DIR)/test/%.o: $(TEST_DIR)/%.c | $(OBJ_DIR)/test
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES)

$(OBJ_DIR)/src $(OBJ_DIR)/test:
	@mkdir -p $@

.PHONY: all clean fclean re run

# Build then run the tests in one go
run: all
	./$(NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(GNL_OBJ)
	rm -f $(NAME)

re: fclean all
