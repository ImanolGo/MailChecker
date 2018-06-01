/*
 *  AppManager.cpp
 *  Mail Checker App
 *
 *  Created by Imanol Gomez on 31/05/18.
 *
 */


#include "ofMain.h"


#include "AppManager.h"

AppManager& AppManager::getInstance()
{
	// The only instance
	// Guaranteed to be lazy initialized
	// Guaranteed that it will be destroyed correctly
	static AppManager    m_instance;
    return m_instance;

}

AppManager::AppManager(): Manager(), m_debugMode(false)
{
   //Intentionally left empty
}


AppManager::~AppManager()
{
    ofLogNotice() <<"AppManager::Destructor";
}


void AppManager::setup()
{
	if(m_initialized)
		return;

    
    ofLogNotice() << "AppManager::initialized";

	Manager::setup();
    
    this->setupOF();
	this->setupManagers();
    
    ofSetLogLevel(OF_LOG_NOTICE);
    
    m_initialized = true;
    
    
    //setDebugMode(m_debugMode);
}

void AppManager::setupOF()
{
    ofSetLogLevel(OF_LOG_NOTICE);
    ofSetVerticalSync(true);
    ofSetEscapeQuitsApp(true);
    ofSetBackgroundAuto(true);
    ofTrueTypeFont::setGlobalDpi(72);
    ofSetDataPathRoot("../Resources/data/");
    
}


void AppManager::setupManagers()
{
 
    m_settingsManager.setup();
    m_resourceManager.setup();
    m_viewManager.setup();
    m_visualEffectsManager.setup();
    m_layoutManager.setup();
    m_mailManager.setup();
    m_guiManager.setup();
    
}

void AppManager::update()
{
    if(!m_initialized)
        return;
    
    m_visualEffectsManager.update();
    m_viewManager.update();
    m_layoutManager.update();
    m_mailManager.update();
    m_guiManager.update();
}


void AppManager::draw()
{
    if(!m_initialized)
        return;
    
    auto color = AppManager::getInstance().getSettingsManager().getColor("BackgroundColor");
    ofBackground(color);
    
    m_layoutManager.draw();
    m_guiManager.draw();

}


void AppManager::toggleDebugMode()
{
    m_debugMode = !m_debugMode;
    setDebugMode(m_debugMode);
}


void AppManager::exit()
{
    m_guiManager.saveGuiValues();
}

void AppManager::setDebugMode(bool showDebug)
{
    m_debugMode = showDebug;
    
    ofLogNotice()<<"AppManager::setDebugMode-> " << m_debugMode;
    
    if(m_debugMode){
        ofSetLogLevel(OF_LOG_VERBOSE);
    }
    else{
        ofSetLogLevel(OF_LOG_NOTICE);
    }
    

    m_guiManager.showGui(m_debugMode);
    
}




