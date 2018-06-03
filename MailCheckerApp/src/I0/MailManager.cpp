/*
 *  MailManager.cpp
 *  Mail Checker App
 *
 *  Created by Imanol Gomez on 31/05/18.
 *
 */

#include "MailManager.h"
#include "AppManager.h"
#include "ofxJSON.h"


MailManager::MailManager(): Manager(), m_currentAddress("imanolgo@gmail.com"), m_currentRow(0)
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
    
    float size = 50;
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



bool MailManager::loadFile(const string& path)
{
    if(this->loadCSV(path) &&  this->checkFormat()){
        this->startValidation();
        return true;
    }
    else{
        return false;
    }
}

bool MailManager::loadCSV(const string& path)
{
    // Load a CSV File.
    if(m_csv.loadAndParse(path,";","#"))
    {
        ofLogNotice() <<"MailManager::loadFile -> Successfully loaded: "<< path ;
        ofLogNotice() <<"MailManager::loadFile -> csv rows: "<< m_csv.getNumRows() << ", csv cols: " << m_csv.getNumCols();
        m_path = path;
        return true;
        
    }
    else
    {
        ofLogNotice() <<"MailManager::loadFile -> Unable to load : "<< path ;
        return false;
    }
}

bool MailManager::checkFormat()
{
    if(m_csv.getNumRows() <= 0 || m_csv.getNumCols() <= 0){
        return false;
    }
    
    if(m_csv.getRow(0).getString(0) == "CONTACTED"){
        ofLogNotice() <<"MailManager::checkFormat -> CSV formatetd correclty ";
        return true;
    }
    
    ofLogNotice() <<"MailManager::checkFormat -> Not valid CSV format";
    return false;
}

void MailManager::startValidation()
{
    m_currentRow = 0;
    this->validateNextRow();
   
}

void MailManager::validateNextRow()
{
    m_currentRow++;
    ofLogNotice() <<"MailManager::validateNextRow: row num = " << m_currentRow;
    
    while(m_currentRow<m_csv.getNumRows()){
        string address = m_csv.getRow(m_currentRow).getString(7);
        if(address!=""){
            ofLogNotice() <<"MailManager::validateCurretRow -> Verifying email address: " << address;
            this->checkEmail(address);
            break;
        }
        
        m_currentRow++;
        ofLogNotice() <<"MailManager::validateNextRow: row num = " << m_currentRow;
    }
    
    if(m_currentRow>=m_csv.getNumRows()){
        ofLogNotice() <<"MailManager::validateCurretRow -> FINISHED!!";
        this->saveResults();
        return;
    }
}

void MailManager::createAddressList()
{
    m_addressList.clear();
    string first = m_csv.getRow(m_currentRow).getString();
    last;
    
}

void MailManager::saveResults()
{
    if( m_csv.save(m_path)){
        ofLogNotice() <<"MailManager::saveResults -> Successfully saved: " << m_path;
    }
    else{
        ofLogNotice() <<"MailManager::saveResults ->Unable to save: " << m_path;
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
    m_text.setText(m_currentAddress);
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
    
    //ofHttpResponse resp = ofLoadURL("https://api.trumail.io/v2/lookups/xml?email=imanolgo@gmail.com&token=e2e9b949-a9e9-489b-9d8f-7f012564c742");
    //ofLogNotice() <<"ApiManager::checkEmail -> DATA " <<  resp.data ;
    
}

void MailManager::urlResponse(ofHttpResponse & response)
{
    //ofLogNotice() <<"ApiManager::urlResponse -> " << response.data << ", " << response.status;
    
    
    if(response.status==200)
    {
        ofLogNotice() <<"ApiManager::urlResponse -> " << response.request.name << ", " << response.status;
        this->parseResult(response.data);
        this->validateNextRow();
    }
    else if(response.status==500){
        ofLogNotice() <<"ApiManager::urlResponse -> " << response.request.name << ", " << response.status;
        m_csv.getRow(m_currentRow).setString(8,"False");
        this->validateNextRow();
    }
}

void MailManager::parseResult(const string& data)
{
    ofLogNotice() <<"ApiManager::parseResult -> Data: " << data;
    ofxJSONElement json;
    
    if(json.parse(data)){
        bool derivable = json["deliverable"].asBool();
        string address = json["address"].asString();
        
        if(derivable){
            ofLogNotice() <<"ApiManager::parseResult << Email " << address << " is DELIVERABLE!";
            m_csv.getRow(m_currentRow).setString(8,"True");
        }
        else{
            ofLogNotice() <<"ApiManager::parseResult << Email " << address << " is NOT  DELIVERABLE!";
                m_csv.getRow(m_currentRow).setString(8,"False");
        }
    }
    else{
        ofLogNotice() <<"ApiManager::parseResult << unable to parse json ";
    }
}



string MailManager::getName(const string& path)
{
    return ofSplitString(path, "/").back();
}



