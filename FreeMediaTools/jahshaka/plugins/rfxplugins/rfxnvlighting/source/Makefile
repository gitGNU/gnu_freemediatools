all: ../../lighting_frag_gpu.fp ../../lighting_vert_gpu.vp Makefile

../../lighting_frag_gpu.fp: lighting_frag_gpu.cg Makefile
	cgc -profile fp30 lighting_frag_gpu.cg -o ../../lighting_frag_gpu.fp

../../lighting_vert_gpu.vp: lighting_vert_gpu.cg Makefile
	cgc -profile vp30 lighting_vert_gpu.cg -o ../../lighting_vert_gpu.vp



