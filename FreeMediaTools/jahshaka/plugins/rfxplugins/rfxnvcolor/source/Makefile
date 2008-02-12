all: ../color_frag_arb_gpu.fp ../color_frag_gpu.fp

../color_frag_gpu.fp: color_frag_gpu.cg Makefile
	cgc -profile fp30 color_frag_gpu.cg -o ../color_frag_gpu.fp
	cp ../color_frag_gpu.fp ../../../../source/Jahshaka/JahLibraries/shaders/

../color_frag_arb_gpu.fp: color_frag_gpu.cg Makefile
	cgc -profile arbfp1 color_frag_gpu.cg -o ../color_frag_arb_gpu.fp



