/*
 *  MailCheckerApp.cpp
 *  Mail Checker App
 *
 *  Created by Imanol Gomez on 31/05/18.
 *
 */




#include "AppManager.h"

#include "MailCheckerApp.h"

//--------------------------------------------------------------
void MailCheckerApp::setup(){
    //ofSetDataPathRoot("../Resources/data/");
    AppManager::getInstance().setup();
}

//--------------------------------------------------------------
void MailCheckerApp::update(){
    AppManager::getInstance().update();
}

//--------------------------------------------------------------
void MailCheckerApp::draw(){
    AppManager::getInstance().draw();
}

void MailCheckerApp::exit()
{
    ofLogNotice() <<"MailCheckerApp::exit";
    AppManager::getInstance().exit();
}

//--------------------------------------------------------------
void MailCheckerApp::keyPressed(int key){

}

//--------------------------------------------------------------
void MailCheckerApp::keyReleased(int key){

}

//--------------------------------------------------------------
void MailCheckerApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void MailCheckerApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void MailCheckerApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void MailCheckerApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void MailCheckerApp::windowResized(int w, int h)
{
    AppManager::getInstance().getLayoutManager().windowResized(w,h);
}

//--------------------------------------------------------------
void MailCheckerApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void MailCheckerApp::dragEvent(ofDragInfo dragInfo)
{
    AppManager::getInstance().getMailManager().loadFiles(dragInfo.files);
}
