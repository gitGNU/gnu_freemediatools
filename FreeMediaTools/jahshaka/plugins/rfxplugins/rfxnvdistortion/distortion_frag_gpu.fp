!!FP1.0
# NV_fragment_program generated by NVIDIA Cg compiler
# cgc version 1.2.1001, build date Mar 17 2004  10:32:28
# command line args: -profile fp30
#vendor NVIDIA Corporation
#version 1.0.02
#profile fp30
#program main
#semantic main.texture
#var sampler2D texture :  : texunit 0 : 4 : 1
#var float4 wpos : $vin.WPOS : WPOS : 0 : 1
#var float2 position : $vin.TEXCOORD0 : TEX0 : 1 : 1
#var float4 noise_position : $vin.TEXCOORD1 : TEX1 : 2 : 1
#var float4 ColorIn : $vin.COLOR0 : COL0 : 3 : 1
#var half4 main : $vout.COLOR : COL : -1 : 1
TEX o[COLH], f[TEX0].xyxx, TEX0, 2D;
END
# 1 instructions, 0 R-regs, 0 H-regs.
# End of program
