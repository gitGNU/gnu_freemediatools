!!ARBfp1.0
# cgc version 1.3.0001, build date Jan  7 2005 14:01:35
# command line args: -profile arbfp1
# source file: color_frag_gpu.cg
#vendor NVIDIA Corporation
#version 1.0.02
#profile arbfp1
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
#const c[4] = 255 0.0001000166 0 1
#const c[5] = 4 2 6 3
#const c[6] = 5
PARAM c[7] = { program.local[0..3],
		{ 255, 0.00010001659, 0, 1 },
		{ 4, 2, 6, 3 },
		{ 5 } };
TEMP R0;
TEMP R1;
TEMP R2;
TEMP R3;
TEMP R4;
TEX R1, fragment.texcoord[0], texture[0], 2D;
ADD R1, R1, c[1].x;
ADD R1, R1, c[0];
MUL R1, R1, c[4].x;
ADD R1, -R1, c[4].x;
MIN R0.w, R1.x, R1.y;
MIN R0.w, R0, R1.z;
ADD R1, R1, -R0.w;
MIN R2.y, R1.x, R1;
MAX R2.x, R1, R1.y;
MAX R2.x, R2, R1.z;
MIN R2.y, R2, R1.z;
ADD R2.y, R2.x, -R2;
ADD R2.z, R1.x, -R2.x;
RCP R2.w, R2.y;
ADD R3.x, R1.y, -R1.z;
MUL R3.z, R3.x, R2.w;
ABS R2.z, R2;
CMP R3.x, -R2.z, c[4].z, c[4].w;
SLT R2.z, c[4].y, R2.y;
MUL R3.y, R2.z, R3.x;
CMP R3.z, -R3.y, R3, R0.x;
SLT R3.w, R3.z, c[4].z;
MUL R3.y, R3, R3.w;
ADD R3.w, R1.z, -R1.x;
ADD R1.z, R1, -R2.x;
ADD R1.x, R1, -R1.y;
ADD R4.x, R3.z, c[5].z;
CMP R4.x, -R3.y, R4, R3.z;
ADD R3.y, R1, -R2.x;
ABS R3.x, R3;
CMP R3.x, -R3, c[4].z, c[4].w;
ABS R3.y, R3;
ABS R1.z, R1;
MAD R3.w, R3, R2, c[5].y;
MUL R3.x, R2.z, R3;
CMP R3.y, -R3, c[4].z, c[4].w;
MUL R3.z, R3.x, R3.y;
ABS R3.y, R3;
CMP R3.y, -R3, c[4].z, c[4].w;
CMP R3.z, -R3, R3.w, R4.x;
MUL R3.x, R3, R3.y;
CMP R1.z, -R1, c[4], c[4].w;
MUL R1.y, R3.x, R1.z;
MAD R1.x, R1, R2.w, c[5];
CMP R1.x, -R1.y, R1, R3.z;
RCP R1.z, c[5].z;
MUL R1.y, R1.x, R1.z;
RCP R1.x, R2.x;
ABS R1.z, R2;
MUL R2.y, R2, R1.x;
CMP R1.z, -R1, c[4], c[4].w;
CMP R1.x, -R1.z, c[4].z, R1.y;
CMP R1.y, -R1.z, c[4].z, R2;
ADD R1.z, -c[2].x, c[2].y;
RCP R2.z, R1.z;
ADD R2.y, R2.x, -c[2].x;
ADD R1.z, -c[2].y, c[2].x;
MUL R2.y, R2, R2.z;
ABS R1.z, R1;
CMP R1.z, -R1, R2.y, R2.x;
ADD R1, R1, c[3].xyzz;
ABS R3.z, R1.y;
ABS R2.x, R1.y;
ADD R2.y, R1.x, -c[4].w;
CMP R2.x, -R2, c[4].z, c[4].w;
ABS R2.y, R2;
ABS R2.x, R2;
CMP R0.xyz, -R3.z, R0, R1.z;
CMP R2.x, -R2, c[4].z, c[4].w;
CMP R2.y, -R2, c[4].z, c[4].w;
MUL R2.y, R2.x, R2;
CMP R1.x, -R2.y, c[4].z, R1;
MUL R2.z, R1.x, c[5];
FLR R1.x, R2.z;
ABS R2.y, R1.x;
SLT R1.x, R1, c[4].z;
FLR R2.y, R2;
CMP R2.y, -R1.x, -R2, R2;
ABS R1.x, R2.y;
CMP R2.w, -R1.x, c[4].z, c[4];
ADD R3.x, R2.y, -c[4].w;
ABS R1.x, R2.w;
ABS R3.x, R3;
ADD R4.x, R2.z, -R2.y;
CMP R3.y, -R3.x, c[4].z, c[4].w;
CMP R1.x, -R1, c[4].z, c[4].w;
MUL R3.x, R2, R1;
MUL R2.x, R2, R2.w;
MUL R1.x, R3, R3.y;
ADD R2.w, -R1.y, c[4];
MUL R2.w, R1.z, R2;
CMP R3.z, -R2.x, R2.w, R0;
ABS R3.y, R3;
CMP R0.z, -R3.y, c[4], c[4].w;
MUL R0.z, R3.x, R0;
ADD R3.x, R2.y, -c[5].y;
ABS R2.z, R3.x;
ADD R3.x, -R4, c[4].w;
MAD R3.y, -R1, R3.x, c[4].w;
MUL R3.y, R1.z, R3;
CMP R2.z, -R2, c[4], c[4].w;
MUL R3.x, R0.z, R2.z;
CMP R3.z, -R1.x, R2.w, R3;
CMP R3.w, -R3.x, R3.y, R3.z;
ADD R3.z, R2.y, -c[5].w;
CMP R0.y, -R2.x, R3, R0;
CMP R0.y, -R1.x, R1.z, R0;
ABS R2.z, R2;
ABS R3.z, R3;
CMP R2.z, -R2, c[4], c[4].w;
MUL R2.z, R0, R2;
CMP R3.z, -R3, c[4], c[4].w;
MUL R0.z, R2, R3;
CMP R4.y, -R0.z, R1.z, R3.w;
ADD R3.w, R2.y, -c[5].x;
ABS R3.z, R3;
CMP R3.z, -R3, c[4], c[4].w;
ABS R3.w, R3;
ADD R2.y, R2, -c[6].x;
ABS R2.y, R2;
MUL R2.z, R2, R3;
CMP R3.w, -R3, c[4].z, c[4];
MUL R3.z, R2, R3.w;
ABS R3.w, R3;
CMP R3.w, -R3, c[4].z, c[4];
MUL R2.z, R2, R3.w;
CMP R2.y, -R2, c[4].z, c[4].w;
MAD R3.w, -R1.y, R4.x, c[4];
MUL R1.y, R2.z, R2;
MUL R2.y, R1.z, R3.w;
CMP R0.x, -R2, R1.z, R0;
CMP R0.x, -R1, R2.y, R0;
CMP R0.x, -R3, R2.w, R0;
CMP R0.x, -R0.z, R2.w, R0;
CMP R0.x, -R3.z, R3.y, R0;
CMP R4.y, -R3.z, R1.z, R4;
CMP R0.y, -R3.x, R1.z, R0;
CMP R0.y, -R0.z, R2, R0;
CMP R0.y, -R3.z, R2.w, R0;
CMP R2.z, -R1.y, R2.y, R4.y;
CMP R2.y, -R1, R2.w, R0;
CMP R2.x, -R1.y, R1.z, R0;
MOV R2.w, R1;
ADD R0, R2, R0.w;
MIN R0.z, R0, c[4].x;
MIN R0.y, R0, c[4].x;
MIN R0.x, R0, c[4];
RCP R1.x, c[4].x;
ADD R0, -R0, c[4].x;
MUL result.color, R0, R1.x;
END
# 151 instructions, 5 R-regs
