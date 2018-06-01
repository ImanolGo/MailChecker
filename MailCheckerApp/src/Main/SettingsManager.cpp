/*
 *  SettingsManager.cpp
 *  Mail Checker App
 *
 *  Created by Imanol Gomez on 31/05/18.
 *
 */


#include "ofMain.h"

#include "SettingsManager.h"


const string SettingsManager::APPLICATION_SETTINGS_FILE_NAME = "xmls/ApplicationSettings.xml";


SettingsManager::SettingsManager(): Manager(), m_appHeight(0.0), m_appWidth(0.0)
{
    //Intentionally left empty
}


SettingsManager::~SettingsManager()
{
    ofLogNotice() <<"SettingsManager::Destructor" ;
}


void SettingsManager::setup()
{
    if(m_initialized)
        return;
    
    ofLogNotice() <<"SettingsManager::initialized" ;
    
    Manager::setup();
    
    if(this->loadSettingsFile()){
        this->loadAllSettings();
    }
}

void SettingsManager::loadAllSettings()
{
    this->setWindowProperties();
    this->setDebugProperties();
    this->setNetworkProperties();
    this->loadApiSettings();
    this->loadTextureSettings();
    this->loadColors();
}

bool SettingsManager::loadSettingsFile()
{
    
    if(!m_xml.load(APPLICATION_SETTINGS_FILE_NAME)){
        ofLogNotice() <<"SettingsManager::loadSettingsFile-> unable to load file: " << APPLICATION_SETTINGS_FILE_NAME ;
        return false;
    }
    
    ofLogNotice() <<"SettingsManager::loadSettingsFile->  successfully loaded " << APPLICATION_SETTINGS_FILE_NAME ;
    return true;
}

void SettingsManager::setDebugProperties()
{
    m_xml.setTo("//");
    
    string ofPath = "//of_settings/debug";
    if(m_xml.exists(ofPath)) {
        m_xml.setTo(ofPath);
        typedef   std::map<string, string>   AttributesMap;
        AttributesMap attributes = m_xml.getAttributes();
        
        bool showCursor = ofToBool(attributes["showCursor"]);
        if(showCursor){
            ofShowCursor();
        }
        else{
            ofHideCursor();
        }
        
        bool setVerbose = ofToBool(attributes["setVerbose"]);
        if(setVerbose){
            ofSetLogLevel(OF_LOG_VERBOSE);
        }
        else{
            ofSetLogLevel(OF_LOG_NOTICE);
        }
        
        
        ofLogNotice() <<"SettingsManager::setDebugProperties->  successfully loaded the OF general settings" ;
        return;
    }
    
    ofLogNotice() <<"SettingsManager::setOFProperties->  path not found: " << ofPath ;
}

void SettingsManager::setWindowProperties()
{
    m_xml.setTo("//");
    
    string windowPath = "//of_settings/window";
    if(m_xml.exists(windowPath)) {
        m_xml.setTo(windowPath);
        typedef   std::map<string, string>   AttributesMap;
        AttributesMap attributes = m_xml.getAttributes();
        string title = attributes["title"];
        m_appWidth = ofToInt(attributes["width"]);
        m_appHeight= ofToInt(attributes["height"]);
        
        //m_appWidth = ofGetScreenWidth();
        //m_appHeight = ofGetScreenHeight();
        
        int x = ofToInt(attributes["x"]);
        int y = ofToInt(attributes["y"]);
        bool fullscreen = ofToBool(attributes["fullscreen"]);
        
        ofSetFullscreen(fullscreen);
        //ofSetWindowShape(ofGetScreenWidth(),ofGetScreenHeight());
        if(!fullscreen){
            ofSetWindowPosition(x,y);
        }
        ofSetWindowTitle(title);
        
        ofLogNotice() <<"SettingsManager::setWindowProperties->  successfully loaded the window settings" ;
        ofLogNotice() <<"SettingsManager::setWindowProperties->  title = "<< title<<", width = " << m_appWidth <<", height = "
        <<m_appHeight <<", x = "<<x<<", y = "<<y;
        return;
    }
    
    ofLogNotice() <<"SettingsManager::setWindowProperties->  path not found: " << windowPath ;
}
void SettingsManager::setNetworkProperties()
{
    m_xml.setTo("//");
    
    string networkPath = "//of_settings/network";
    if(m_xml.exists(networkPath)) {
        m_xml.setTo(networkPath);
        typedef   std::map<string, string>   AttributesMap;
        AttributesMap attributes = m_xml.getAttributes();
        
        m_portOscSend  = ofToInt(attributes["portOscSend"]);
        m_portOscReceive  = ofToInt(attributes["portOscReceive"]);
        m_ipAddress  = ofToString(attributes["ipAddress"]);
        
        ofLogNotice() <<"SettingsManager::setNetworkProperties->  successfully loaded the network settings" ;
        return;
    }
    
    ofLogNotice() <<"SettingsManager::setNetworkProperties->  path not found: " << networkPath ;
}

