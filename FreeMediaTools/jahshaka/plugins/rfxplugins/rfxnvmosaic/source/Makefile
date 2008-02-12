all: ../mosaic_frag.fp  ../mosaic_frag_arb.fp Makefile

../mosaic_frag.fp: mosaic_frag.cg Makefile
	cgc -profile fp30 mosaic_frag.cg -o ../mosaic_frag.fp

../mosaic_frag_arb.fp: mosaic_frag.cg Makefile
	cgc -profile arbfp1 mosaic_frag.cg -o ../mosaic_frag_arb.fp

