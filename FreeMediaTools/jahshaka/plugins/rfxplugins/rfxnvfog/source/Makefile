all: ../fog_frag_gpu.fp ../fog_vert_gpu.vp Makefile

../fog_frag_gpu.fp: fog_frag_gpu.cg Makefile
	cgc -profile fp30 fog_frag_gpu.cg -o ../fog_frag_gpu.fp

../fog_vert_gpu.vp: fog_vert_gpu.cg Makefile
	cgc -profile vp30 fog_vert_gpu.cg -o ../fog_vert_gpu.vp



