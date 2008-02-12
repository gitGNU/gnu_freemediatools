all: ../fire_frag_gpu.fp ../fire_vert_gpu.vp Makefile

../fire_frag_gpu.fp: fire_frag_gpu.cg Makefile
	cgc -profile fp30 fire_frag_gpu.cg -o ../fire_frag_gpu.fp

../fire_vert_gpu.vp: fire_vert_gpu.cg Makefile
	cgc -profile vp30 fire_vert_gpu.cg -o ../fire_vert_gpu.vp



