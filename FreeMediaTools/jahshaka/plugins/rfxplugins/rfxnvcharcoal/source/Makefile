all: ../../charcoal_frag_gpu.fp ../../charcoal_vert_gpu.vp Makefile

../../charcoal_frag_gpu.fp: charcoal_frag_gpu.cg Makefile
	cgc -profile fp30 charcoal_frag_gpu.cg -o ../../charcoal_frag_gpu.fp

../../charcoal_vert_gpu.vp: charcoal_vert_gpu.cg Makefile
	cgc -profile vp30 charcoal_vert_gpu.cg -o ../../charcoal_vert_gpu.vp



