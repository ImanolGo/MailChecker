/*
 *  MailManager.cpp
 *  Mail Checker App
 *
 *  Created by Imanol Gomez on 31/05/18.
 *
 */

#include "MailManager.h"
#include "AppManager.h"


MailManager::MailManager(): Manager(), m_currentAddress("imanolgo@gmail.com")
{
    //Intentionally left empty
}

MailManager::~MailManager()
{
   ofLogNotice() << "MailManager::destructor";
   ofUnregisterURLNotification(this);
}


//--------------------------------------------------------------

void MailManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    ofRegisterURLNotification(this);
    
    this->setupText();
    this->checkEmail(m_currentAddress);
    
    
    
    ofLogNotice() <<"MailManager::initialized" ;

    
}

void MailManager::setupText()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    float size = 60;
    float w = width -  4*LayoutManager::MARGIN;
    
    float h = size;
    float x = width*0.5;
    float y = height*0.5;
    ofPoint pos = ofPoint(x, y);
    string text = "Email";
    string fontName = LayoutManager::LAYOUT_FONT;
    
    m_text = TextVisual(pos,w,h,true);
    m_text.setText(text, fontName, size, ofColor::white);
}



bool MailManager::loadFile(string path)
{
    // Load a CSV File.
    if(m_csv.loadAndParse(path,",","#"))
    {
        ofLogNotice() <<"MailManager::loadFile -> Successfully loaded: "<< path ;
        ofLogNotice() <<"MailManager::loadFile -> csv rows: "<< m_csv.getNumRows() << ", csv cols: " << m_csv.getNumCols();
        return true;
        
    }
    else
    {
        ofLogNotice() <<"MailManager::loadFile -> Unable to load : "<< path ;
         return false;
    }
}

bool MailManager::reloadFile()
{
    return loadFile(m_csv.getPath());
}


void MailManager::loadFiles(vector <string>& files)
{
    for(unsigned int i = 0; i < files.size(); i++)
    {
        if(this->loadFile(files[i]))
        {
            break;
        }
    }
}

void MailManager::update()
{
    //this->updateText();
}

void MailManager::updateText()
{
   // m_text.setText(m_currentAddress);
}


void MailManager::draw()
{
    this->drawText();
}


void MailManager::drawText()
{
    m_text.draw();
}

void MailManager::checkEmail(const string& address)
{
    m_url = AppManager::getInstance().getSettingsManager().getUrl();
    m_url+= address;
    m_url+= "&token=";
    m_url+=AppManager::getInstance().getSettingsManager().getKey();
    
    ofLogNotice() <<"ApiManager::checkEmail -> url " << m_url;
    m_currentAddress = address;
    this->updateText();
    
    ofLoadURLAsync(m_url, m_currentAddress);
    
    ofHttpResponse resp = ofLoadURL("https://api.trumail.io/v2/lookups/xml?email=imanolgo@gmail.com&token=e2e9b949-a9e9-489b-9d8f-7f012564c742");
    ofLogNotice() <<"ApiManager::checkEmail -> DATA " <<  resp.data ;
    
}

void MailManager::urlResponse(ofHttpResponse & response)
{
    ofLogNotice() <<"ApiManager::urlResponse -> " << response.data << ", " << response.status;
    
    
    if(response.status==200)
    {
        ofLogNotice() <<"ApiManager::urlResponse -> " << response.request.name << ", " << response.status;
        
        //this->parseResult(response.data);
    }
}

void MailManager::parseResult(const string& data)
{
    ofLogNotice() <<"ApiManager::parseResult -> Data: " << data;
    ofXml xml;
    
    if(!xml.parse( data )){
        ofLogNotice() <<"ApiManager::parseResult << Unable to parse data: " << data;
        return;
    }
    
    string path = "//lookup/deliverable";
    auto xmlChild = xml.findFirst(path);
    bool derivable = xmlChild.getBoolValue();
    
    path = "//lookup/address";
    xmlChild = xml.findFirst(path);
    string address = xmlChild.getValue();
   
    if(derivable){
        ofLogNotice() <<"ApiManager::parseResult << Email " << address << " is DERIVABLE!";
    }
    else{
        ofLogNotice() <<"ApiManager::parseResult << Email " << address << " is NOT  DERIVABLE!";
    }
    
    
}



string MailManager::getName(const string& path)
{
    return ofSplitString(path, "/").back();
}



