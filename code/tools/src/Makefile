top_builddir = ../../..
include ${top_builddir}/makeinclude

CPP_SRC = \
	convert.cxx \
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

clean:
	$(RM) $(DEP) $(OBJ) libtools.a

.PHONY: dep obj clean
