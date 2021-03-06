!!FP1.0
# cgc version 1.3.0001, build date Aug  4 2004 10:01:10
# command line args: -profile fp30
# source file: blur7_frag_gpu.cg
#vendor NVIDIA Corporation
#version 1.0.02
#profile fp30
#program main
#semantic main.horizontal : C0
#semantic main.vertical : C1
#semantic main.stretch : C2
#semantic main.weight : C3
#semantic main.texture : TEXUNIT0
#var float2 position : $vin.TEX0 : TEX0 : 0 : 1
#var float horizontal : C0 : c[0] : 1 : 1
#var float vertical : C1 : c[1] : 2 : 1
#var float stretch : C2 : c[2] : 3 : 1
#var float weight : C3 : c[3] : 4 : 1
#var sampler2D texture : TEXUNIT0 : texunit 0 : 5 : 1
#var half4 main : $vout.COL : COL : -1 : 1
DECLARE horizontal;
DECLARE vertical;
DECLARE stretch;
DECLARE weight;
MOVR  R2.y, vertical.x;
MOVR  R2.x, horizontal;
SLTR  H0.x, {0}, R2;
SLTR  H2.x, {0}, R2.y;
ADDX_SAT H2.y, H0.x, H2.x;
MULX  H4.y, H2, H0.x;
MOVR  R2.w, f[TEX0].x;
MOVRC RC.x, H4.y;
MULX  H4.x, H2.y, H2;
ADDR  R2.w(GT.x), f[TEX0].x, stretch.x;
MOVRC RC.x, H4.y;
MOVR  R0.x, R2.w;
MOVR  R0.y, f[TEX0];
TEX   R0, R0, TEX0, 2D;
TEX   R1, f[TEX0], TEX0, 2D;
MADR  H1(GT.x), R0, weight.x, R1;
MOVRC RC.x, H4.y;
MOVR  R1.x, f[TEX0];
ADDR  R1.x(GT), f[TEX0], -stretch;
MOVRC RC.x, H4.y;
MOVH  H0, H1;
MOVR  R0.x, R1;
MOVR  R0.y, f[TEX0];
MOVR  R1.y, f[TEX0];
TEX   R0, R0, TEX0, 2D;
MADR  H0(GT.x), R0, weight.x, H1;
MOVRC RC.x, H4;
ADDR  R1.y(GT.x), f[TEX0], stretch.x;
MOVRC RC.x, H4;
MOVH  H1, H0;
MOVR  R0.y, R1;
MOVR  R0.x, f[TEX0];
MOVR  R2.z, f[TEX0].y;
TEX   R0, R0, TEX0, 2D;
MADR  H1(GT.x), R0, weight.x, H0;
MOVRC RC.x, H4;
ADDR  R2.z(GT.x), f[TEX0].y, -stretch.x;
MOVRC RC.x, H4;
MOVH  H0, H1;
MOVR  R0.y, R2.z;
MOVR  R0.x, f[TEX0];
MOVR  R1.w, R2;
TEX   R0, R0, TEX0, 2D;
MADR  H0(GT.x), R0, weight.x, H1;
SLTR  H1.x, {1}, R2;
SLTR  H2.x, {1}, R2.y;
ADDX_SAT H2.y, H1.x, H2.x;
MULX  H3.w, H2.y, H1.x;
MULX  H3.z, H2.y, H2.x;
MOVH  H1, H0;
MOVRC RC.x, H3.w;
ADDR  R1.w(GT.x), R2, stretch.x;
MOVRC RC.x, H3.w;
MOVR  R0.x, R1.w;
MOVR  R0.y, f[TEX0];
MOVR  R1.z, R1.x;
TEX   R0, R0, TEX0, 2D;
MADR  H1(GT.x), R0, weight.x, H0;
MOVRC RC.x, H3.w;
ADDR  R1.z(GT.x), R1.x, -stretch.x;
MOVRC RC.x, H3.w;
MOVH  H0, H1;
MOVR  R0.x, R1.z;
MOVR  R0.y, f[TEX0];
MOVR  R1.x, R1.y;
TEX   R0, R0, TEX0, 2D;
MADR  H0(GT.x), R0, weight.x, H1;
MOVRC RC.x, H3.z;
ADDR  R1.x(GT), R1.y, stretch;
MOVRC RC.x, H3.z;
MOVH  H1, H0;
MOVR  R0.y, R1.x;
MOVR  R0.x, f[TEX0];
MOVR  R1.y, R2.z;
TEX   R0, R0, TEX0, 2D;
MADR  H1(GT.x), R0, weight.x, H0;
MOVRC RC.x, H3.z;
ADDR  R1.y(GT.x), R2.z, -stretch.x;
MOVRC RC.x, H3.z;
MOVH  H0, H1;
MOVR  R0.y, R1;
MOVR  R0.x, f[TEX0];
MOVR  R2.z, R1.w;
TEX   R0, R0, TEX0, 2D;
MADR  H0(GT.x), R0, weight.x, H1;
SLTR  H1.x, {2}, R2;
SLTR  H2.x, {2}, R2.y;
ADDX_SAT H2.y, H1.x, H2.x;
MULX  H3.y, H2, H1.x;
MOVH  H1, H0;
MOVRC RC.x, H3.y;
MULX  H3.x, H2.y, H2;
ADDR  R2.z(GT.x), R1.w, stretch.x;
MOVRC RC.x, H3.y;
MOVR  R0.x, R2.z;
MOVR  R0.y, f[TEX0];
MOVR  R1.w, R1.z;
TEX   R0, R0, TEX0, 2D;
MADR  H1(GT.x), R0, weight.x, H0;
MOVRC RC.x, H3.y;
ADDR  R1.w(GT.x), R1.z, -stretch.x;
MOVRC RC.x, H3.y;
MOVH  H0, H1;
MOVR  R0.x, R1.w;
MOVR  R0.y, f[TEX0];
MOVR  R1.z, R1.x;
TEX   R0, R0, TEX0, 2D;
MADR  H0(GT.x), R0, weight.x, H1;
MOVRC RC.x, H3;
ADDR  R1.z(GT.x), R1.x, stretch.x;
MOVRC RC.x, H3;
MOVH  H1, H0;
MOVR  R0.y, R1.z;
MOVR  R0.x, f[TEX0];
MOVR  R1.x, R1.y;
TEX   R0, R0, TEX0, 2D;
MADR  H1(GT.x), R0, weight.x, H0;
MOVRC RC.x, H3;
ADDR  R1.x(GT), R1.y, -stretch;
MOVRC RC.x, H3;
MOVH  H0, H1;
MOVR  R0.y, R1.x;
MOVR  R0.x, f[TEX0];
MOVR  R1.y, R2.z;
TEX   R0, R0, TEX0, 2D;
MADR  H0(GT.x), R0, weight.x, H1;
SLTR  H1.x, {3}, R2;
SLTR  H2.x, {3}, R2.y;
ADDX_SAT H2.y, H1.x, H2.x;
MULX  H2.w, H2.y, H1.x;
MULX  H2.z, H2.y, H2.x;
MOVH  H1, H0;
MOVRC RC.x, H2.w;
ADDR  R1.y(GT.x), R2.z, stretch.x;
MOVRC RC.x, H2.w;
MOVR  R0.x, R1.y;
MOVR  R0.y, f[TEX0];
MOVR  R2.z, R1.w;
TEX   R0, R0, TEX0, 2D;
MADR  H1(GT.x), R0, weight.x, H0;
MOVRC RC.x, H2.w;
ADDR  R2.z(GT.x), R1.w, -stretch.x;
MOVRC RC.x, H2.w;
MOVH  H0, H1;
MOVR  R0.x, R2.z;
MOVR  R0.y, f[TEX0];
MOVR  R1.w, R1.z;
TEX   R0, R0, TEX0, 2D;
MADR  H0(GT.x), R0, weight.x, H1;
MOVRC RC.x, H2.z;
ADDR  R1.w(GT.x), R1.z, stretch.x;
MOVRC RC.x, H2.z;
MOVH  H1, H0;
MOVR  R0.y, R1.w;
MOVR  R0.x, f[TEX0];
MOVR  R1.z, R1.x;
TEX   R0, R0, TEX0, 2D;
MADR  H1(GT.x), R0, weight.x, H0;
MOVRC RC.x, H2.z;
ADDR  R1.z(GT.x), R1.x, -stretch.x;
MOVRC RC.x, H2.z;
MOVH  H0, H1;
MOVR  R0.y, R1.z;
MOVR  R0.x, f[TEX0];
MOVR  R1.x, R1.y;
TEX   R0, R0, TEX0, 2D;
MADR  H0(GT.x), R0, weight.x, H1;
SLTR  H1.x, {4}, R2;
SLTR  H2.x, {4}, R2.y;
ADDX_SAT H4.z, H1.x, H2.x;
MULX  H2.y, H4.z, H1.x;
MOVH  H1, H0;
MOVRC RC.x, H2.y;
MULX  H2.x, H4.z, H2;
ADDR  R1.x(GT), R1.y, stretch;
MOVRC RC.x, H2.y;
MOVR  R0.x, R1;
MOVR  R0.y, f[TEX0];
MOVR  R1.y, R2.z;
TEX   R0, R0, TEX0, 2D;
MADR  H1(GT.x), R0, weight.x, H0;
MOVRC RC.x, H2.y;
ADDR  R1.y(GT.x), R2.z, -stretch.x;
MOVRC RC.x, H2.y;
MOVH  H0, H1;
MOVR  R0.x, R1.y;
MOVR  R0.y, f[TEX0];
MOVR  R2.z, R1.w;
TEX   R0, R0, TEX0, 2D;
MADR  H0(GT.x), R0, weight.x, H1;
MOVRC RC.x, H2;
ADDR  R2.z(GT.x), R1.w, stretch.x;
MOVRC RC.x, H2;
MOVH  H1, H0;
MOVR  R0.y, R2.z;
MOVR  R0.x, f[TEX0];
MOVR  R1.w, R1.z;
TEX   R0, R0, TEX0, 2D;
MADR  H1(GT.x), R0, weight.x, H0;
MOVRC RC.x, H2;
ADDR  R1.w(GT.x), R1.z, -stretch.x;
MOVRC RC.x, H2;
MOVH  H0, H1;
MOVR  R0.y, R1.w;
MOVR  R0.x, f[TEX0];
MOVR  R1.z, R1.x;
TEX   R0, R0, TEX0, 2D;
MADR  H0(GT.x), R0, weight.x, H1;
SLTR  H4.w, {5}.x, R2.y;
SLTR  H1.x, {5}, R2;
ADDX_SAT H5.x, H1, H4.w;
MULX  H4.z, H5.x, H1.x;
MULX  H4.w, H5.x, H4;
MOVH  H1, H0;
MOVRC RC.x, H4.z;
ADDR  R1.z(GT.x), R1.x, stretch.x;
MOVRC RC.x, H4.z;
MOVR  R1.x, f[TEX0].y;
TEX   R0, R1.zxzw, TEX0, 2D;
MADR  H1(GT.x), R0, weight.x, H0;
MOVRC RC.x, H4.z;
MOVR  R1.x, R1.y;
ADDR  R1.x(GT), R1.y, -stretch;
MOVRC RC.x, H4.z;
MOVR  R1.y, f[TEX0];
MOVH  H0, H1;
TEX   R0, R1, TEX0, 2D;
MADR  H0(GT.x), R0, weight.x, H1;
MOVRC RC.x, H4.w;
MOVR  R1.y, R2.z;
ADDR  R1.y(GT.x), R2.z, stretch.x;
MOVRC RC.x, H4.w;
MOVH  H1, H0;
MOVR  R0.y, R1;
MOVR  R0.x, f[TEX0];
MOVR  R2.z, R1.w;
TEX   R0, R0, TEX0, 2D;
MADR  H1(GT.x), R0, weight.x, H0;
MOVRC RC.x, H4.w;
ADDR  R2.z(GT.x), R1.w, -stretch.x;
MOVRC RC.x, H4.w;
MOVH  H0, H1;
MOVR  R0.y, R2.z;
MOVR  R0.x, f[TEX0];
TEX   R0, R0, TEX0, 2D;
MADR  H0(GT.x), R0, weight.x, H1;
MOVH  H1, H0;
ADDR  R0.x, R1.z, stretch;
MOVR  R0.y, f[TEX0];
SLTR  H5.x, {6}, R2;
SLTR  H5.y, {6}.x, R2;
ADDX_SAT H5.z, H5.x, H5.y;
MULX  H5.x, H5.z, H5;
TEX   R0, R0, TEX0, 2D;
MOVRC RC.x, H5;
MULX  H5.y, H5.z, H5;
MADR  H1(GT.x), R0, weight.x, H0;
MOVRC RC.x, H5;
MOVH  H0, H1;
ADDR  R0.x, R1, -stretch;
MOVR  R0.y, f[TEX0];
TEX   R0, R0, TEX0, 2D;
MADR  H0(GT.x), R0, weight.x, H1;
MOVRC RC.x, H5.y;
MOVH  H1, H0;
ADDR  R0.y, R1, stretch.x;
MOVR  R0.x, f[TEX0];
TEX   R0, R0, TEX0, 2D;
MADR  H1(GT.x), R0, weight.x, H0;
MOVRC RC.x, H5.y;
MOVH  H0, H1;
ADDR  R0.y, R2.z, -stretch.x;
MOVR  R0.x, f[TEX0];
TEX   R0, R0, TEX0, 2D;
MADR  H0(GT.x), R0, weight.x, H1;
MOVRC RC.x, H4.y;
MOVR  R0.y, {1}.x;
MOVR  R0.y(LE.x), {0}.x;
MOVRC RC.x, H4;
MOVR  R0.x, R0.y;
ADDR  R0.x(GT), R0.y, {1};
MOVRC RC.x, H3.w;
MOVR  R0.y, R0.x;
ADDR  R0.y(GT.x), R0.x, {1}.x;
MOVRC RC.x, H3.z;
MOVR  R0.x, R0.y;
ADDR  R0.x(GT), R0.y, {1};
MOVRC RC.x, H3.y;
MOVR  R0.y, R0.x;
ADDR  R0.y(GT.x), R0.x, {1}.x;
MOVRC RC.x, H3;
MOVR  R0.x, R0.y;
ADDR  R0.x(GT), R0.y, {1};
MOVRC RC.x, H2.w;
MOVR  R0.y, R0.x;
ADDR  R0.y(GT.x), R0.x, {1}.x;
MOVRC RC.x, H2.z;
MOVR  R0.x, R0.y;
ADDR  R0.x(GT), R0.y, {1};
MOVRC RC.x, H2.y;
MOVR  R0.y, R0.x;
ADDR  R0.y(GT.x), R0.x, {1}.x;
MOVRC RC.x, H2;
MOVR  R0.x, R0.y;
ADDR  R0.x(GT), R0.y, {1};
MOVRC RC.x, H4.z;
MOVR  R0.y, R0.x;
ADDR  R0.y(GT.x), R0.x, {1}.x;
MOVRC RC.x, H4.w;
MOVR  R0.x, R0.y;
ADDR  R0.x(GT), R0.y, {1};
MOVRC RC.x, H5;
MOVR  R0.y, R0.x;
ADDR  R0.y(GT.x), R0.x, {1}.x;
MOVRC RC.x, H5.y;
MOVR  R0.x, R0.y;
ADDR  R0.x(GT), R0.y, {1};
MULR  R0.y, R0.x, weight.x;
MADR  R0.y, R0, {2, 1}.x, {2, 1};
MOVH  o[COLH], H0;
RCPR  R0.y, R0.y;
MOVRC RC.x, R0;
MULR  o[COLH](GT.x), H0, R0.y;
END
# 323 instructions, 3 R-regs, 6 H-regs
