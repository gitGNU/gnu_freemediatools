#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <qglobal.h>

#include "dxf.h"

#ifdef USE_DFX
/*
  where does this code come from?
*/
#include "basic.h"
#include "integers.h"
#include "reals.h"
#include "boundbox3d.h"
#include "charutilities.h"


using namespace charutilities;

//namespace charutilities 
//{

static char * GoToNextLineTrimmed(FILE * aFile)
{
    static char lString[1024];
    char * lPointer;        
    do
    {
      lPointer=fgets(lString,1024,aFile);
      if (lPointer!=NULL)
      {
        while ( (*lPointer=='\t') || (*lPointer=='\r') || (*lPointer==' ') ) 
        {
          lPointer++;
        }
      }
    }
    while ( (lPointer!=NULL) && (*lPointer=='\n') );
    toLowercaseInPlace(lPointer);
    return lPointer;
}

static char * GoToNext3dFace(FILE * aFile)
{
  char * lPointer;        
  do
  {
    lPointer=GoToNextLineTrimmed(aFile);
  }
  while ( (lPointer!=NULL) && (strncmp(lPointer,"3dface",6)!=0) );
  return lPointer;
}

bool OpenDxf ( 
	char const * const & aFileName ,
	xyz * & aoVertices ,
	uint32 & aoNumOfVertices ,
	ijk * & aoTriangles ,
	uint32 & aoNumOfTriangles )
{
    uint32 i;

	ASSERT ( aoVertices ) ;
  	ASSERT ( aoTriangles ) ;

	uint32 const lc_AllocationBlockSizeBig = 1024 ;

    char lFileName[1024];
    insureExtension(lFileName,aFileName,"dxf");

    FILE * lFile=fopen(lFileName,"rt");

    uint32 lTrianglesAllocated = lc_AllocationBlockSizeBig;
    ijk * lTriangles = (ijk *) malloc(lTrianglesAllocated*sizeof(ijk));
    ASSERT(lTriangles);
    aoNumOfTriangles=0;

    uint32 lVerticesAllocated = lc_AllocationBlockSizeBig;
    xyz * lVertices = (xyz *) malloc(lVerticesAllocated*sizeof(xyz));
    ASSERT(lVertices);
    aoNumOfVertices=0;

    while (GoToNext3dFace(lFile)!=NULL)
    {
        insureReallocation(sizeof(ijk),lc_AllocationBlockSizeBig,(aoNumOfTriangles+2),lTrianglesAllocated,(void*&)lTriangles);
        insureReallocation(sizeof(xyz),lc_AllocationBlockSizeBig,(aoNumOfVertices+4),lVerticesAllocated,(void*&)lVertices);

        xyz lVerticesTmp[4];
        
        int32 lCode;
        while ( ( lCode = atol(GoToNextLineTrimmed(lFile)) ) != 0 )
        {
          switch (lCode)
          {
            case 10 : 
                lVerticesTmp[0].x = atof(GoToNextLineTrimmed(lFile));
                break;
            case 20 : 
                lVerticesTmp[0].y = atof(GoToNextLineTrimmed(lFile));
                break;
            case 30 : 
                lVerticesTmp[0].z = atof(GoToNextLineTrimmed(lFile));
                break;
            case 11 : 
                lVerticesTmp[1].x = atof(GoToNextLineTrimmed(lFile));
                break;
            case 21 : 
                lVerticesTmp[1].y = atof(GoToNextLineTrimmed(lFile));
                break;
            case 31 : 
                lVerticesTmp[1].z = atof(GoToNextLineTrimmed(lFile));
                break;
            case 12 : 
                lVerticesTmp[2].x = atof(GoToNextLineTrimmed(lFile));
                break;
            case 22 : 
                lVerticesTmp[2].y = atof(GoToNextLineTrimmed(lFile));
                break;
            case 32 : 
                lVerticesTmp[2].z = atof(GoToNextLineTrimmed(lFile));
                break;
            case 13 : 
                lVerticesTmp[3].x = atof(GoToNextLineTrimmed(lFile));
                break;
            case 23 : 
                lVerticesTmp[3].y = atof(GoToNextLineTrimmed(lFile));
                break;
            case 33 : 
                lVerticesTmp[3].z = atof(GoToNextLineTrimmed(lFile));
                break;
            default:
                GoToNextLineTrimmed(lFile);    
          }
        }

        lTriangles[aoNumOfTriangles].i=aoNumOfVertices;
        lVertices[aoNumOfVertices]=lVerticesTmp[0];
        aoNumOfVertices++;
        
        lTriangles[aoNumOfTriangles].j=aoNumOfVertices;
        lVertices[aoNumOfVertices]=lVerticesTmp[1];
        aoNumOfVertices++;
        
        lTriangles[aoNumOfTriangles].k=aoNumOfVertices;
        lVertices[aoNumOfVertices]=lVerticesTmp[2];
        aoNumOfVertices++;

        aoNumOfTriangles++;
        
        if (lVerticesTmp[3]!=lVerticesTmp[2])
        {
          lTriangles[aoNumOfTriangles].i=aoNumOfVertices-3;          
          lTriangles[aoNumOfTriangles].j=aoNumOfVertices-1;

          lTriangles[aoNumOfTriangles].k=aoNumOfVertices;
          lVertices[aoNumOfVertices]=lVerticesTmp[3];
          aoNumOfVertices++;
          
          aoNumOfTriangles++;        
        }
        
    }

    fclose(lFile);

    if (aoNumOfTriangles>0)
    {
		delete [ ] aoTriangles ;
        aoTriangles = new ijk [aoNumOfTriangles];
        for (i=0;i<aoNumOfTriangles;i++)
        {
            aoTriangles[i]=lTriangles[i];
        }
    }
    free(lTriangles);

    if (aoNumOfVertices>0)
    {
		delete [ ] aoVertices ;
        aoVertices = new xyz [aoNumOfVertices];
        for (i=0;i<aoNumOfVertices;i++)
        {
            aoVertices[i]=lVertices[i];
        }
    }
    free(lVertices);
    
    return true;
}

bool readDxf (
	char const * const & aFileName,
	std::vector <xyz> & aoVertices,
	std::vector <ijk> & aoTriangles)
{
  xyz * lVertices = new xyz [0];
  ijk * lTriangles = new ijk [0];
  uint32 lNumOfVertices;
  uint32 lNumOfTriangles;
  bool lRead = OpenDxf(aFileName,
                             lVertices,
                             lNumOfVertices,
                             lTriangles,
                             lNumOfTriangles);
  if (lRead==true)
  { 
    if (lVertices)
    {
  	  aoVertices . assign (&lVertices[0],&lVertices[lNumOfVertices]);
  	  delete [] lVertices;
    }
  	if (lTriangles)
  	{
  	  aoTriangles . assign (&lTriangles[0],&lTriangles[lNumOfTriangles]);
  	  delete [] lTriangles;
    }
  }

  return lRead;          
}

//} // charutilities 

#endif
