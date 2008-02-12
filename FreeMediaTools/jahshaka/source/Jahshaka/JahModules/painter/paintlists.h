/*******************************************************************************
**
** The source file for the Jahshaka animation module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

//once this works we will start to store the images on disk
//instead of in memory

class PaintHistory
{
protected:
    GLuint              m_texture_id;
    bool                m_dirty;
    int                 m_original_video_frame_number;

public:
    int                 getOriginalVideoFrameNumber() { return m_original_video_frame_number; }
    void                setOriginalVideoFrameNumber(int number) { m_original_video_frame_number = number; }
    GLuint              getTextureId() { return m_texture_id; }
    void                setTextureId(GLuint id) { m_texture_id = id; }
    GLuint*             getTextureIdPtr() { return &m_texture_id; }
    bool                getDirty() { return m_dirty; }
    void                setDirty() { m_dirty = true; }
    void                clearDirty() { m_dirty = false; }
    int                 insertSorted(QPtrList<PaintHistory>* list);

    PaintHistory(int frame_number = 0)
    {
        m_texture_id = INVALID_TEXTURE_ID;
        m_dirty = false;
        glGenTextures( 1, getTextureIdPtr() );
        setOriginalVideoFrameNumber(frame_number);
    }

    ~PaintHistory()
    {
        if ( *getTextureIdPtr() && getTextureId() != INVALID_TEXTURE_ID )
        {
            glDeleteTextures(1, getTextureIdPtr() );
        }
    }
};



class PaintImage
{
public:
    PaintImage(){};

    QImage getImage() 	
	{ 
		return theimage; 
	}
    
    int getFramenumber() 	
	{ 
		return frameNumber; 
	}
    
	void   addImage(QImage img, int theframe)
	{ 
		theimage = img; 
		frameNumber = theframe;
	}

	void   updateImage(QImage img)
	{ 
		theimage = img; 
	}

private:

    int             frameNumber;        //number of selected layers
    QImage          theimage;			//number of layers in listview

};

class PaintImageList
{
public:
    PaintImageList()
	{
		numImages = 0;

        paintlist = new QPtrList<PaintImage>;
        paintlist->setAutoDelete( TRUE );     // delete items when they are removed

	};

    QImage getImage(int frame)
	{
		int thelayer = findFrameLayer(frame);

		if (thelayer == -1) 
			return false;
		
		return	paintlist->at(thelayer)->getImage();
	};
    
	bool   updateList(int frame, QImage theimage)
	{

		if (isThereAImage(frame))
		{
			//debug("theres a image already");
			updateImage(frame, theimage);
		}
		else
		{
			//debug("doing a new image");
			newImage(frame, theimage);
		}

		return true;

	}


	bool   isThereAImage(int frame)
	{
		int thelayer = findFrameLayer(frame);

		if (thelayer == -1) 
			return false;
		else
			return true;
	};
	
	bool   newImage(int frame, QImage theimage)
	{
		//debug(QString("adding new image for %1").arg(numImages));

		paintlist->append( new PaintImage() );
		paintlist->at(numImages)->addImage(theimage,frame);
		
		numImages +=1;

		//false if we were out of memory
		return true;
	}
	
	bool   updateImage(int frame, QImage theimage)
	{
		//1. find which item in the list has the framenumber
		// for i=0 to numImages...
		int thelayer = findFrameLayer(frame);

		if (thelayer == -1) 
			return false;
		
		paintlist->at(thelayer)->updateImage(theimage);

		return true;
	};

	int findFrameLayer(int frame)
	{
		if (numImages ==0)
			return -1;

		int frameLayer = -1;

		for (int i=0; i<numImages; i++)
		{
			if ( paintlist->at(i)->getFramenumber() == frame )
				frameLayer = i;
		}

		return frameLayer;
	}

private:

    QPtrList<PaintImage>   *paintlist;   //for the linked list for dynamic object layers
	int numImages;
};

