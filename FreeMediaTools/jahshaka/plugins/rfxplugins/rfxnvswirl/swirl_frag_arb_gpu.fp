!!ARBfp1.0
# cgc version 1.3.0001, build date Jan  7 2005 14:01:35
# command line args: -profile arbfp1
# source file: swirl_frag_gpu.cg
#vendor NVIDIA Corporation
#version 1.0.02
#profile arbfp1
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
#const c[4] = 1e-007 0 1 -0.03898651
#const c[5] = 0.1462645 -0.321175 0.9992138 2
#const c[6] = -1 0 3.141593 1.570796
PARAM c[7] = { program.local[0..3],
		{ 1e-007, 0, 1, -0.038986512 },
		{ 0.14626446, -0.32117498, 0.99921381, 2 },
		{ -1, 0, 3.1415927, 1.5707964 } };
TEMP R0;
TEMP R1;
TEMP R2;
ADD R2.y, fragment.texcoord[0], -c[1].x;
ADD R0.x, fragment.texcoord[0], -c[0];
ADD R2.x, R0, c[4];
ADD R2.z, R2.y, c[4].x;
ADD R0.x, R2, R2.z;
SLT R0.z, R2.x, -R2;
SGE R0.y, R2.x, R2.z;
MUL R1.w, R0.y, R0.z;
SLT R0.y, c[4], R0.x;
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
DP4 R0.z, R1, c[6].yzww;
MUL R0.y, R0.x, R0.x;
MUL R0.y, R0, c[4].w;
ADD R0.y, R0, c[5].x;
MUL R0.y, R0, R0.x;
MAD R0.y, R0, R0.x, c[5];
MUL R0.y, R0, R0.x;
MUL R0.w, R2.z, R2.z;
SGE R1.x, R2.y, -c[4];
MAD R0.w, R2.x, R2.x, R0;
MUL R1.x, R1, c[5].w;
ADD R1.x, R1, c[6];
MAD R0.y, R0, R0.x, c[5].z;
MUL R0.z, R1.x, R0;
RSQ R0.w, R0.w;
RCP R1.x, R0.w;
MAD R0.y, R0, R0.x, R0.z;
ADD R0.x, -R1, c[2];
MAD R0.x, -R0, c[3], R0.y;
SIN R0.y, R0.x;
COS R0.x, R0.x;
MAD R0.z, R1.x, R0.x, c[0].x;
MAD R0.w, R1.x, R0.y, c[1].x;
SLT R0.x, R1, c[2];
TEX R1.xyz, R0.zwzw, texture[0], 2D;
ABS R0.w, R0.x;
TEX R0.xyz, fragment.texcoord[0], texture[0], 2D;
CMP R0.w, -R0, c[4].y, c[4].z;
CMP result.color.xyz, -R0.w, R0, R1;
MOV result.color.w, c[4].z;
END
# 52 instructions, 3 R-regs