void SettingsManager::loadApiSettings()
{
    
    m_xml.setTo("//");
    
    string path = "//api/trumail";
    if(m_xml.exists(path)) {
        m_xml.setTo(path);
        typedef   std::map<string, string>   AttributesMap;
        AttributesMap attributes = m_xml.getAttributes();
        
        m_url  = ofToString(attributes["url"]);
        m_key  = ofToString(attributes["key"]);
        
         ofLogNotice() <<"SettingsManager::loadApiSettings->  successfully loaded the api settings" ;
        return;
    }
    
    ofLogNotice() <<"SettingsManager::loadApiSettings->  path not found: " << path ;
}

void SettingsManager::loadColors()
{
    m_xml.setTo("//");
    
    string colorsSettingsPath = "//colors";
    if(m_xml.exists(colorsSettingsPath)) {
        
        typedef   std::map<string, string>   AttributesMap;
        AttributesMap attributes;
        
        colorsSettingsPath = "//colors/color[0]";
        m_xml.setTo(colorsSettingsPath);
        do {
            
            attributes = m_xml.getAttributes();
            
            int r = ofToInt(attributes["r"]);
            int g = ofToInt(attributes["g"]);
            int b = ofToInt(attributes["b"]);
            int a = ofToInt(attributes["a"]);
            
            ofPtr<ofColor> color = ofPtr<ofColor> (new ofColor(r,g,b,a));
            m_colors[attributes["name"]] = color;
            
            
            ofLogNotice() <<"SettingsManager::loadColors->  color = " << attributes["name"] <<", r = " << r
            <<", g = "<< g << ", b = " << b << ", a = " << a ;
        }
        while(m_xml.setToSibling()); // go to the next node
        
        
        ofLogNotice() <<"SettingsManager::loadColors->  successfully loaded the applications colors" ;
        return;
    }
    
    ofLogNotice() <<"SettingsManager::loadColors->  path not found: " << colorsSettingsPath ;
}

void SettingsManager::loadTextureSettings()
{
    m_xml.setTo("//");
    
    string resourcesPath = "//textures";
    if(m_xml.exists(resourcesPath)) {
        
        typedef   std::map<string, string>   AttributesMap;
        AttributesMap attributes;
        
        resourcesPath = "//textures/texture[0]";
        m_xml.setTo(resourcesPath);
        do {
            
            attributes = m_xml.getAttributes();
            m_texturesPath[attributes["name"]] = attributes["path"];
            
            ofLogNotice() <<"SettingsManager::loadTextureSettings->  texture = " << attributes["name"]
            <<", path = "<< attributes["path"] ;
        }
        while(m_xml.setToSibling()); // go to the next texture
        
        
        ofLogNotice() <<"SettingsManager::loadTextureSettings->  successfully loaded the resource settings" ;
        return;
    }
    
    ofLogNotice() <<"SettingsManager::loadTextureSettings->  path not found: " << resourcesPath ;
}

ofColor SettingsManager::getColor(const string& colorName)
{
    ofColor color;
    if(m_colors.find(colorName)!= m_colors.end()){
        color.setHex(m_colors[colorName]->getHex());
    }
    
    return color;
}





