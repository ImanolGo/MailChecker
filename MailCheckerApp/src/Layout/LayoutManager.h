/*
 *  LayoutManager.h
 *  Mail Checker App
 *
 *  Created by Imanol Gomez on 31/05/18.
 *
 */


#pragma once

#include "Manager.h"
#include "TextVisual.h"
#include "ImageVisual.h"
#include "RectangleVisual.h"


//========================== class LayoutManager ==============================
//============================================================================
/** \class LayoutManager LayoutManager.h
 *    \brief Class managing the layout of the application
 *    \details It creates an places al the text and elements regarding the layout
 */

class LayoutManager: public Manager
{
    
public:
    
    static const int MARGIN;
    static const int FRAME_MARGIN;
    static const string LAYOUT_FONT;
    static const string LAYOUT_FONT_LIGHT;
    
    //! Constructor
    LayoutManager();
    
    //! Destructor
    ~LayoutManager();
    
    //! Set-up the layout
    void setup();
    
    //! Update the layout
    void update();
    
    //! Draw the layout
    void draw();
    
    void onFullScreenChange(bool value);
    
    void windowResized(int w, int h);
    
    const ofRectangle& getWindowRect() {return m_windowRect;}
    
    void setFullScreen();
    
    const ofFbo& getCurrentFbo(){return m_fbo;}
    
    
private:
    
    
    //! Create the text visuals
    void createTextVisuals();
    
    //! Create the image visuals
    void createImageVisuals();
    
    //! Create the background Image
    void createBackground();
    
    
    //! Add all visuals as overlays
    void addVisuals();
    
    void setupFbo();
    
    void updateFbo();
    
    void drawFbo();
    
    void drawText();
    
    void resetWindowRect();
    
    void resetWindowFrame();
    
    void resetWindowTitle();
    
    void setupWindowFrame();
    
private:
    
    
    typedef  map<string, ofPtr<TextVisual> >      TextMap;            ///< defines a map of Text attached to an identifier
    typedef  map<string, ofPtr<ImageVisual>  >    ImageMap;           ///< defines a map of ImageVisual Map attached to an identifier
    
    TextMap             m_textVisuals;             ///< map storing the text visuals attached to a name
    ImageMap            m_imageVisuals;            ///< map storing the image visuals attached to a name
    
    ofRectangle         m_windowRect;
    RectangleVisual     m_windowFrame;
    
    ofFbo               m_fbo;
    ofColor             m_color;
};

//==========================================================================


