all: ../../blur_frag_gpu.fp ../../blur_frag_arb_gpu.fp

../../blur_frag_gpu.fp: blur_frag_gpu.cg Makefile
	cgc -profile fp30 blur_frag_gpu.cg -o ../blur_frag_gpu.fp

../../blur_frag_arb_gpu.fp: blur_frag_gpu.cg Makefile
	cgc -profile arbfp1 blur_frag_gpu.cg -o ../blur_frag_arb_gpu.fp





