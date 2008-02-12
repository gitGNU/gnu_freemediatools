!!FP1.0
# cgc version 1.3.0001, build date Aug  4 2004 10:01:10
# command line args: -profile fp30
# source file: charcoal_frag_gpu.cg
#vendor NVIDIA Corporation
#version 1.0.02
#profile fp30
#program main
#semantic main.BaseTexture : TEXUNIT0
#semantic main.contrastTexture : TEXUNIT1
#semantic main.randomTexture : TEXUNIT2
#semantic main.darker : C0
#semantic main.transparency : C1
#semantic main.blend : C2
#semantic main.random_factor : C3
#var float2 base_uv : $vin.TEX0 : TEX0 : 0 : 1
#var float4 light1Position :  :  : 1 : 0
#var float edge : $vin.TEX2 : TEX2 : 2 : 1
#var float2 random_uv : $vin.TEX3 : TEX3 : 3 : 1
#var float2 contrast_uv : $vin.TEX4 : TEX4 : 4 : 1
#var sampler2D BaseTexture : TEXUNIT0 : texunit 0 : 5 : 1
#var sampler2D contrastTexture : TEXUNIT1 : texunit 1 : 6 : 1
#var sampler2D randomTexture : TEXUNIT2 : texunit 2 : 7 : 1
#var float darker : C0 : c[0] : 8 : 1
#var float transparency : C1 : c[1] : 9 : 1
#var float blend : C2 : c[2] : 10 : 1
#var float random_factor : C3 : c[3] : 11 : 1
#var float4 color : $vout.COL : COL : 12 : 1
DECLARE transparency;
DECLARE random_factor;
DECLARE darker;
DECLARE blend;
TEX   R1.xyz, f[TEX3], TEX2, 2D;
TEX   R0, f[TEX4], TEX1, 2D;
MULR  R1.xyz, R1, random_factor.x;
RCPR  R0.w, {10}.x;
MULR  R1.xyz, R1, R0.w;
RCPR  R0.w, {5}.x;
MADR  R0.xyz, R0, R0.w, R1;
MOVR  R0.w, {1}.x;
ADDR  R0.xyz, R0, -darker.x;
ADDR  R0.w, -transparency.x, R0;
MULR  R0.xyz, R0, R0.w;
MULR  R1.xyz, R0, blend.x;
TEX   R0, f[TEX0], TEX0, 2D;
MULR  R0.xyz, f[TEX2].x, R0;
MADR  o[COLR].xyz, R0, transparency.x, R1;
MOVR  o[COLR].w, {1}.x;
END
# 16 instructions, 2 R-regs, 0 H-regs
