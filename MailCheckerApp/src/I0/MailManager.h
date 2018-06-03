/*
 *  MailManager.h
 *  Mail Checker App
 *
 *  Created by Imanol Gomez on 31/05/18.
 *
 */

#pragma once

#include "ofMain.h"
#include "Manager.h"
#include "ofxCsv.h"
#include "TextVisual.h"


//========================== class MailManager =======================================
//==============================================================================
/** \class MailManager MailManager.h
 *	\brief class for managing the mail checker API
 *	\details It reads from a CSV file with the names and checks the e-mails using an API
 */


class MailManager: public Manager
{

public:
    //! Constructor
    MailManager();

    //! Destructor
    virtual ~MailManager();

    //! setups the subtitles
    void setup();
    
    void update();
    
    void draw();
    
    //! loads a file given a path
    bool loadFile(const string& path);
    
    //! reloads the current file
    bool reloadFile();
    
    //! loads a file given a list of files
    void loadFiles(vector <string>& files);
    
    void urlResponse(ofHttpResponse & response);
    
private:
    
    void setupText();
    
    void updateText();
    
    void drawText();
    
    string getName(const string& path);
    
    void checkEmail(const string& address);
    
    void parseResult(const string& result);
    
    bool loadCSV(const string& path);
    
    bool checkFormat();
    
    void startValidation();
    
    void validateNextRow();
    
    void saveResults();
    
    void createAddressList();
    
private:
    
    ofxCsv m_csv;
    vector<string>  m_addressList;
    
    string m_currentAddress;
    TextVisual   m_text;
    string       m_url;
    int          m_currentRow;
    string       m_path;
    int          m_emailIndex;
    int          m_domainIndex;
    int          m_firstNameIndex;
    int          m_lastNameIndex;
    int          m_lastNameIndex;
    
};

