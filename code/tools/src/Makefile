top_builddir = ../../..
include ${top_builddir}/makeinclude

CPP_SRC = \
    sparse_inpaint.cxx

OBJ = ${CPP_SRC:%.cxx=%.o}
DEP = ${CPP_SRC:%.cxx=%.dep}

ifeq (${MAKECMDGOALS}, obj)
include ${DEP}
endif

dep: ${DEP}

obj: ${OBJ}

lib: libtools.a

libtools.a : ${OBJ}
	${AR} $@ $(OBJ)

bin : create_dictionary.cxx
	g++ -Wall -O3 -std=c++14 create_dictionary.cxx -o create_dictionary `pkg-config opencv --cflags --libs`
	
clean:
	$(RM) $(DEP) $(OBJ) libtools.a create_dictionary

.PHONY: dep obj clean
