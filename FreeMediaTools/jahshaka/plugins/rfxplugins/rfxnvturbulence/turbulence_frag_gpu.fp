!!FP1.0
# NV_fragment_program generated by NVIDIA Cg compiler
# cgc version 1.2.1001, build date Mar 17 2004  10:32:28
# command line args: -profile fp30
#vendor NVIDIA Corporation
#version 1.0.02
#profile fp30
#program main
#semantic main.texture : TEXUNIT0
#var sampler2D texture : TEXUNIT0 : texunit 0 : 3 : 1
#var float2 position : $vin.TEXCOORD0 : TEX0 : 0 : 1
#var float2 undistorted_position : $vin.TEXCOORD1 : TEX1 : 1 : 1
#var float4 ColorIn : $vin.COLOR0 : COL0 : 2 : 1
#var half4 main : $vout.COLOR : COL : -1 : 1
MOVR o[COLH], f[COL0];
END
# 1 instructions, 0 R-regs, 0 H-regs.
# End of program
