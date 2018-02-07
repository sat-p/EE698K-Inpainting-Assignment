top_builddir = ../../..
include ${top_builddir}/makeinclude

CPP_SRC = \
	slic.cxx \
	slicGC.cxx \
	omp.cxx \
	sparse_inpaint.cxx

OBJ = ${CPP_SRC:%.cxx=%.o}
DEP = ${CPP_SRC:%.cxx=%.dep}

ifeq (${MAKECMDGOALS}, obj)
include ${DEP}
endif

dep: ${DEP}

obj: ${OBJ}

lib: libalgos.a

libalgos.a : ${OBJ}
	${AR} $@ $(OBJ)

clean:
	$(RM) $(DEP) $(OBJ) libalgos.a

.PHONY: dep obj clean