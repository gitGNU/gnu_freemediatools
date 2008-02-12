all: ../diffusion_frag_gpu.fp ../diffusion_vert_gpu.vp Makefile

../diffusion_frag_gpu.fp: diffusion_frag_gpu.cg Makefile
	cgc -profile fp30 diffusion_frag_gpu.cg -o ../diffusion_frag_gpu.fp

../diffusion_vert_gpu.vp: diffusion_vert_gpu.cg Makefile
	cgc -profile vp30 diffusion_vert_gpu.cg -o ../diffusion_vert_gpu.vp



