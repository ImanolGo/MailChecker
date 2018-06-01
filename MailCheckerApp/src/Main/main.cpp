#include "ofMain.h"
#include "MailCheckerApp.h"

//========================================================================
int main( ){
    
    ofGLWindowSettings settings;
    settings.setGLVersion(2,1);
    ofCreateWindow(settings);
  
  ofRunApp( new MailCheckerApp());
   
    
}
