all: ../../create_turbulence_texture_frag_gpu.fp ../../create_turbulence_texture_vert_gpu.vp ../../create_turbulence_texture_combiner_frag_gpu.fp Makefile

../../create_turbulence_texture_frag_gpu.fp: create_turbulence_texture_frag_gpu.cg Makefile
	cgc -profile fp30 create_turbulence_texture_frag_gpu.cg -o ../../create_turbulence_texture_frag_gpu.fp

../../create_turbulence_texture_vert_gpu.vp: create_turbulence_texture_vert_gpu.cg Makefile
	cgc -profile vp30 create_turbulence_texture_vert_gpu.cg -o ../../create_turbulence_texture_vert_gpu.vp

../../create_turbulence_texture_combiner_frag_gpu.fp: create_turbulence_texture_combiner_frag_gpu.cg Makefile
	cgc -profile fp30 create_turbulence_texture_combiner_frag_gpu.cg -o ../../create_turbulence_texture_combiner_frag_gpu.fp

