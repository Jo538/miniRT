# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: admin <admin@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/09/07 14:27:05 by jchartie          #+#    #+#              #
#    Updated: 2026/09/10 13:43:34 by admin            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -O0
NAME = miniRT

# Directories
SRC_DIR = src/
OBJ_DIR = obj/
LIBFT_DIR = lib/libft
GNL_DIR = lib/gnl

# Libraries and Flags
ifeq ($(shell uname), Darwin)
	MLX_DIR = lib/mlx_macos
	INCLUDES = -I$(MLX_DIR)
	MLX_FLAGS = -framework OpenGL -framework AppKit
else
	MLX_DIR = lib/mlx_linux
	INCLUDES = -I$(MLX_DIR) -I/usr/include
	MLX_FLAGS = -L/usr/lib -lXext -lX11 -lm -lz
endif
INCLUDES += -Iinclude -I$(LIBFT_DIR) -I$(GNL_DIR)

# Sources and Objects
VPATH = $(SRC_DIR):$(GNL_DIR):src/parser:src/checker
SOURCES = main.c file_reader.c get_next_line.c get_next_line_utils.c \
          checker.c checker2.c object_parser.c
OBJECTS = $(addprefix $(OBJ_DIR), $(SOURCES:.c=.o))
LIB_OBJ = $(LIBFT_DIR)/libft.a $(MLX_DIR)/libmlx.a

# Default rule
all: $(NAME)

# Make the miniRT executable
$(NAME): $(OBJECTS) $(LIB_OBJ)
	$(CC) $(OBJECTS) $(LIB_OBJ) $(MLX_FLAGS) -o $(NAME)

# Make each library ($(@D) expands to the .a file's directory)
%.a:
	$(MAKE) -C $(@D)

# mlx is third-party: -w silences its deprecated OpenGL/prototype warnings.
# Explicit target beats the %.a pattern rule, so libft still gets its warnings.
$(MLX_DIR)/libmlx.a:
	$(MAKE) -C $(@D) CC="$(CC) -w"

# Make object files: VPATH lets make find the matching .c in src/ or lib/gnl
$(OBJ_DIR)%.o: %.c | $(OBJ_DIR)
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES)

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

# Phony targets declaration
.PHONY: all clean fclean re

# Clean project's object files
clean:
	rm -f $(OBJECTS)
	@if [ -d $(OBJ_DIR) ]; then rmdir $(OBJ_DIR); fi

# Clean project's miniRT executable and clean libraries
fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(MAKE) -C $(MLX_DIR) clean
	rm -f $(NAME)

# Recompile all files
re: fclean
	$(MAKE) all
