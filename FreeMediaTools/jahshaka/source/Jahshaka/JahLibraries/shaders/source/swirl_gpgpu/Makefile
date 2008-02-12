all: ../../swirl_frag_gpgpu.fp Makefile

../../swirl_frag_gpgpu.fp: swirl_frag_gpgpu.cg 
	cgc -profile fp30 swirl_frag_gpgpu.cg -o ../../swirl_frag_gpgpu.fp
