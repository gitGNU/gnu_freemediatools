all: ../turbulence_frag_gpu.fp ../turbulence_vert_gpu.vp ../turbulence_combiner_frag_gpu.fp Makefile

../turbulence_frag_gpu.fp: turbulence_frag_gpu.cg Makefile
	cgc -profile fp30 turbulence_frag_gpu.cg -o ../turbulence_frag_gpu.fp

../turbulence_vert_gpu.vp: turbulence_vert_gpu.cg Makefile
	cgc -profile vp30 turbulence_vert_gpu.cg -o ../turbulence_vert_gpu.vp

../turbulence_combiner_frag_gpu.fp: turbulence_combiner_frag_gpu.cg Makefile
	cgc -profile fp30 turbulence_combiner_frag_gpu.cg -o ../turbulence_combiner_frag_gpu.fp

