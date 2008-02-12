all: ../pan_frag_arb_gpu.fp ../pan_frag_gpu.fp

../pan_frag_gpu.fp: pan_frag_gpu.cg Makefile
	cgc -profile fp30 pan_frag_gpu.cg -o ../pan_frag_gpu.fp

../pan_frag_arb_gpu.fp: pan_frag_gpu.cg Makefile
	cgc -profile arbfp1 pan_frag_gpu.cg -o ../pan_frag_arb_gpu.fp



