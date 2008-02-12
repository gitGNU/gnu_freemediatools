all: ../fisheye_frag_arb_gpu.fp ../fisheye_frag_gpu.fp Makefile

../fisheye_frag_gpu.fp: fisheye_frag_gpu.cg Makefile
	cgc -profile fp30 fisheye_frag_gpu.cg -o ../fisheye_frag_gpu.fp

../fisheye_frag_arb_gpu.fp: fisheye_frag_gpu.cg Makefile
	cgc -profile arbfp1 fisheye_frag_gpu.cg -o ../fisheye_frag_arb_gpu.fp



