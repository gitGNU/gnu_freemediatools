/*******************************************************************************
**
** The source file for the Jahshaka paint module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/


#include "paint.h"
#include <InputLCD.h>
#include <supergrangecontrol.h>

int             
GLPaint::getCurrentHistoryFrame() 
{ 
    return m_current_history_frame; 
}


void            
GLPaint::deleteFrames(int start_frame, int count)
{
    if ( start_frame >= (int)getHistoryListPtr()->count() )
    {
        return;
    }

	if (start_frame < 1)
	{
		return;
	}

    int frame_number = start_frame;
    int number_removed = 0;

    for ( ; getHistoryListPtr()->count() > 1 && number_removed < count; )
    {
        PaintHistory* history_to_remove = getHistoryListPtr()->at(frame_number);

        if (history_to_remove)
        {
            getDirtyFramesList()->remove(history_to_remove);
            getHistoryListPtr()->remove(frame_number);
            number_removed++;
            frame_number++;
        }
        else
        {
            break;
        }
    }

    m_nMaxFrame = jahstd::Min<unsigned int>(m_nMaxFrame, getHistoryListPtr()->count() - 1);
    updatesliderEndframe(m_nMaxFrame);
    m_pEndFrameControl->setValue(m_nMaxFrame);
    updateTimeSlider(start_frame - 1);
	m_current_history_frame = jahstd::Max<int>(start_frame - 1, 1);
    m_nCurrentFrame = m_current_history_frame;
    updateGL();
}

void            
GLPaint::copyFrames(int start_frame, int count)
{
    if ( start_frame >= (int)getHistoryListPtr()->count() )
    {
        return;
    }

    int frame_number = start_frame;

    delete m_copy_list_ptr;
    m_copy_list_ptr = new QPtrList<PaintHistory>;
    getCopyListPtr()->setAutoDelete(true);

    int i;

    if ( !getClipIsStillImage() )
    {
        // Force any unreferenced video frames into history
        for (i = 0; i < count; i++)
        {
            nextframeanimation();
        }

        for (i = 0; i < count; i++)
        {
            previousframeanimation();
        }
    }

    for ( ; frame_number < start_frame + count; frame_number++ )
    {
        if ( getHistoryListPtr()->at(frame_number) )
        {
            PaintHistory* frame = getHistoryListPtr()->at(frame_number);
            GLuint* texture_id_ptr = frame->getTextureIdPtr();
            PaintHistory* frame_copy = new PaintHistory;
            getCopyListPtr()->append(frame_copy);
            frame_copy->setOriginalVideoFrameNumber( frame->getOriginalVideoFrameNumber() );
            frame_copy->setDirty();
            texture_id_ptr = getCopyListPtr()->getLast()->getTextureIdPtr();
            refreshTexture(texture_id_ptr);
            glBindTexture(GL_TEXTURE_2D, *texture_id_ptr); 
            glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, getLowerLeftX(), getLowerLeftY(), getFrameWidth(), getFrameHeight());
        }
        else
        {
            break;
        }
    }
}

void            
GLPaint::pasteFrames(int start_frame)
{
    for ( PaintHistory* frame = getCopyListPtr()->first(); frame; frame = getCopyListPtr()->next() )
    {
        GLuint* texture_id_ptr = frame->getTextureIdPtr();
		drawTexturedQuad(*texture_id_ptr);

        PaintHistory* frame_copy = new PaintHistory;
        frame_copy->setOriginalVideoFrameNumber( frame->getOriginalVideoFrameNumber() );
        frame_copy->setDirty();
        texture_id_ptr = frame_copy->getTextureIdPtr();
        refreshTexture(texture_id_ptr);
        glBindTexture(GL_TEXTURE_2D, *texture_id_ptr); 
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, getLowerLeftX(), getLowerLeftY(), getFrameWidth(), getFrameHeight());

        getHistoryListPtr()->insert(++start_frame, frame_copy);
        // The new frame should be inserted in to the dirty frames list, but
        // we don't want is deleted nor sorted by the original frame number
        //frame_copy->insertSorted( getDirtyFramesList() );
    }

    m_current_history_frame += getCopyListPtr()->count();
    m_nCurrentFrame = m_current_history_frame;
    m_nMaxFrame += getCopyListPtr()->count();
    updatesliderEndframe(m_nMaxFrame);
    m_pEndFrameControl->setValue(m_nMaxFrame);
    updateTimeSlider(m_nCurrentFrame);
}

void            
GLPaint::clearVideoFrameDirtyList()
{
    PaintHistory* paint_history = getHistoryListPtr()->first();

    for (; paint_history; paint_history = getHistoryListPtr()->next() )
    {
        paint_history->clearDirty();
    }
}

bool            
GLPaint::getVideoFrameDirty(int frame_number) 
{ 
    if ( !getHistoryListPtr() )
    {
        return false;
    }

    PaintHistory* paint_history = getHistoryListPtr()->at(frame_number);

    if (!paint_history)
    {
        return false;
    }

    return paint_history->getDirty();
}

void            
GLPaint::setVideoFrameDirty(int frame_number, bool flag) 
{ 
    //m_video_frame_dirty[frame_number] = flag; 
    PaintHistory* paint_history = getHistoryListPtr()->at(frame_number);

    if (!paint_history)
    {
        return;
    }

    if (flag)
    {
        paint_history->setDirty();
    }
    else
    {
        paint_history->clearDirty();
    }
}



void 
GLPaint::historyRemoveCurrent()
{
    if ( m_current_history_frame < 2 )
    {
        return;
    }

    deleteFrames(m_current_history_frame, 1);
}


/*
| Store the current buffer into the next location in this canvas' history.
*/
void GLPaint::incrementHistoryCount(bool add_a_frame) 
{
    int list_size = (int)getHistoryListPtr()->count();
	m_current_history_frame = jahstd::Min<int>(list_size, m_current_history_frame + 1);

    if ( getCurrentHistoryFrame() >= list_size)
    {
        PaintHistory* new_history_list_element =  new PaintHistory;
        getHistoryListPtr()->append(new_history_list_element);
    }
    else if (add_a_frame)
    {
        PaintHistory* new_history_list_element =  new PaintHistory;
        getHistoryListPtr()->insert(getCurrentHistoryFrame(), new_history_list_element);
    }

    if (m_current_history_frame > m_nMaxFrame)
    {
        m_nMaxFrame = m_current_history_frame;
        updatesliderEndframe(m_nMaxFrame);
        m_pEndFrameControl->setValue(m_nMaxFrame);
    }

    m_nCurrentFrame = m_current_history_frame;
}

int                 
PaintHistory::insertSorted(QPtrList<PaintHistory>* list)
{
    PaintHistory* paint_history = list->first();
    int this_frame_number = this->getOriginalVideoFrameNumber();

    for ( ; paint_history; paint_history = list->next() )
    {
        if ( paint_history->getOriginalVideoFrameNumber() > this_frame_number )
        {
            break;
        }
    }

    int position = list->at();

    if (!paint_history)
    {
        list->append(this);
        position = list->at();
    }
    else
    {
        list->insert(position, this);
    }

    //printf("Inserted frame # %d at position %d\n", this_frame_number, position);
    return position;
}
















