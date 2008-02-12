!!FP1.0
# cgc version 1.3.0001, build date Jan  7 2005 14:01:35
# command line args: -profile fp30
# source file: mosaic_frag.cg
#vendor NVIDIA Corporation
#version 1.0.02
#profile fp30
#program main
#semantic main.rectangle_size : C0
#semantic main.transparency : C1
#semantic main.screen_offset : C2
#semantic main.image_size : C3
#semantic main.texture_ratio : C4
#semantic main.texture : TEXUNIT0
#var float2 wpos : $vin.WPOS : WPOS : 0 : 1
#var float2 rectangle_size : C0 : c[0] : 1 : 1
#var float transparency : C1 : c[1] : 2 : 1
#var float2 screen_offset : C2 : c[2] : 3 : 1
#var float2 image_size : C3 : c[3] : 4 : 1
#var float2 texture_ratio : C4 : c[4] : 5 : 1
#var sampler2D texture : TEXUNIT0 : texunit 0 : 6 : 1
#var half4 main : $vout.COL : COL : -1 : 1
DECLARE screen_offset;
DECLARE image_size;
DECLARE texture_ratio;
DECLARE rectangle_size;
DECLARE transparency;
RCPR  R0.z, rectangle_size.x;
RCPR  R0.w, rectangle_size.y;
ADDR  R0.xy, f[WPOS], -screen_offset;
MULR  R0.zw, R0.xyxy, R0;
RCPR  R1.x, {2}.x;
FLRR  R0.zw, R0;
MULR  R1.xy, R1.x, rectangle_size;
MADR  R1.xy, R0.zwzw, rectangle_size, R1;
RCPR  R0.z, image_size.x;
RCPR  R0.w, image_size.y;
MULR  R0.xy, R0, R0.zwzw;
MULR  R0.zw, R1.xyxy, R0;
MULR  R1.xy, R0.zwzw, texture_ratio;
MOVR  R0.z, {1}.x;
ADDR  R0.z, -transparency.x, R0;
TEX   R1, R1, TEX0, 2D;
MULR  R1, R1, R0.z;
MULR  R0.xy, R0, texture_ratio;
TEX   R0, R0, TEX0, 2D;
MADR  o[COLH], R0, transparency.x, R1;
END
# 20 instructions, 2 R-regs, 0 H-regs
