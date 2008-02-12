!!ARBfp1.0
# cgc version 1.3.0001, build date Jan  7 2005 14:01:35
# command line args: -profile arbfp1
# source file: mosaic_frag.cg
#vendor NVIDIA Corporation
#version 1.0.02
#profile arbfp1
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
#const c[5] = 2 1
PARAM c[6] = { program.local[0..4],
		{ 2, 1 } };
TEMP R0;
TEMP R1;
TEMP R2;
ADD R1.xy, fragment.position, -c[2];
RCP R0.w, c[3].y;
RCP R0.z, c[3].x;
MUL R1.zw, R1.xyxy, R0;
RCP R2.x, c[5].x;
RCP R0.y, c[0].y;
RCP R0.x, c[0].x;
MUL R0.xy, R1, R0;
FLR R0.xy, R0;
MUL R1.xy, R2.x, c[0];
MAD R1.xy, R0, c[0], R1;
MUL R0.zw, R1.xyxy, R0;
MUL R0.xy, R1.zwzw, c[4];
MUL R1.xy, R0.zwzw, c[4];
MOV R1.z, c[5].y;
ADD R0.z, -c[1].x, R1;
TEX R1, R1, texture[0], 2D;
MUL R1, R1, R0.z;
TEX R0, R0, texture[0], 2D;
MAD result.color, R0, c[1].x, R1;
END
# 20 instructions, 3 R-regs
