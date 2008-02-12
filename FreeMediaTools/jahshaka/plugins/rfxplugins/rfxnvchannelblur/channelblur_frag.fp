!!FP1.0
# cgc version 1.3.0001, build date Jan  7 2005 14:01:35
# command line args: -profile fp30
# source file: channelblur_frag.cg
#vendor NVIDIA Corporation
#version 1.0.02
#profile fp30
#program main
#semantic main.horizontal : C0
#semantic main.vertical : C1
#semantic main.stretch : C2
#semantic main.weight : C3
#semantic main.counter : C4
#semantic main.ratio : C5
#semantic main.transparency : C6
#semantic main.screen_offset : C7
#semantic main.image_size : C8
#semantic main.texture_ratio : C9
#semantic main.texture : TEXUNIT0
#var float2 wpos : $vin.WPOS : WPOS : 0 : 1
#var float horizontal : C0 : c[0] : 1 : 1
#var float vertical : C1 : c[1] : 2 : 1
#var float stretch : C2 : c[2] : 3 : 0
#var float2 weight : C3 : c[3] : 4 : 1
#var float counter : C4 : c[4] : 5 : 1
#var float ratio : C5 : c[5] : 6 : 1
#var float transparency : C6 : c[6] : 7 : 1
#var float2 screen_offset : C7 : c[7] : 8 : 1
#var float2 image_size : C8 : c[8] : 9 : 1
#var float2 texture_ratio : C9 : c[9] : 10 : 1
#var sampler2D texture : TEXUNIT0 : texunit 0 : 11 : 1
#var half4 main : $vout.COL : COL : -1 : 1
DECLARE weight;
DECLARE screen_offset;
DECLARE image_size;
DECLARE texture_ratio;
DECLARE horizontal;
DECLARE ratio;
DECLARE vertical;
DECLARE counter;
DECLARE transparency;
RCPR  R0.x, image_size.x;
RCPR  R0.y, image_size.y;
ADDR  R0.zw, f[WPOS].xyxy, -screen_offset.xyxy;
MULR  R2.xy, R0.zwzw, R0;
MOVR  R0.x, {2};
MULR  R0.x, image_size, R0;
MOVR  R2.w, counter.x;
MULR  R3.z, horizontal.x, R2.w;
RCPR  R3.x, R0.x;
MULR  R0.x, R3.z, R3;
ADDR  R1.x, R2, R0;
SLTR  H0.x, R1, {0.99};
MOVRC RC.x, H0;
MULR  R0.xy, R2, texture_ratio;
TEX   R0, R0, TEX0, 2D;
MOVH  H1.xyz, R0.xyww;
MULR  H1.xyz(GT.x), R0.xyww, weight.x;
MOVRC RC.x, H0;
MOVR  R1.y, R2;
MULR  R1.xy, R1, texture_ratio;
MOVH  H0.xyz, H1;
MOVR  R0.w, {1}.x;
ADDR  R2.z, -weight.x, R0.w;
TEX   R1, R1, TEX0, 2D;
MADR  H0.xyz(GT.x), R1.xyww, R2.z, H1;
MOVR  R1.x, {4};
MULR  R1.x, image_size, R1;
MOVH  H1.xyz, H0;
RCPR  R3.y, R1.x;
MULR  R1.x, R3.z, R3.y;
ADDR  R1.x, R2, -R1;
SGTR  H0.w, R1.x, {0.01}.x;
MOVRC RC.x, H0.w;
MULR  H1.xyz(GT.x), H0, weight.x;
MOVRC RC.x, H0.w;
MOVR  R1.y, R2;
MULR  R1.xy, R1, texture_ratio;
MOVH  H0.xyz, H1;
TEX   R1, R1, TEX0, 2D;
MADR  H0.xyz(GT.x), R1.xyww, R2.z, H1;
MULR  R2.w, vertical.x, R2;
MULR  R1.x, R2.w, R3;
MOVH  H1.xyz, H0;
ADDR  R1.x, R2.y, R1;
SLTR  H0.w, R1.x, {0.99}.x;
MOVRC RC.x, H0.w;
MULR  H1.xyz(GT.x), H0, weight.x;
MOVRC RC.x, H0.w;
MOVR  R1.y, R1.x;
MOVR  R1.x, R2;
MOVH  H0.xyz, H1;
MULR  R1.xy, R1, texture_ratio;
TEX   R1, R1, TEX0, 2D;
MADR  H0.xyz(GT.x), R1.xyww, R2.z, H1;
MULR  R1.x, R2.w, R3.y;
ADDR  R1.x, R2.y, -R1;
SGTR  H0.w, R1.x, {0.01}.x;
MOVRC RC.x, H0.w;
MOVH  H1.xyz, H0;
MULR  H1.xyz(GT.x), H0, weight.x;
MOVRC RC.x, H0.w;
MOVR  R1.y, R1.x;
MOVR  R1.x, R2;
MOVH  H0.xyz, H1;
MULR  R1.xy, R1, texture_ratio;
TEX   R1, R1, TEX0, 2D;
MADR  H0.xyz(GT.x), R1.xyww, R2.z, H1;
ADDR  R0.w, -transparency.x, R0;
MULR  R1.xy, H0, R0.w;
MOVH  o[COLH].w, H0.z;
MADR  o[COLH].xy, R0, transparency.x, R1;
MOVH  o[COLH].z, R0;
END
# 72 instructions, 4 R-regs, 2 H-regs
