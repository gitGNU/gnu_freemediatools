all: ../distortion_frag_gpu.fp ../distortion_vert_gpu.vp Makefile

../distortion_frag_gpu.fp: distortion_frag_gpu.cg Makefile
	cgc -profile fp30 distortion_frag_gpu.cg -o ../distortion_frag_gpu.fp

../distortion_vert_gpu.vp: distortion_vert_gpu.cg Makefile
	cgc -profile vp30 distortion_vert_gpu.cg -o ../distortion_vert_gpu.vp



