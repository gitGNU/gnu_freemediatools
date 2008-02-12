all: ../sphere_frag_arb_gpu.fp ../sphere_frag_gpu.fp

../sphere_frag_gpu.fp: sphere_frag_gpu.cg Makefile
	cgc -profile fp30 sphere_frag_gpu.cg -o ../sphere_frag_gpu.fp

../sphere_frag_arb_gpu.fp: sphere_frag_gpu.cg Makefile
	cgc -profile arbfp1 sphere_frag_gpu.cg -o ../sphere_frag_arb_gpu.fp



