all: ../../jahshaka_basic_frag.fp ../../jahshaka_basic_vert.vp

../../jahshaka_basic_frag.fp :  jahshaka_basic_frag.cg Makefile
	cgc -profile fp30 -o ../../jahshaka_basic_frag.fp jahshaka_basic_frag.cg

../../jahshaka_basic_vert.vp : jahshaka_basic_vert.cg Makefile
	cgc -profile vp30 -o ../../jahshaka_basic_vert.vp jahshaka_basic_vert.cg
