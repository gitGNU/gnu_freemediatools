!!FP1.0
# cgc version 1.3.0001, build date Jan  7 2005 14:01:35
# command line args: -profile fp30
# source file: swirl_frag_gpu.cg
#vendor NVIDIA Corporation
#version 1.0.02
#profile fp30
#program main
#semantic main.xcenter : C0
#semantic main.ycenter : C1
#semantic main.swirl_radius : C2
#semantic main.twist_effect : C3
#semantic main.texture
#var float2 uv : $vin.TEX0 : TEX0 : 0 : 1
#var float xcenter : C0 : c[0] : 1 : 1
#var float ycenter : C1 : c[1] : 2 : 1
#var float swirl_radius : C2 : c[2] : 3 : 1
#var float twist_effect : C3 : c[3] : 4 : 1
#var sampler2D texture :  : texunit 0 : 5 : 1
#var half4 main : $vout.COL : COL : -1 : 1
DECLARE xcenter;
DECLARE ycenter;
DECLARE swirl_radius;
DECLARE twist_effect;
ADDR  R0.x, f[TEX0], -xcenter;
ADDR  R2.y, f[TEX0], -ycenter.x;
ADDR  R2.x, R0, {1e-007};
ADDR  R2.z, R2.y, {1e-007}.x;
SLER  H0.x, R2.z, R2;
SLTR  H0.y, R2.x, -R2.z;
MULR  R1.w, H0.x, H0.y;
ADDR  R0.x, R2, R2.z;
SLTR  H0.y, {0}.x, R0.x;
SLER  H0.x, R2, R2.z;
MULR  R1.z, H0.x, H0.y;
SLER  H0.x, R2.z, -R2;
SLTR  H0.y, R2.x, R2.z;
MULR  R1.y, H0.x, H0;
SLER  H0.x, -R2, R2.z;
SLTR  H0.y, R2.z, R2.x;
MULR  R1.x, H0, H0.y;
RCPR  R0.x, R2.z;
MULR  R0.zw, -R2.x, R0.x;
RCPR  R0.x, R2.x;
MULR  R0.xy, R2.z, R0.x;
DP4R  R0.x, R0, R1;
MULR  R0.y, R0.x, R0.x;
MADR  R0.y, R0, {-0.0389865, 0.146264}.x, {-0.0389865, 0.146264};
MULR  R0.y, R0, R0.x;
MADR  R0.y, R0, R0.x, {-0.321175}.x;
MULR  R0.y, R0, R0.x;
SGER  H0.x, R2.y, {-1e-007};
MADR  R0.y, R0, R0.x, {0.999214}.x;
DP4R  R0.z, R1, {0, 3.14159, 1.5708}.xyzz;
MADR  R0.w, H0.x, {2, -1}.x, {2, -1}.y;
MULR  R0.z, R0.w, R0;
MADR  R0.x, R0.y, R0, R0.z;
MULR  R0.y, R2.z, R2.z;
MADR  R0.y, R2.x, R2.x, R0;
RSQR  R0.y, R0.y;
RCPR  R0.y, R0.y;
ADDR  R0.z, -R0.y, swirl_radius.x;
MADR  R0.z, -R0, twist_effect.x, R0.x;
COSR  R0.x, R0.z;
SINR  R0.z, R0.z;
MADR  R1.y, R0, R0.z, ycenter.x;
MADR  R1.x, R0.y, R0, xcenter;
TEX   o[COLH], R1, TEX0, 2D;
SLTRC HC.x, R0.y, swirl_radius;
TEX   o[COLH].xyz(EQ.x), f[TEX0], TEX0, 2D;
MOVH  o[COLH].w, {1}.x;
END
# 47 instructions, 3 R-regs, 1 H-regs
