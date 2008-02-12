!!FP1.0
# cgc version 1.3.0001, build date Jan  7 2005 14:01:35
# command line args: -profile fp30
# source file: fisheye_frag_gpu.cg
#vendor NVIDIA Corporation
#version 1.0.02
#profile fp30
#program main
#semantic main.xcenter : C0
#semantic main.ycenter : C1
#semantic main.fisheye_radius : C2
#semantic main.transparency : C3
#semantic main.gamma : C4
#semantic main.size_ratio : C5
#semantic main.number_of_lenses : C6
#semantic main.texture
#var half2 uv : $vin.TEX0 : TEX0 : 0 : 1
#var half xcenter : C0 : c[0] : 1 : 1
#var half ycenter : C1 : c[1] : 2 : 1
#var half fisheye_radius : C2 : c[2] : 3 : 1
#var half transparency : C3 : c[3] : 4 : 1
#var half gamma : C4 : c[4] : 5 : 1
#var half2 size_ratio : C5 : c[5] : 6 : 1
#var half number_of_lenses : C6 : c[6] : 7 : 1
#var sampler2D texture :  : texunit 0 : 8 : 1
#var half4 main : $vout.COL : COL : -1 : 1
DECLARE transparency;
DECLARE xcenter;
DECLARE ycenter;
DECLARE gamma;
DECLARE fisheye_radius;
DECLARE size_ratio;
DECLARE number_of_lenses;
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
DP4R  R0.z, R1, {0, 3.14159, 1.5708}.xyzz;
SGER  H0.x, R2.y, {-1e-007};
MADR  R0.y, R0, R0.x, {0.999214}.x;
MADR  R0.w, H0.x, {2, -1}.x, {2, -1}.y;
MULR  R0.z, R0.w, R0;
MADR  R0.y, R0, R0.x, R0.z;
COSR  R0.x, R0.y;
MULR  R0.z, R2, R2;
MADR  R0.z, R2.x, R2.x, R0;
SINR  R0.y, R0.y;
RSQR  R0.z, R0.z;
RCPR  R0.z, R0.z;
MOVR  R0.w, {1e-006}.x;
ADDR  R0.w, fisheye_radius.x, R0;
POWR  R0.z, R0.z, gamma.x;
RCPR  R0.w, R0.w;
MULR  R0.z, R0, R0.w;
MADR  R0.y, R0.z, R0, ycenter.x;
MADR  R0.x, R0.z, R0, xcenter;
RCPR  R0.z, size_ratio.x;
RCPR  R0.w, size_ratio.y;
MULR  R0.zw, R0.xyxy, R0;
MOVH  H0.x, {1};
ADDH  H0.x, number_of_lenses, -H0;
SLTR  H0.w, R0, number_of_lenses.x;
SGTR  H0.z, R0.w, -H0.x;
TEX   R1, f[TEX0], TEX0, 2D;
SGTR  H0.x, R0.z, -H0;
SLTR  H0.y, R0.z, number_of_lenses.x;
MULX  H0.x, H0, H0.y;
MULX  H0.x, H0, H0.z;
MOVH  o[COLH].xyz, R1;
MULXC HC.x, H0, H0.w;
MOVR  R0.z, {1}.x;
ADDR  R0.z, -|transparency.x|, R0;
MULR  R1.xyz, R0.z, R1;
TEX   R0, R0, TEX0, 2D;
MADR  o[COLH].xyz(NE.x), R0, |transparency.x|, R1;
MOVH  o[COLH].w, {1}.x;
END
# 66 instructions, 3 R-regs, 1 H-regs
