all: ../swirl_frag_arb_gpu.fp ../swirl_frag_gpu.fp Makefile

../swirl_frag_gpu.fp: swirl_frag_gpu.cg Makefile
	cgc -profile fp30 swirl_frag_gpu.cg -o ../swirl_frag_gpu.fp

../swirl_frag_arb_gpu.fp: swirl_frag_gpu.cg Makefile
	cgc -profile arbfp1 swirl_frag_gpu.cg -o ../swirl_frag_arb_gpu.fp



