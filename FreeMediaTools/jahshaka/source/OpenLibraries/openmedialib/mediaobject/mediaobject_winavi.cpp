/*******************************************************************************
**
** The source file for the Jahshaka OpenMediaLib class
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

/*
  this is windows only code.

*/
#ifdef WIN32

#include "mediaobject_winavi.h"

////////////////////////////////////////
// constructors and destructors

mediaObjectWinavi::mediaObjectWinavi(void) : hdd(0)
{ 
    jtrace = OL_Trace::getInstance();	//set up tracer

    mediaopen = false;
}

mediaObjectWinavi::~mediaObjectWinavi(void) 
{ 
    if (mediaopen) 
    { 
        closeMedia(); 
    }
}

////////////////////////////////////////
// main code only uses AVI for now

void  mediaObjectWinavi::printinfo(void) 
{
};

bool  mediaObjectWinavi::isMediaOpen(void) 
{
    return mediaopen;
};

bool mediaObjectWinavi::openMedia(QString szFile) 
{

    bool status = false;
    
    if (mediaopen) 
    { 
        closeMedia(); 
    }
    
    hdc = CreateCompatibleDC(0);								// Creates A Compatible Device Context
    data = 0;										            // Pointer To Our Resized Image

	AVIFileInit();												// Opens The AVIFile Library

	// Opens The AVI Stream
	if (AVIStreamOpenFromFile(&pavi, szFile, streamtypeVIDEO, 0, OF_READ, NULL) !=0)
	{
		debug("Failed to open The AVI file");
        return false;
    }

	AVIStreamInfo(pavi, &psi, sizeof(psi));						// Reads Information About The Stream Into psi
	aviwidth=psi.rcFrame.right-psi.rcFrame.left;					// Width Is Right Side Of Frame Minus Left
	aviheight=psi.rcFrame.bottom-psi.rcFrame.top;					// Height Is Bottom Of Frame Minus Top

	avilastframe=AVIStreamLength(pavi);							// The Last Frame Of The Stream

	mpf=AVIStreamSampleToTime(pavi,avilastframe)/avilastframe;		// Calculate Rough Milliseconds Per Frame

	bmih.biSize = sizeof (BITMAPINFOHEADER);					// Size Of The BitmapInfoHeader
	bmih.biPlanes = 1;											// Bitplanes	
	bmih.biBitCount = 32;										// Bits Format We Want (24 Bit, 3 Bytes)
	bmih.biWidth = aviwidth;	//256										// Width We Want (256 Pixels)
	bmih.biHeight = aviheight; //256										// Height We Want (256 Pixels)
	bmih.biCompression = BI_RGB;								// Requested Mode = RGB

	hBitmap = CreateDIBSection (hdc, (BITMAPINFO*)(&bmih), DIB_RGB_COLORS, (void**)(&data), NULL, NULL);
	SelectObject (hdc, hBitmap);								// Select hBitmap Into Our Device Context (hdc)

	pgf=AVIStreamGetFrameOpen(pavi, NULL);						// Create The PGETFRAME	Using Our Request Mode
	if (pgf==NULL)
	{
		debug("An Error Occurred Opening The Frame");
        mediaopen = false; return false;
	} 

    mediaopen = true;
    status = true;

    return status;
};

int mediaObjectWinavi::getVideoLength() 
{
    if (mediaopen) 
    { 
        //should check if stream is open...
        int maxFrames=AVIStreamLength(pavi);							// The Last Frame Of The Stream
        return maxFrames;
    }

    return 0;

};
 
bool mediaObjectWinavi::closeMedia(void) 
{
    bool status = false;

    if (mediaopen) 
    { 
	    DeleteObject(hBitmap);										// Delete The Device Dependant Bitmap Object
	    DrawDibClose(hdd);											// Closes The DrawDib Device Context
	    AVIStreamGetFrameClose(pgf);								// Deallocates The GetFrame Resources
	    AVIStreamRelease(pavi);										// Release The Stream
	    AVIFileExit();												// Release The File
            
        mediaopen = false; 
        status = true;
    }

    return status;

};
        

QImage mediaObjectWinavi::getVideoFrame(int frame) 
{
    QImage thedata;
    QString rere;

    if (frame<1) frame=1;

    //lastframe crashes app for some reason...
    if (mediaopen) 
    { 
        if (frame>(avilastframe-1)) 
        { 
            frame=(avilastframe-1); 
        }

        LPBITMAPINFOHEADER pBmp;												// Holds The Bitmap Header Information
	    pBmp = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pgf, frame);				// Grab Data From The AVI Stream **returns a DIB
	    
        LPBITMAPINFO pBmpInfo = (LPBITMAPINFO)pBmp;
	    LPBITMAPINFOHEADER pBmpInfoHeader = (LPBITMAPINFOHEADER)pBmpInfo;

        LPBYTE pBmpBits = ((LPBYTE)pBmpInfo) + pBmpInfo->bmiHeader.biSize;
		    
        LPBYTE pGDIBits;	
        HBITMAP hBmp = CreateDIBSection(hdc, pBmpInfo, DIB_RGB_COLORS, (LPVOID*)&pGDIBits, NULL, 0);

        memcpy(pGDIBits, pBmpBits, pBmpInfoHeader->biSizeImage);
		    
        HDC hBmpDC = CreateCompatibleDC(hdc);

        //this is where we create our qimage
        //need to speed this process up as it s slow...
        //QImage thedata = bitmapToImage( hBmpDC, hBmp  );
        thedata = bitmapToImage( hBmpDC, hBmp  );

        DeleteObject(hBmp);
        //ReleaseDC(hBmpDC);
	    //debug(">>grabbed the frame");

        return thedata;
    }

    //this is where we grab a blank image for display
    //since nothing else worked...
    jtrace->debug(">>couldnt grab anything");

    if ( !thedata.load( JahBasePath+"Pixmaps/missingmedia.png" ) ) {	// Load first image from file
                qWarning( "Could not read default image, using single-color instead." );
                QImage dummy( 256, 256, 32 );
                dummy.fill( Qt::blue.rgb() );
                thedata = dummy;
                //returnval = FALSE;
    }

