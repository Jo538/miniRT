# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jchartie <jchartie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/09/07 14:27:05 by jchartie          #+#    #+#              #
#    Updated: 2026/09/15 14:36:00 by jchartie         ###   ########.fr        #
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
INCLUDES = -Iinclude -I$(LIBFT_DIR) -I$(GNL_DIR)

# Sources and Objects
VPATH = $(SRC_DIR):$(GNL_DIR):src/parser:src/checker
SOURCES = main.c file_reader.c get_next_line.c get_next_line_utils.c \
          line.c line_bis.c nested_items.c utils_bis.c utils.c object_parser.c \
		  parser_create_nodes.c parser_utils.c parser.c parserbis.c
OBJECTS = $(addprefix $(OBJ_DIR), $(SOURCES:.c=.o))
LIB_OBJ = $(LIBFT_DIR)/libft.a

# Default rule
all: $(NAME)

# Make the miniRT executable
$(NAME): $(OBJECTS) $(LIB_OBJ)
	$(CC) $(OBJECTS) $(LIB_OBJ) -o $(NAME)

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
	rm -f $(NAME)

# Recompile all files
re: fclean
	$(MAKE) all
