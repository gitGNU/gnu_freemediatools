!!ARBfp1.0

TEMP value, one, two;

TEX value, fragment.texcoord[0], texture[0], 2D;
TEX one, value.x, texture[1], 1D;
TEX two, value.z, texture[2], 1D;
ADD result.color, one, two;

END
