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


MailManager::MailManager(): Manager(), m_currentAddress("imanol.gomez@thepowerhouse.group"), m_currentRow(0), m_emailIndex(0), m_domainIndex(0),
m_firstNameIndex(0), m_lastNameIndex(0), m_deliverabilityIndex(0), m_deliverable(true), m_permutationIndex(0)
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
    this->checkEmail(m_currentAddress, "GivenAddress");
    
    ofLogNotice() <<"MailManager::initialized" ;

}

void MailManager::setupText()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    float size = 40;
    float w = width -  4*LayoutManager::MARGIN;
    
    float h = size;
    float x = width*0.5;
    float y = 2*height/3;
    ofPoint pos = ofPoint(x, y);
    string text = "Email";
    string fontName = LayoutManager::LAYOUT_FONT;
    
    m_currentAddressText = TextVisual(pos,w,h,true);
    m_currentAddressText.setText(text, fontName, size, ofColor::white);
    
    size/=2;
    h = size;
    x = width*0.5;
    y = 1*height/3;
    pos = ofPoint(x, y);
    text = " ";
    
    m_previousAddressText = TextVisual(pos,w,h,true);
    m_previousAddressText.setText(text, fontName, size, ofColor::white);
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
    
    if(m_csv.getRow(0).getString(0) == "ID"){
        ofLogNotice() <<"MailManager::checkFormat -> CSV formatetd correclty ";
        this->getIndexes();
        return true;
    }
    
    ofLogNotice() <<"MailManager::checkFormat -> Not valid CSV format";
    return false;
}

void MailManager::getIndexes()
{
    for(int j = 0; j < m_csv.getNumCols(0); j++)
    {
        if(m_csv[0][j] == "FIRST NAME"){
            m_firstNameIndex = j;
             ofLogNotice() <<"MailManager::getIndexes -> First Name Index: " << m_firstNameIndex;
            
        }
        else if(m_csv[0][j] == "LAST NAME"){
            m_lastNameIndex = j;
            ofLogNotice() <<"MailManager::getIndexes -> Last Name Index: " << m_lastNameIndex;
            
        }
        
        else if(m_csv[0][j] == "EMAIL"){
            m_emailIndex = j;
            ofLogNotice() <<"MailManager::getIndexes -> Email Index: " << m_emailIndex;
            
        }
        
        else if(m_csv[0][j] == "DELIVERABILITY"){
            m_deliverabilityIndex = j;
            ofLogNotice() <<"MailManager::getIndexes -> Deliverability Index: " << m_deliverabilityIndex;
            
        }
        
        else if(m_csv[0][j] == "DOMAIN"){
            m_domainIndex = j;
            ofLogNotice() <<"MailManager::getIndexes -> Domain Index: " << m_domainIndex;
            
        }
        
    }
}

void MailManager::startValidation()
{
    m_currentRow = 0;
    this->validateNextRow();
}

void MailManager::startPermutation()
{
    m_permutationIndex = -1;
    this->createAddressList();
    this->validateNextPermutation();
}



void MailManager::validateNextPermutation()
{
    m_permutationIndex++;
    ofLogNotice() <<"MailManager::validateNextPermutation: permutation num = " << m_permutationIndex;
    
    if(m_permutationIndex>=m_addressList.size()){
        ofLogNotice() <<"MailManager::validateNextPermutation -> FINISHED!!";
        this->saveResults();
        this->validateNextRow();
        return;
    }
    
    this->checkEmail(m_addressList[m_permutationIndex], "PermutationAddress");
}


