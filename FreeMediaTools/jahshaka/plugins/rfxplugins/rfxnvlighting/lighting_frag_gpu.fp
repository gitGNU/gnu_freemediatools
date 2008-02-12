!!FP1.0
# cgc version 1.3.0001, build date Aug  4 2004 10:01:10
# command line args: -profile fp30
# source file: lighting_frag_gpu.cg
#vendor NVIDIA Corporation
#version 1.0.02
#profile fp30
#program main
#semantic main.BaseTexture : TEXUNIT0
#semantic main.contrastTexture : TEXUNIT1
#semantic main.randomTexture : TEXUNIT2
#semantic main.light_color : C0
#var float2 base_uv : $vin.TEX0 : TEX0 : 0 : 1
#var float4 light1Position :  :  : 1 : 0
#var float edge : $vin.TEX2 : TEX2 : 2 : 1
#var float2 random_uv : $vin.TEX3 : TEX3 : 3 : 1
#var float2 contrast_uv : $vin.TEX4 : TEX4 : 4 : 1
#var sampler2D BaseTexture : TEXUNIT0 : texunit 0 : 5 : 1
#var sampler2D contrastTexture : TEXUNIT1 : texunit 1 : 6 : 1
#var sampler2D randomTexture : TEXUNIT2 : texunit 2 : 7 : 1
#var float4 light_color : C0 : c[0] : 8 : 1
#var float4 color : $vout.COL : COL : 9 : 1
DECLARE light_color;
TEX   R0, f[TEX0], TEX0, 2D;
MULR  R0.xyz, f[TEX2].x, R0;
MULR  o[COLR].xyz, R0, light_color;
MOVR  o[COLR].w, {1}.x;
END
# 4 instructions, 1 R-regs, 0 H-regs
