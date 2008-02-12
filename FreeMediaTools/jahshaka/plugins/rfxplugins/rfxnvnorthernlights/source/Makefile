all: ../northernlights_frag_gpu.fp ../northernlights_vert_gpu.vp ../northernlights_combiner_frag_gpu.fp Makefile

../northernlights_frag_gpu.fp: northernlights_frag_gpu.cg Makefile
	cgc -profile fp30 northernlights_frag_gpu.cg -o ../northernlights_frag_gpu.fp

../northernlights_vert_gpu.vp: northernlights_vert_gpu.cg Makefile
	cgc -profile vp30 northernlights_vert_gpu.cg -o ../northernlights_vert_gpu.vp

../northernlights_combiner_frag_gpu.fp: northernlights_combiner_frag_gpu.cg Makefile
	cgc -profile fp30 northernlights_combiner_frag_gpu.cg -o ../northernlights_combiner_frag_gpu.fp

