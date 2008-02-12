all: ../../fisheye_frag_gpgpu.fp Makefile

../../fisheye_frag_gpgpu.fp: fisheye_frag_gpgpu.cg 
	cgc -profile fp30 fisheye_frag_gpgpu.cg -o ../../fisheye_frag_gpgpu.fp
