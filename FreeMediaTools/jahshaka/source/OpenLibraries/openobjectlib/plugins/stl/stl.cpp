#include <fstream>
#include <qglobal.h>

#include "stl.h"

#ifdef WHEREFROM

#include "defined.h"
#include "integers.h"
#include "reals.h"
#include "basic.h"
#include "streambin.h"
#include "charutilities.h"

namespace charutilities 
{
using namespace std ;

static bool OpenStlAscii ( 
	char const * const & aFileName,
	xyz * & aoVertices,		//must be allocated by: new type[0]
	uint32 & aoNumOfVertices,
	ijk * & aoTriangles,	//must be allocated by: new type[0]
	uint32 & aoNumOfTriangles)
{
    uint32 i;

	ASSERT ( aoVertices ) ;
	ASSERT ( aoTriangles ) ;

	uint32 const lc_AllocationBlockSizeBig = 1024 ;

    char lFilename[1024];
    insureExtension(lFilename,aFileName,"stl");

    FILE * lFile=fopen(lFilename,"rt");

    uint32 lTrianglesAllocated = lc_AllocationBlockSizeBig;
    ijk * lTriangles = (ijk *) malloc(lTrianglesAllocated*sizeof(ijk));
    ASSERT(lTriangles);
    aoNumOfTriangles=0;

    uint32 lVerticesAllocated = lc_AllocationBlockSizeBig;
    xyz * lVertices = (xyz *) malloc(lVerticesAllocated*sizeof(xyz));
    ASSERT(lVertices);
    aoNumOfVertices=0;

    char lString[1024];
    char * lPointer;
    
    do
    {
        fgets(lString,1024,lFile);
        lPointer=lString;
        while ( (*lPointer=='\t') || (*lPointer=='\r') || (*lPointer==' ') ) lPointer++;
    }
    while (*lPointer=='\n');
    toLowercaseInPlace(lPointer);
    
    if (strncmp(lPointer,"solid",5)==0)
    {
        do
        {
            fgets(lString,1024,lFile);
            lPointer=lString;
            while ( (*lPointer=='\t') || (*lPointer=='\r') || (*lPointer==' ') ) lPointer++;
        }
        while (*lPointer=='\n');
        toLowercaseInPlace(lPointer);
    }    

    while (strncmp(lPointer,"facet normal",12)==0)
    {
        insureReallocation(sizeof(ijk),lc_AllocationBlockSizeBig,(aoNumOfTriangles+1),lTrianglesAllocated,(void*&)lTriangles);
        insureReallocation(sizeof(xyz),lc_AllocationBlockSizeBig,(aoNumOfVertices+3),lVerticesAllocated,(void*&)lVertices);

        do
        {
            fgets(lString,1024,lFile);
            lPointer=lString;
            while ( (*lPointer=='\t') || (*lPointer=='\r') || (*lPointer==' ') ) lPointer++;
        }
        while (*lPointer=='\n');
        toLowercaseInPlace(lPointer);
    
        ASSERT(strncmp(lPointer,"outer loop",10)==0);
        do
        {
            fgets(lString,1024,lFile);
            lPointer=lString;
            while ( (*lPointer=='\t') || (*lPointer=='\r') || (*lPointer==' ') ) lPointer++;
        }
        while (*lPointer=='\n');
        toLowercaseInPlace(lPointer);
        
        ASSERT(strncmp(lPointer,"vertex",6)==0);    
        lTriangles[aoNumOfTriangles].i=aoNumOfVertices;
#ifdef REAL_IS_FLT
        sscanf(lPointer,"%*s %e %e %e\n",&lVertices[aoNumOfVertices].x,
                                        &lVertices[aoNumOfVertices].y,
                                        &lVertices[aoNumOfVertices].z);
        aoNumOfVertices++;
        do
        {
            fgets(lString,1024,lFile);
            lPointer=lString;
            while ( (*lPointer=='\t') || (*lPointer=='\r') || (*lPointer==' ') ) lPointer++;
        }
        while (*lPointer=='\n');
        ToLowerInPlace(lPointer);
        
        ASSERT(strncmp(lPointer,"vertex",6)==0);
        lTriangles[aoNumOfTriangles].j=aoNumOfVertices;
        sscanf(lPointer,"%*s %e %e %e\n",&lVertices[aoNumOfVertices].x,
                                        &lVertices[aoNumOfVertices].y,
                                        &lVertices[aoNumOfVertices].z);
        aoNumOfVertices++;
        do
        {
            fgets(lString,1024,lFile);
            lPointer=lString;
            while ( (*lPointer=='\t') || (*lPointer=='\r') || (*lPointer==' ') ) lPointer++;
        }
        while (*lPointer=='\n');
        ToLowerInPlace(lPointer);
        
        ASSERT(strncmp(lPointer,"vertex",6)==0);
        lTriangles[aoNumOfTriangles].k=aoNumOfVertices;
        sscanf(lPointer,"%*s %e %e %e\n",&lVertices[aoNumOfVertices].x,
                                        &lVertices[aoNumOfVertices].y,
                                        &lVertices[aoNumOfVertices].z);
#else
        sscanf(lPointer,"%*s %le %le %le\n",&lVertices[aoNumOfVertices].x,
                                        &lVertices[aoNumOfVertices].y,
                                        &lVertices[aoNumOfVertices].z);
        aoNumOfVertices++;
        do
        {
            fgets(lString,1024,lFile);
            lPointer=lString;
            while ( (*lPointer=='\t') || (*lPointer=='\r') || (*lPointer==' ') ) lPointer++;
        }
        while (*lPointer=='\n');
        toLowercaseInPlace(lPointer);
        
        ASSERT(strncmp(lPointer,"vertex",6)==0);
        lTriangles[aoNumOfTriangles].j=aoNumOfVertices;
        sscanf(lPointer,"%*s %le %le %le\n",&lVertices[aoNumOfVertices].x,
                                        &lVertices[aoNumOfVertices].y,
                                        &lVertices[aoNumOfVertices].z);
        aoNumOfVertices++;
        do
        {
            fgets(lString,1024,lFile);
            lPointer=lString;
            while ( (*lPointer=='\t') || (*lPointer=='\r') || (*lPointer==' ') ) lPointer++;
        }
        while (*lPointer=='\n');
        toLowercaseInPlace(lPointer);
        
        ASSERT(strncmp(lPointer,"vertex",6)==0);
        lTriangles[aoNumOfTriangles].k=aoNumOfVertices;
        sscanf(lPointer,"%*s %le %le %le\n",&lVertices[aoNumOfVertices].x,
                                        &lVertices[aoNumOfVertices].y,
                                        &lVertices[aoNumOfVertices].z);
#endif // REAL_IS_FLT
        aoNumOfVertices++;
        do
        {
            fgets(lString,1024,lFile);
            lPointer=lString;
            while ( (*lPointer=='\t') || (*lPointer=='\r') || (*lPointer==' ') ) lPointer++;
        }
        while (*lPointer=='\n');
        toLowercaseInPlace(lPointer);

        ASSERT(strncmp(lPointer,"endloop",7)==0);    
        do
        {
            fgets(lString,1024,lFile);
            lPointer=lString;
            while ( (*lPointer=='\t') || (*lPointer=='\r') || (*lPointer==' ') ) lPointer++;
        }
        while (*lPointer=='\n');
        toLowercaseInPlace(lPointer);

        ASSERT(strncmp(lPointer,"endfacet",7)==0);    
        do
        {
            fgets(lString,1024,lFile);
            lPointer=lString;
            while ( (*lPointer=='\t') || (*lPointer=='\r') || (*lPointer==' ') ) lPointer++;
        }
        while (*lPointer=='\n');
        toLowercaseInPlace(lPointer);

        aoNumOfTriangles++;
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

static bool OpenStlBinary(
	char const * const & aFileName,
	bool const & a_file_is_little_endian,
	xyz * & aoVertices,  //must be allocated by: new type[0]
	uint32 & aoNumOfVertices,
	ijk * & aoTriangles, //must be allocated by: new type[0]
	uint32 & aoNumOfTriangles)
{
    uint32 i;

	ASSERT ( aoVertices ) ;
	ASSERT ( aoTriangles ) ;
    
    char lFilename[1024];
    insureExtension(lFilename,aFileName,"stl");

    int64 lFileSize = fileSize(lFilename);

    fstreamb_endian lFile ;
	lFile.m_file_is_little_endian = a_file_is_little_endian;
    lFile.open(lFilename,ios::in|ios::binary);

    lFile.seekg(80,ios::beg);
    
    lFile >> (uint32 &) aoNumOfTriangles;
       
    int32 lBinaryStlFileSize = 80 + 4 + aoNumOfTriangles * 50;
    
    if (lBinaryStlFileSize!=lFileSize)
    {
        lFile . close ( ) ;
        return false ;
    }

	delete [ ] aoTriangles ;
	delete [ ] aoVertices ;
    aoTriangles = new ijk [aoNumOfTriangles];
    aoVertices = new xyz [3*aoNumOfTriangles];
    
    aoNumOfVertices=0;
    for (i=0;i<aoNumOfTriangles;i++)
    {
        float lTmpfloat;
        lFile >> (float &) lTmpfloat;   
        lFile >> (float &) lTmpfloat;   
        lFile >> (float &) lTmpfloat;   

        aoTriangles[i].i=aoNumOfVertices;
        lFile >> (float &) lTmpfloat;   
        aoVertices[aoNumOfVertices].x=lTmpfloat;   
        lFile >> (float &) lTmpfloat;   
        aoVertices[aoNumOfVertices].y=lTmpfloat;   
        lFile >> (float &) lTmpfloat;   
        aoVertices[aoNumOfVertices].z=lTmpfloat;   
        aoNumOfVertices++;

        aoTriangles[i].j=aoNumOfVertices; 
        lFile >> (float &) lTmpfloat;   
        aoVertices[aoNumOfVertices].x=lTmpfloat;   
        lFile >> (float &) lTmpfloat;   
        aoVertices[aoNumOfVertices].y=lTmpfloat;   
        lFile >> (float &) lTmpfloat;   
        aoVertices[aoNumOfVertices].z=lTmpfloat;   
        aoNumOfVertices++;

        aoTriangles[i].k=aoNumOfVertices;
        lFile >> (float &) lTmpfloat;   
        aoVertices[aoNumOfVertices].x=lTmpfloat;   
        lFile >> (float &) lTmpfloat;   
        aoVertices[aoNumOfVertices].y=lTmpfloat;   
        lFile >> (float &) lTmpfloat;   
        aoVertices[aoNumOfVertices].z=lTmpfloat;   
        aoNumOfVertices++;

        unsigned short lTmpShort;
        lFile >> (uint16 &) lTmpShort;   
    }

    lFile . close ( ) ; 
    return true ; 
} 

bool readStl (
	char const * const & aFileName,
	vector < xyz > & aoVertices,		//must be allocated by: new type[0]
	vector < ijk > & aoTriangles)		//must be allocated by: new type[0]
{
	xyz * lVertices = new xyz [ 0 ] ;
	ijk * lTriangles = new ijk [ 0 ] ;
	uint32 lNumOfVertices;
	uint32 lNumOfTriangles;
	bool lRead = OpenStlBinary(
		aFileName,
		charutilities::c_is_little_endian,
		lVertices,
		lNumOfVertices,
		lTriangles,
		lNumOfTriangles);
        
	if (lRead==false)
	{ 
		lRead = OpenStlBinary(
			aFileName,
			!charutilities::c_is_little_endian,
			lVertices,
			lNumOfVertices,
			lTriangles,
			lNumOfTriangles);

		if (lRead==false)
		{ 
			lRead = OpenStlAscii(
				aFileName,
				lVertices,
				lNumOfVertices,
				lTriangles,
				lNumOfTriangles);
		}
	}

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
	else
	{
		aoVertices . resize ( 0 ) ;
		aoTriangles . resize ( 0 ) ;
	}

	return lRead;
}
	

} // charutilities


#endif