return thedata;

};
    
void  mediaObjectWinavi::getAudioFrame(int frame) 
{

        //int max;
        //return max;
};

QString mediaObjectWinavi::getFileName() 
{
  jtrace->debug(">>getting media filename");

    QString loadfile;
    return loadfile;  
};
    
int  mediaObjectWinavi::getNumberFrames() 
{
  jtrace->debug(">>getting number frames");

    if (mediaopen) { 
        //should check if stream is open...
        int maxFrames=AVIStreamLength(pavi);	// The Last Frame Of The Stream
        return maxFrames;
    }

    return false;
};

////////////////////////////////////////////////
// utilities here need to be in separate file ?


QImage mediaObjectWinavi::bitmapToImage(HDC chdc, HBITMAP chBitmap)
{
	//debug("converting the frame");
    QImage res;
    BITMAP bmp;
    PBITMAPINFO pbmi;
    WORD    cClrBits;

    // Retrieve the bitmap color format, width, and height.
    if (!GetObject(chBitmap, sizeof(BITMAP), (LPSTR)&bmp))
        return res;

    // Convert the color format to a count of bits.
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
    if (cClrBits == 1)
        cClrBits = 1;
    else if (cClrBits <= 4)
        cClrBits = 4;
    else if (cClrBits <= 8)
        cClrBits = 8;
    else if (cClrBits <= 16)
        cClrBits = 16;
    else if (cClrBits <= 24)
        cClrBits = 24;
    else cClrBits = 32;

    // Allocate memory for the BITMAPINFO structure. (This structure
    // contains a BITMAPINFOHEADER structure and an array of RGBQUAD
    // data structures.)

	if (cClrBits != 24) {
		pbmi = (PBITMAPINFO) new unsigned char[sizeof(BITMAPINFOHEADER) +  sizeof(RGBQUAD) * (1<< cClrBits)]; }
     // There is no RGBQUAD array for the 24-bit-per-pixel format. 

	else {
		pbmi = (PBITMAPINFO) new unsigned char[sizeof(BITMAPINFOHEADER)]; }

    // Initialize the fields in the BITMAPINFO structure. 

    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
    pbmi->bmiHeader.biWidth = bmp.bmWidth; 
    pbmi->bmiHeader.biHeight = bmp.bmHeight; 
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel; 
    if (cClrBits < 24) 
        pbmi->bmiHeader.biClrUsed = (1<<cClrBits); 

    // If the bitmap is not compressed, set the BI_RGB flag. 
    pbmi->bmiHeader.biCompression = BI_RGB; 

    // Compute the number of bytes in the array of color 
    // indices and store the result in biSizeImage. 
    // For Windows NT, the width must be DWORD aligned unless 
    // the bitmap is RLE compressed. This example shows this. 
    // For Windows 95/98/Me, the width must be WORD aligned unless the 
    // bitmap is RLE compressed.
    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) /8
                                  * pbmi->bmiHeader.biHeight; 
    // Set biClrImportant to 0, indicating that all of the 
    // device colors are important. 
     pbmi->bmiHeader.biClrImportant = 0;
	 
    BITMAPFILEHEADER hdr;       // bitmap file-header 
    PBITMAPINFOHEADER pbih;     // bitmap info-header 
    LPBYTE lpBits;              // memory pointer 
    DWORD dwTotal;              // total count of bytes 
    DWORD cb;                   // incremental count of bytes 
    BYTE *hp;                   // byte pointer 

    pbih = (PBITMAPINFOHEADER) pbmi; 
    lpBits = (LPBYTE) new unsigned char[pbih->biSizeImage];     

    // Retrieve the color table (RGBQUAD array) and the bits 
    // (array of palette indices) from the DIB. 
    if (!GetDIBits(chdc, chBitmap, 0, (WORD) pbih->biHeight, lpBits, pbmi, 
        DIB_RGB_COLORS)) 
    {
        delete[] pbmi; 
		delete[] lpBits;
		return res;
    }
    
    hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M" 
    // Compute the size of the entire file. 
    hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + 
                 pbih->biSize + pbih->biClrUsed 
                 * sizeof(RGBQUAD) + pbih->biSizeImage); 
    hdr.bfReserved1 = 0; 
    hdr.bfReserved2 = 0; 

    // Compute the offset to the array of color indices. 
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + 
                    pbih->biSize + pbih->biClrUsed 
                    * sizeof (RGBQUAD); 

	dwTotal = cb = pbih->biSizeImage;
	hp = lpBits;
	
	long length1 = sizeof(BITMAPFILEHEADER);
	long length2 = sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof (RGBQUAD);
	long length3 = dwTotal;
	long length = length1 + length2 + length3;

	unsigned char *finalBuffer = new unsigned char[length];

	memcpy(finalBuffer, &hdr, length1);
	memcpy(finalBuffer + length1, pbih, length2);
	memcpy(finalBuffer + length1 + length2, hp, length3);

	res.loadFromData((uchar *) finalBuffer, length);

	delete[] pbmi;
	delete[] lpBits;
	delete[] finalBuffer;

	return res;
}

#endif
