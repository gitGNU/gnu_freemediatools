all: ../../fire3D_frag_gpu.fp ../../fire3D_vert_gpu.vp Makefile

../../fire3D_frag_gpu.fp: fire3D_frag_gpu.cg Makefile
	cgc -profile fp30 fire3D_frag_gpu.cg -o ../../fire3D_frag_gpu.fp

../../fire3D_vert_gpu.vp: fire3D_vert_gpu.cg Makefile
	cgc -profile vp30 fire3D_vert_gpu.cg -o ../../fire3D_vert_gpu.vp



