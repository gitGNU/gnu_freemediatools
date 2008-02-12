#ifndef RenderConfig_h
#define RenderConfig_h

//////////////////////////////////////////////////////////////////////
// the list of encoders

enum JahEncoder { NONE, JAHMPEG, LINAVI, WINAVI, THEORA, QUICKTIME, DIVX, PNGENC, NumEncoders };


///////////////////////////////////////////////////////////////////////
//the render config object
//currently a mess since it holds data for all supported encodins defined in 
//the JahEncoder 

typedef struct RenderConfig
{
	
	//mpeg encoder needs these values here sent to it
const char * SourceFilename; //"AnimationClip4_"
const char * SourceDirectory;
const char * SourceExtension;
const char * FramePadding;   //"0001"
int          FramePadval;   //"0001"

const char * DestFilename;
const char * DestDirectory;

const char * mpeg_tplref;
const char * mpeg_iqname;
const char * mpeg_niqname;
const char * mpeg_statname;

int startFrame;
int endFrame;

float        conversionratio;

int          mpegPicFormat;      //2
int          mpegNumberFrames;   //20
int          mpegFirstFrame;     //1
const char * mpegStartTimecode;  //"00:00:00:00"
int          mpegGopFrames;      //12
int          mpegIPframeDist;    //3
int          mpegISOstream;      //1
int          mpegFieldFrameSwitch; //0
int          mpegXres;             //720
int          mpegYres;             //486
int          mpegAspectRatio;      //8
int          mpegFrameRate;        //3
float        mpegBitRate;          //3000000.0
int          mpegVdvBufferSize;    //20
int          mpegLowDelay;         //0
int          mpegConstParamFlag;   //1
int          mpegProfileID;        //4
int          mpegLevelID;          //8
int          mpegProgessiveSeq;    //1
int          mpegChromaFormat;     //1
int          mpegVideoFormat;      //1
int          mpegColorPrimaries;   //5
int          mpegTransferCharac;   //5
int          mpegMatricCoeff;      //5
int          mpegDisplayX;         //352
int          mpegDisplayY;         //288
int          mpegIntraDcPrecision; //0
int          mpegTopFieldFirst;    //0

///////////////////////////////////////////////
// we can make these 3 value arrays
int          mpeg_frame_pred_dct_tab[3];
int          mpeg_conceal_tab[3];
int          mpeg_qscale_tab[3];
int          mpeg_intravlc_tab[3];
int          mpeg_altscan_tab[3];

int          mpegRepeatFirstField;  //0
int          mpegProgressiveFrame;  //1
int          mpegPDistanceRefresh;  //0
int          mpegRRateControl;      //0
int          mpegAvgRateControl;    //0
int          mpegXiRateControl;     //0
int          mpegXpRateControl;     //0
int          mpegXbRateControl;     //0
int          mpegdOiRateControl;    //0
int          mpegdOpRateControl;    //0
int          mpegdObRateControl;    //0


///////////////////////////////////////////////
// we can make these 4 value arrays
int          motion_data[4];
//int          mpegPFHorizCode;          //2
//int          mpegPFVertCode;           //2
//int          mpegPFSearchWidth;        //11
//int          mpegPFSearchHeight;       //11

int          mpegB1FHorizCode;          //1
int          mpegB1FVertCode;           //1
int          mpegB1FSearchWidth;        //3
int          mpegB1FSearchHeight;       //3

int          mpegB1BHorizCode;          //1
int          mpegB1BVertCode;           //1
int          mpegB1BSearchWidth;        //7
int          mpegB1BSearchHeight;       //7

int          mpegB2FHorizCode;          //1
int          mpegB2FVertCode;           //1
int          mpegB2FSearchWidth;        //7
int          mpegB2FSearchHeight;       //7

int          mpegB2BHorizCode;          //1
int          mpegB2BVertCode;           //1
int          mpegB2BSearchWidth;        //3
int          mpegB2BSearchHeight;       //3

} RenderConfig;


#endif // RenderConfig_h
