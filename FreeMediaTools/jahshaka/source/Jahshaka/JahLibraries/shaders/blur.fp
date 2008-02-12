!!ARBfp1.0

PARAM param = { 2.0, -1.0, 0.0, 0.0 };
PARAM offset = program.env[0];

TEMP src, coord0, coord1, old, new, temp;

# source texture
TEX src, fragment.texcoord[0], texture[0], 2D;

# calculate offset
ADD coord0, fragment.texcoord[0], offset;
MUL coord0, coord0, 10;

TEX coord0, coord0, texture[2], 2D;
MAD coord0, coord0, param.x, param.y;

TEX coord1, fragment.texcoord[0], texture[1], 2D;
MAD coord1, coord1, param.x, param.y;

MAD coord0, coord0, 0.02, fragment.texcoord[0];
MAD coord1, coord1, 0.02, coord0;

# neighbor point
TEX new, coord1, texture[3], 2D;

# current point
TEX old, fragment.texcoord[0], texture[3], 2D;

# blured it
MAD old, new, 1, old;
MUL new, old, 0.492;

# source replace
SGE temp, src, 0.3;
MUL src, src, temp;
SUB temp, 1, temp;

MUL new, new, temp;

ADD result.color, new, src;

END
