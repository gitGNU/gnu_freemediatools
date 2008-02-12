!!ARBfp1.0
# cgc version 1.3.0001, build date Jan  7 2005 14:01:35
# command line args: -profile arbfp1
# source file: channelblur_frag.cg
#vendor NVIDIA Corporation
#version 1.0.02
#profile arbfp1
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
#const c[10] = 4 0.01 1 2
#const c[11] = 0.99
PARAM c[12] = { program.local[0..9],
		{ 4, 0.0099999998, 1, 2 },
		{ 0.99000001 } };
TEMP R0;
TEMP R1;
TEMP R2;
TEMP R3;
TEMP R4;
TEMP R5;
MOV R4.x, c[4];
MUL R2.w, c[0].x, R4.x;
MOV R1.x, c[10].w;
MUL R1.x, c[8], R1;
RCP R4.y, R1.x;
MUL R1.x, R2.w, R4.y;
ADD R0.zw, fragment.position.xyxy, -c[7].xyxy;
RCP R0.y, c[8].y;
RCP R0.x, c[8].x;
MUL R3.xy, R0.zwzw, R0;
ADD R2.x, R3, R1;
MUL R0.xy, R3, c[9];
TEX R0, R0, texture[0], 2D;
MOV R2.y, R3;
MUL R3.zw, R2.xyxy, c[9].xyxy;
MUL R1.xyz, R0.xyww, c[3].x;
ADD R4.w, R2.x, -c[11].x;
CMP R2.xyz, R4.w, R1, R0.xyww;
TEX R1.xyw, R3.zwzw, texture[0], 2D;
MOV R0.w, c[10].z;
ADD R4.z, -c[3].x, R0.w;
MAD R1.xyz, R1.xyww, R4.z, R2;
CMP R1.xyz, R4.w, R1, R2;
MOV R3.z, c[10].x;
MUL R1.w, c[8].x, R3.z;
RCP R1.w, R1.w;
MUL R2.w, R2, R1;
ADD R5.x, R3, -R2.w;
MOV R5.y, R3;
MUL R4.x, c[1], R4;
MUL R1.w, R4.x, R1;
MUL R2.xyz, R1, c[3].x;
ADD R4.w, R5.x, -c[10].y;
CMP R1.xyz, -R4.w, R2, R1;
MUL R3.zw, R5.xyxy, c[9].xyxy;
TEX R2.xyw, R3.zwzw, texture[0], 2D;
MAD R2.xyz, R2.xyww, R4.z, R1;
CMP R1.xyz, -R4.w, R2, R1;
MUL R2.w, R4.x, R4.y;
ADD R2.w, R3.y, R2;
ADD R1.w, R3.y, -R1;
MOV R3.y, R1.w;
MOV R3.z, R3.x;
MOV R3.w, R2;
MUL R2.xyz, R1, c[3].x;
ADD R4.y, R2.w, -c[11].x;
CMP R1.xyz, R4.y, R2, R1;
MUL R3.zw, R3, c[9].xyxy;
TEX R2.xyw, R3.zwzw, texture[0], 2D;
MAD R2.xyz, R2.xyww, R4.z, R1;
CMP R1.xyz, R4.y, R2, R1;
MUL R2.xyz, R1, c[3].x;
ADD R1.w, R1, -c[10].y;
CMP R1.xyz, -R1.w, R2, R1;
MUL R3.xy, R3, c[9];
TEX R2.xyw, R3, texture[0], 2D;
MAD R2.xyz, R2.xyww, R4.z, R1;
CMP R1.xyz, -R1.w, R2, R1;
ADD R0.w, -c[6].x, R0;
MUL R1.xy, R1, R0.w;
MAD result.color.xy, R0, c[6].x, R1;
MOV result.color.w, R1.z;
MOV result.color.z, R0;
END
# 63 instructions, 6 R-regs
