all: ../flame_frag_gpu.fp ../flame_vert_gpu.vp ../flame_combiner_frag_gpu.fp ../flame_blur_frag_gpu.fp Makefile

../flame_frag_gpu.fp: flame_frag_gpu.cg Makefile
	cgc -profile fp30 flame_frag_gpu.cg -o ../flame_frag_gpu.fp

../flame_vert_gpu.vp: flame_vert_gpu.cg Makefile
	cgc -profile vp30 flame_vert_gpu.cg -o ../flame_vert_gpu.vp

../flame_combiner_frag_gpu.fp: flame_combiner_frag_gpu.cg Makefile
	cgc -profile fp30 flame_combiner_frag_gpu.cg -o ../flame_combiner_frag_gpu.fp

../flame_blur_frag_gpu.fp: flame_blur_frag_gpu.cg Makefile
	cgc -profile fp30 flame_blur_frag_gpu.cg -o ../flame_blur_frag_gpu.fp

