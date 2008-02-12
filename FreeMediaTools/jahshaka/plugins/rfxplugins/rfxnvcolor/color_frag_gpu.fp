!!FP1.0
# cgc version 1.3.0001, build date Jan  7 2005 14:01:35
# command line args: -profile fp30
# source file: color_frag_gpu.cg
#vendor NVIDIA Corporation
#version 1.0.02
#profile fp30
#program main
#semantic main.color_scale : C0
#semantic main.brightness : C1
#semantic main.enhance : C2
#semantic main.hsv : C3
#semantic main.texture
#var float2 position : $vin.TEX0 : TEX0 : 0 : 1
#var half4 color_scale : C0 : c[0] : 1 : 1
#var half brightness : C1 : c[1] : 2 : 1
#var half2 enhance : C2 : c[2] : 3 : 1
#var half3 hsv : C3 : c[3] : 4 : 1
#var sampler2D texture :  : texunit 0 : 5 : 1
#var half4 main : $vout.COL : COL : -1 : 1
DECLARE brightness;
DECLARE color_scale;
DECLARE enhance;
DECLARE hsv;
TEX   R0, f[TEX0], TEX0, 2D;
ADDR  R0, R0, brightness.x;
ADDR  R0, R0, color_scale;
MULR  H1, R0, {255}.x;
ADDH  H1, -H1, {255}.x;
MINH  H0.w, H1.x, H1.y;
MINH  H0.w, H0, H1.z;
ADDH  H1, H1, -H0.w;
MAXH  H2.x, H1, H1.y;
MAXH  H2.x, H2, H1.z;
MINH  H2.y, H1.x, H1;
MINH  H2.y, H2, H1.z;
ADDH  H2.y, H2.x, -H2;
RCPH  H3.y, H2.y;
SGTH  H2.z, H2.y, {0.000100017}.x;
SEQH  H3.x, H1, H2;
MULX  H3.w, H2.z, H3.x;
SEQX  H3.x, H3, {0};
MOVRC RC.x, H3.w;
ADDH  H2.w, H1.y, -H1.z;
MULX  H3.x, H2.z, H3;
MOVH  H3.z, H0.x;
MULH  H3.z(GT.x), H2.w, H3.y;
SLTH  H2.w, H3.z, {0}.x;
MULXC HC.x, H3.w, H2.w;
MOVH  H2.w, H3.z;
ADDH  H3.w, H1.z, -H1.x;
ADDH  H2.w(GT.x), H3.z, {6}.x;
SEQH  H3.z, H1.y, H2.x;
MULXC HC.x, H3, H3.z;
ADDH  H1.x, H1, -H1.y;
SEQX  H1.y, H3.z, {0}.x;
MULX  H1.y, H3.x, H1;
MADH  H2.w(GT.x), H3, H3.y, {2}.x;
SEQH  H1.z, H1, H2.x;
MULXC HC.x, H1.y, H1.z;
MADH  H2.w(GT.x), H1.x, H3.y, {4}.x;
MOVRC RC.x, H2.z;
RCPH  H1.y, {6}.x;
MOVH  H1.x, {0};
MULH  H1.x(NE), H2.w, H1.y;
ADDH  H2.w, -enhance.x, enhance.y;
ADDH  H1.z, H2.x, -enhance.x;
MOVH  H1.y, H2.x;
RCPH  H2.w, H2.w;
SNEHC HC.x, enhance, enhance.y;
MULH  H1.y(GT.x), H1.z, H2.w;
MOVRC RC.x, H2.z;
RCPH  H1.z, H2.x;
MULH  H2.x, H2.y, H1.z;
MOVH  H2.x(EQ), {0};
MOVH  H1.z, H1.y;
MOVH  H1.y, H2.x;
ADDH  H1, H1, hsv.xyzz;
MOVH  H2.y, H1.x;
SEQH  H2.z, H1.x, {1}.x;
SEQH  H2.x, H2, -hsv.y;
SEQX  H1.x, H2, {0};
MULXC HC.x, H1, H2.z;
MOVH  H2.y(GT.x), {0}.x;
MULH  H2.w, H2.y, {6}.x;
FLRH  H2.y, H2.w;
MOVRC RC.x, H2.y;
FLRR  R0.y, |H2|;
MOVR  R0.x, R0.y;
MOVR  R0.x(LT), -R0.y;
MOVRC RC.x, H2;
MOVH  H2.xyz, H1.z;
MOVH  H2.xyz(LE.x), H0;
ADDH  H0.z, H2.w, -R0.x;
ADDR  R0.y, -H0.z, {1}.x;
MADR  R0.y, -H1, R0, {1}.x;
MULR  H0.y, H1.z, R0;
SEQR  H2.w, R0.x, {0}.x;
MULX  H3.y, H1.x, H2.w;
MOVRC RC.x, H3.y;
MOVH  H0.x, H2.y;
SEQX  H2.y, H2.w, {0}.x;
MULX  H1.x, H1, H2.y;
SEQR  H2.y, R0.x, {1}.x;
MULX  H3.x, H1, H2.y;
SEQX  H2.y, H2, {0}.x;
MOVH  H0.x(GT), H0.y;
MOVRC RC.x, H3;
MULX  H1.x, H1, H2.y;
MADR  R0.y, -H1, H0.z, {1}.x;
SEQR  H2.y, R0.x, {2}.x;
MULX  H2.w, H1.x, H2.y;
SEQX  H2.y, H2, {0}.x;
MULX  H1.x, H1, H2.y;
MOVH  H0.x(GT), H1.z;
MOVRC RC.x, H2.w;
MOVH  H0.x(GT), H1.z;
SEQR  H3.z, R0.x, {3}.x;
MULX  H2.y, H1.x, H3.z;
MOVRC RC.x, H2.y;
MULR  H0.z, H1, R0.y;
SEQX  H3.z, H3, {0}.x;
MULX  H1.x, H1, H3.z;
ADDH  H1.y, -H1, {1}.x;
MOVH  H0.x(GT), H0.z;
SEQR  H3.z, R0.x, {4}.x;
MULH  H3.w, H1.z, H1.y;
MULX  H1.y, H1.x, H3.z;
MOVRC RC.x, H1.y;
MOVH  H0.x(GT), H3.w;
SEQX  H3.z, H3, {0}.x;
MULX  H1.x, H1, H3.z;
SEQR  H3.z, R0.x, {5}.x;
MULX  H3.z, H1.x, H3;
MOVRC RC.x, H3.z;
MOVH  H0.x(GT), H3.w;
MOVRC RC.x, H3.y;
MOVH  H1.x, H2;
MOVH  H1.x(GT), H1.z;
MOVRC RC.x, H3;
MOVH  H1.x(GT), H0.z;
MOVRC RC.x, H2.w;
MOVH  H1.x(GT), H3.w;
MOVRC RC.x, H2.y;
MOVH  H1.x(GT), H3.w;
MOVRC RC.x, H3.y;
MOVH  H2.x, H2.z;
MOVH  H2.x(GT), H3.w;
MOVRC RC.x, H3;
MOVH  H2.x(GT), H3.w;
MOVRC RC.x, H2.w;
MOVH  H2.x(GT), H0.y;
MOVRC RC.x, H1.y;
MOVH  H1.x(GT), H0.y;
MOVRC RC.x, H3.z;
MOVH  H1.x(GT), H1.z;
MOVRC RC.x, H2.y;
MOVH  H2.x(GT), H1.z;
MOVRC RC.x, H1.y;
MOVH  H2.x(GT), H1.z;
MOVRC RC.x, H3.z;
MOVH  H2.x(GT), H0.z;
MOVH  H1.z, H2.x;
MOVH  H1.y, H0.x;
ADDH  H0, H1, H0.w;
MINH  H0.xyz, H0, {255}.x;
ADDH  H0, -H0, {255}.x;
RCPH  H1.x, {255}.x;
MULH  o[COLH], H0, H1.x;
END
# 145 instructions, 1 R-regs, 4 H-regs
