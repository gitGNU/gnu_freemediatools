all: ../../earthquake_frag_gpu.fp ../../earthquake_vert_gpu.vp Makefile

../../earthquake_frag_gpu.fp: earthquake_frag_gpu.cg Makefile
	cgc -profile fp30 earthquake_frag_gpu.cg -o ../../earthquake_frag_gpu.fp

../../earthquake_vert_gpu.vp: earthquake_vert_gpu.cg Makefile
	cgc -profile vp30 earthquake_vert_gpu.cg -o ../../earthquake_vert_gpu.vp



