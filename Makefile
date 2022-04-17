# disable implcit rules and variables
MAKEFLAGS += rR
$(foreach x,$(filter-out .% MAKE% SHELL CURDIR,$(.VARIABLES)) MAKEINFO,$(if $(filter default,$(origin $x)),$(eval override undefine $x)))

BINARY ?= flappybird

SRC_DIRECTORY = ./src
BUILD_DIRECTORY = ./build
INCLUDE_DIRECTORY += ./include

CC ?= clang++
ifdef NDEBUG
	CFLAGS ?= -O3 -DNDEBUG
else
	CFLAGS ?= -g
endif
LD_FLAGS ?= -lSDL2
CXX_STANDARD ?= c++20
COMPILE_FLAGS := ${CFLAGS} --std=${CXX_STANDARD} -I ${INCLUDE_DIRECTORY} -Wall -Wextra
DEP_FILE = ./.cache/dep.d

BINARY_PATH=${BUILD_DIRECTORY}/${BINARY}

SRCS := $(shell find $(SRC_DIRECTORY) -name '*.cpp')
OBJS := $(SRCS:$(SRC_DIRECTORY)/%.cpp=$(BUILD_DIRECTORY)/%.o)

.PHONY: build
build: ${BINARY_PATH}

.PHONY: clean
clean:
	@rm --verbose -f $(wildcard $(BUILD_DIRECTORY)/*)
	@rm --verbose ${DEP_FILE}


${BINARY_PATH}: ${OBJS}
	${CC} $^ -o $@ ${LD_FLAGS} ${CFLAGS}

${OBJS}: ${BUILD_DIRECTORY}/%.o:
	${CC} ${SRC_DIRECTORY}/$*.cpp -o $@ -c ${COMPILE_FLAGS}

$(shell ${CC} -MM -I ${INCLUDE_DIRECTORY} --std=${CXX_STANDARD} ${SRC_DIRECTORY}/* > ${DEP_FILE})
include ${DEP_FILE}
