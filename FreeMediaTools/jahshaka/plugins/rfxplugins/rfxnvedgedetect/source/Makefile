all: ../edgedetect_frag_gpu.fp Makefile

../edgedetect_frag_gpu.fp: edgedetect_frag_gpu.cg Makefile
	cgc -profile fp30 edgedetect_frag_gpu.cg -o ../edgedetect_frag_gpu.fp

