all: ../../channelblur_frag.fp ../../channelblur_frag_arb.fp Makefile

../../channelblur_frag.fp: channelblur_frag.cg Makefile
	cgc -profile fp30 channelblur_frag.cg -o ../channelblur_frag.fp


../../channelblur_frag_arb.fp: channelblur_frag.cg Makefile
	cgc -profile arbfp1 channelblur_frag.cg -o ../channelblur_frag_arb.fp

