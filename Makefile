NAME = webserv
CXX = c++
RM = rm -f
INCLUDES = -I./include
CXXFLAGS = -std=c++11 -Wall -Wextra -Werror
OBJ_DIR = obj/

SRC_1 = main.cpp \
		src/errors/parsing/errors.cpp \
		src/parsing/ConfigParser.cpp \
		src/parsing/LocationConfig.cpp \
		src/parsing/LocationConfigParser.cpp \
		src/parsing/ServerConfig.cpp \
		src/parsing/ServerConfigParser.cpp \
		src/HTTP-Core/Client.cpp \
		src/HTTP-Core/HttpParser.cpp \
		src/HTTP-Core/HttpResponse.cpp \
		src/HTTP-Core/RequestHandler.cpp \
		src/HTTP-Core/WebServ.cpp \
		src/utils/Logger.cpp \
		src/utils/ServerStats.cpp \
		src/utils/LRUCache.cpp \
		src/utils/TrieRouter.cpp \
		src/utils/RateLimiter.cpp

OBJ_1 = $(patsubst %.cpp,$(OBJ_DIR)%.o,$(SRC_1))

$(OBJ_DIR)%.o: %.cpp
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

.cpp.o:
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

${NAME}: ${OBJ_1}
	@echo "Compiling $(NAME)..."
	@${CXX} ${CXXFLAGS} ${OBJ_1} -o ${NAME}
	@echo "$(NAME) compiled successfully."

all: ${NAME}

clean:
	@echo "Cleaning object files..."
	@${RM} ${OBJ_1}
	@rm -rf ${OBJ_DIR}
	@echo "Object files cleaned."

fclean: clean
	@echo "Cleaning executable..."
	@${RM} ${NAME}
	@rm -rf ${OBJ_DIR}
	@echo "Executable cleaned."

re: clean all

.PHONY: all clean fclean re