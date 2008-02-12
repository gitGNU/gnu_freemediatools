!!ARBfp1.0
# cgc version 1.3.0001, build date Jan  7 2005 14:01:35
# command line args: -profile arbfp1
# source file: fisheye_frag_gpu.cg
#vendor NVIDIA Corporation
#version 1.0.02
#profile arbfp1
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
#const c[7] = 1 1e-007 1e-006 0
#const c[8] = -0.03898651 0.1462645 -0.321175 0.9992138
#const c[9] = 2 -1
#const c[10] = 0 3.141593 1.570796
PARAM c[11] = { program.local[0..6],
		{ 1, 1e-007, 1e-006, 0 },
		{ -0.038986512, 0.14626446, -0.32117498, 0.99921381 },
		{ 2, -1 },
		{ 0, 3.1415927, 1.5707964 } };
TEMP R0;
TEMP R1;
TEMP R2;
ADD R2.y, fragment.texcoord[0], -c[1].x;
ADD R0.x, fragment.texcoord[0], -c[0];
ADD R2.x, R0, c[7].y;
ADD R2.z, R2.y, c[7].y;
ADD R0.x, R2, R2.z;
SLT R0.z, R2.x, -R2;
SGE R0.y, R2.x, R2.z;
MUL R1.w, R0.y, R0.z;
SLT R0.y, c[7].w, R0.x;
SGE R0.x, R2.z, R2;
MUL R1.z, R0.x, R0.y;
SLT R0.y, R2.x, R2.z;
SGE R0.x, -R2, R2.z;
MUL R1.y, R0.x, R0;
SLT R0.y, R2.z, R2.x;
SGE R0.x, R2.z, -R2;
MUL R1.x, R0, R0.y;
RCP R0.y, R2.z;
MUL R0.zw, -R2.x, R0.y;
RCP R0.x, R2.x;
MUL R0.xy, R2.z, R0.x;
DP4 R0.x, R0, R1;
DP4 R0.w, R1, c[10].xyzz;
MUL R0.y, R0.x, R0.x;
SGE R0.z, R2.y, -c[7].y;
ABS R1.w, c[3].x;
MAD R0.y, R0, c[8].x, c[8];
MUL R0.y, R0, R0.x;
MAD R0.y, R0, R0.x, c[8].z;
MUL R0.y, R0, R0.x;
MAD R0.z, R0, c[9].x, c[9].y;
MUL R0.z, R0, R0.w;
MAD R0.y, R0, R0.x, c[8].w;
MAD R0.y, R0, R0.x, R0.z;
COS R0.x, R0.y;
MUL R0.z, R2, R2;
MAD R0.z, R2.x, R2.x, R0;
MOV R0.w, c[7].z;
RSQ R0.z, R0.z;
ADD R0.w, c[2].x, R0;
RCP R0.z, R0.z;
SIN R0.y, R0.y;
RCP R0.w, R0.w;
POW R0.z, R0.z, c[4].x;
MUL R0.z, R0, R0.w;
MAD R1.y, R0.z, R0, c[1].x;
MAD R1.x, R0.z, R0, c[0];
MOV R0.z, c[7].x;
ADD R0.z, c[6].x, -R0;
ADD R1.z, -R1.w, c[7].x;
RCP R0.y, c[5].y;
RCP R0.x, c[5].x;
MUL R0.xy, R1, R0;
SLT R0.w, R0.x, c[6].x;
SLT R0.x, -R0.z, R0;
SLT R0.z, -R0, R0.y;
MUL R0.x, R0, R0.w;
MUL R0.x, R0, R0.z;
SLT R0.y, R0, c[6].x;
MUL R0.x, R0, R0.y;
ABS R0.w, R0.x;
TEX R0.xyz, fragment.texcoord[0], texture[0], 2D;
MUL R2.xyz, R1.z, R0;
TEX R1.xyz, R1, texture[0], 2D;
MAD R1.xyz, R1, R1.w, R2;
CMP R0.w, -R0, c[7], c[7].x;
CMP result.color.xyz, -R0.w, R0, R1;
MOV result.color.w, c[7].x;
END
# 68 instructions, 3 R-regs
