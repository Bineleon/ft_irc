NAME = ircserv
SRCS = srcs/main.cpp
INCLUDE = includes/ircserv.hpp

OBJS_DIR = objects
OBJS = ${addprefix ${OBJS_DIR}/, ${SRCS:.cpp=.o}}

DEPS_DIR = dependances
DEPS = ${addprefix ${DEPS_DIR}/, ${OBJS:.o=.d}}

CC = c++
CFLAGS = -Wall -Werror -Wextra -std=c++98 -g -MMD -MP

${OBJS_DIR}/%.o: %.cpp ${INCLUDE}
		mkdir -p ${OBJS_DIR} ${DEPS_DIR}
		${CC} ${CFLAGS} -c $< -o $@ -MF ${DEPS_DIR}/$*.d

${NAME}: ${OBJS} ${INCLUDE} Makefile
		${CC} ${CFLAGS} ${OBJS} -I ./ -o $@

all: ${NAME}

clean:
		rm -rf ${OBJS_DIR} ${DEPS_DIR}

fclean: clean;
		rm -f ${NAME}

-include ${DEPS}

re: fclean all

.PHONY: all clean fclean re