void MailManager::validateNextRow()
{
    m_currentRow++;
    ofLogNotice() <<"MailManager::validateNextRow: row num = " << m_currentRow;
    
    while(m_currentRow<m_csv.getNumRows()){
        string address = m_csv.getRow(m_currentRow).getString(m_emailIndex);
        string deliverability = m_csv.getRow(m_currentRow).getString(m_deliverabilityIndex);
        if(address.empty()){
            ofLogNotice() <<"MailManager::validateCurretRow -> Empty email address ";
            this->startPermutation();
            break;
        }
       else{
           if(deliverability.empty()){
               ofLogNotice() <<"MailManager::validateCurretRow -> Verifying email address: " << address;
               this->checkEmail(address, "GivenAddress");
               this->saveResults();
               break;
           }
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
    string first = ofToLower(m_csv.getRow(m_currentRow).getString(m_firstNameIndex));
    ofStringReplace(first," ", "");
    
    string last = ofToLower(m_csv.getRow(m_currentRow).getString(m_lastNameIndex));
    ofStringReplace(last," ", "");
    
    string domain = m_csv.getRow(m_currentRow).getString(m_domainIndex);
    
    
    ofLogNotice() <<"MailManager::createAddressList -> First Name: " << first << ", Last Name: " << last << ", Domain: " << domain;
    
    //Simple
    string address = first + "@" + domain; m_addressList.push_back(address);
    address = last + "@" + domain; m_addressList.push_back(address);
    
    //Basic
    address = first + last + "@" + domain; m_addressList.push_back(address);
    address = first + "." + last + "@" + domain; m_addressList.push_back(address);
    address = string(1,first[0]) + last + "@" + domain; m_addressList.push_back(address);
    address = string(1,first[0]) + "." + last + "@" + domain; m_addressList.push_back(address);
    address = first + string(1,last[0]) + "@" + domain; m_addressList.push_back(address);
    address = first + "." + string(1,last[0]) + "@" + domain; m_addressList.push_back(address);
    address = string(1,first[0]) + string(1,last[0]) + "@" + domain; m_addressList.push_back(address);
    address = string(1,first[0]) + "." + string(1,last[0]) + "@" + domain; m_addressList.push_back(address);
    
    //Backwards
    address = last + first + "@" + domain; m_addressList.push_back(address);
    address = last + "." + first + "@" + domain; m_addressList.push_back(address);
    address = string(1,last[0]) + first + "@" + domain; m_addressList.push_back(address);
    address = string(1,last[0]) + "." + first + "@" + domain; m_addressList.push_back(address);
    address = last + string(1,first[0]) + "@" + domain; m_addressList.push_back(address);
    address = last + "." + string(1,first[0]) + "@" + domain; m_addressList.push_back(address);
    address = string(1,last[0]) + string(1,first[0]) + "@" + domain; m_addressList.push_back(address);
    address = string(1,last[0]) + "." + string(1,first[0]) + "@" + domain; m_addressList.push_back(address);
    
    //Dashes
    address = first + "-" + last + "@" + domain; m_addressList.push_back(address);
    address = string(1,first[0]) + "-" + last + "@" + domain; m_addressList.push_back(address);
    address = first + "-" + string(1,last[0]) + "@" + domain; m_addressList.push_back(address);
    address = string(1,first[0]) + "-" + string(1,last[0]) + "@" + domain; m_addressList.push_back(address);
    address = last + "-" + first + "@" + domain; m_addressList.push_back(address);
    address = string(1,last[0]) + "-" + first + "@" + domain; m_addressList.push_back(address);
    address = last + "-" + string(1,first[0]) + "@" + domain; m_addressList.push_back(address);
    address = string(1,last[0]) + "-" + string(1,first[0]) + "@" + domain; m_addressList.push_back(address);
    
    //Underscore
    address = first + "_" + last + "@" + domain; m_addressList.push_back(address);
    address = string(1,first[0]) + "_" + last + "@" + domain; m_addressList.push_back(address);
    address = first + "-" + string(1,last[0]) + "@" + domain; m_addressList.push_back(address);
    address = string(1,first[0]) + "_" + string(1,last[0]) + "@" + domain; m_addressList.push_back(address);
    address = last + "_" + first + "@" + domain; m_addressList.push_back(address);
    address = string(1,last[0]) + "_" + first + "@" + domain; m_addressList.push_back(address);
    address = last + "_" + string(1,first[0]) + "@" + domain; m_addressList.push_back(address);
    address = string(1,last[0]) + "_" + string(1,first[0]) + "@" + domain; m_addressList.push_back(address);
    
    for(auto& address: m_addressList){
         ofLogNotice() <<"MailManager::createAddressList -> Address: " << address;
    }
    
    
    
    
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
   
    m_currentAddressText.setText(m_currentAddress);
    
    m_previousAddressText.setText(m_previousAddress);
    if(m_deliverable){
        m_previousAddressText.setColor(ofColor::green);
    }
    else{
        m_previousAddressText.setColor(ofColor::red);
    }
}


void MailManager::draw()
{
    this->drawText();
}


void MailManager::drawText()
{
    m_currentAddressText.draw();
    m_previousAddressText.draw();

}

void MailManager::checkEmail(const string& address, const string& requestName)
{
    m_url = AppManager::getInstance().getSettingsManager().getUrl();
    m_url+= address;
    m_url+= "&token=";
    m_url+=AppManager::getInstance().getSettingsManager().getKey();
    
    ofLogNotice() <<"ApiManager::checkEmail -> address " << address << ", request name: " <<requestName ;
    ofLogNotice() <<"ApiManager::checkEmail -> url " << m_url;
    m_previousAddress = m_currentAddress;
    m_currentAddress = address;
    this->updateText();
    
    ofLoadURLAsync(m_url, requestName);
    
    //ofHttpResponse resp = ofLoadURL("https://api.trumail.io/v2/lookups/xml?email=imanolgo@gmail.com&token=e2e9b949-a9e9-489b-9d8f-7f012564c742");
    //ofLogNotice() <<"ApiManager::checkEmail -> DATA " <<  resp.data ;
    
}

void MailManager::urlResponse(ofHttpResponse & response)
{
    //ofLogNotice() <<"ApiManager::urlResponse -> " << response.data << ", " << response.status;
    
    
    if(response.request.name=="GivenAddress")
    {
        if(response.status==200)
        {
            ofLogNotice() <<"ApiManager::urlResponse -> " << response.request.name << ", " << response.status;
            this->parseResult(response.data);
            if(m_deliverable){
                this->validateNextRow();
            }
            else{
                this->startPermutation();
            }
        }
        
        else if(response.status==500){
            ofLogNotice() <<"ApiManager::urlResponse -> " << response.request.name << ", " << response.status;
            this->setDeliverable(false, m_currentAddress);
            this->startPermutation();
        }
    }
    else if(response.request.name=="PermutationAddress"){
        
        if(response.status==200)
        {
            ofLogNotice() <<"ApiManager::urlResponse -> " << response.request.name << ", " << response.status;
            this->parseResult(response.data);
            
            if(m_deliverable){
                this->saveResults();
                this->validateNextRow();
            }
            else{
                this->validateNextPermutation();
            }
        }
        
        else if(response.status==500){
            ofLogNotice() <<"ApiManager::urlResponse -> " << response.request.name << ", " << response.status;
            this->setDeliverable(false, m_currentAddress);
            this->validateNextPermutation();
        }
    }
}

void MailManager::setDeliverable(bool value, string address)
{
    m_deliverable = value;
    m_previousAddress = address;
    this->updateText();
    
    if(m_deliverable){
         ofLogNotice() <<"ApiManager::setDeliverable << Email " << m_currentAddress << " is DELIVERABLE!";
         m_csv.getRow(m_currentRow).setString(m_deliverabilityIndex,"True");
    }
    else{
          ofLogNotice() <<"ApiManager::setDeliverable << Email " << m_currentAddress << " is NOT  DELIVERABLE!";
          m_csv.getRow(m_currentRow).setString(m_deliverabilityIndex,"False");
          //this->startPermutation();
    }
    
     m_csv.getRow(m_currentRow).setString(m_emailIndex,address);
    
}


void MailManager::parseResult(const string& data)
{
    ofLogNotice() <<"ApiManager::parseResult -> Data: " << data;
    ofxJSONElement json;
    
    if(json.parse(data)){
        bool derivable = json["deliverable"].asBool();
        string address = json["address"].asString();
        this->setDeliverable(derivable, address);
        
    }
    else{
        ofLogNotice() <<"ApiManager::parseResult << unable to parse json ";
    }
}



string MailManager::getName(const string& path)
{
    return ofSplitString(path, "/").back();
}



