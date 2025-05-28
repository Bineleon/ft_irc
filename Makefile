NAME = ircserv

SRC_DIR = srcs/
SRCS = main.cpp Server.cpp Client.cpp
SRC = ${addprefix ${SRC_DIR}, ${SRCS}}

INCLUDE_DIR = includes/
INCLUDES =  Server.hpp Client.hpp
INCLUDE = ${addprefix ${INCLUDE_DIR}, ${INCLUDES}}

OBJS_DIR = objects/
OBJS = ${addprefix ${OBJS_DIR}, ${SRCS:.cpp=.o}}

DEPS_DIR = dependances/
DEPS = ${addprefix ${DEPS_DIR}, ${SRCS:.cpp=.d}}

CXX = c++
CFLAGS = -Wall -Werror -Wextra -std=c++98 -g -MMD -MP

${OBJS_DIR}%.o: ${SRC_DIR}%.cpp ${INCLUDE}
		mkdir -p ${OBJS_DIR} ${DEPS_DIR}
		${CXX} ${CFLAGS} -c $< -o $@ -MF ${DEPS_DIR}$*.d

${NAME}: ${OBJS} ${INCLUDE} Makefile
		${CXX} ${CFLAGS} ${OBJS} -I ./ -o $@

all: ${NAME}

clean:
		rm -rf ${OBJS_DIR} ${DEPS_DIR}

fclean: clean;
		rm -f ${NAME}

-include ${DEPS}

re: fclean all

.PHONY: all clean fclean re
