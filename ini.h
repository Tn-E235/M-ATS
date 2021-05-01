#pragma once
//
// Generated on 2021/05/01 by inimoni 
//

/*
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Documentation
  Struct = Section name, Field = Key name.
  The function used is only a declaration, load(), and save().
  The constructor automatically reads initialization file [initFileName].
  Please rewrite [initFileName] in the source freely.
  It becomes Windows folder when there is not path. The relative path can be used.

Example
#include "SampleIni.h"
void CTestDlg::OnButton1()
{
    //initFileName = "./sample.ini";
    SampleIni data1;                   // Open ./sample.ini
    int v = data1.Sample.value;        // [Sample]-value Get
    data1.Sample.value = v+1;          // [Sample]-value Modify
    data1.save();                      // Save ./Sample.ini (Opened file)

    SampleIni data2("sample.ini");     // Open C:/WINDOWS/Sample.ini
    data2.load("./sample.ini");        // Open Specified file.
    data2.save();                      // Save Opened file (Making at not being)
}

Supplementation
  Reading and writing becomes the unit of the file.
  [initFileName] is the same as the argument of GetPrivateProfileString() of SDK.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/

#ifndef INI_H
#define INI_H

#include <string>
using namespace std;

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#ifdef _UNICODE
typedef std::wstring string_t;
#else
typedef std::string string_t;
#endif

namespace inimoni {
    template<class T>
    static bool inirw(int is_read, string_t& fname, string_t sec, string_t key, T& val_t);
};

/**
    aaa.ini input output class.
*/
class readIni
{
public:

    readIni(string_t fname = _T(""))
    {
        initFileName = _T("./M-ATS.ini"); //Default file.    
        init();
        load(fname);
    }

    /**
        INI file is read.
        @param fname Filename
        When there is not a file, It becomes initFileName.
        When there is not a pass, It becomes Windows folder.
    */
    bool load(string_t fname = _T(""))
    {
        if (fname.empty()) {
            fname = initFileName;
        }
        loadFileName = fname;
        WIN32_FIND_DATA fd;
        HANDLE h = ::FindFirstFile(fname.c_str(), &fd);
        if (h != INVALID_HANDLE_VALUE) {
            iniRW(fname, 1);
        }
        return (h != INVALID_HANDLE_VALUE);
    }

    /**
        It writes it in the INI file.
        @param fname Filename
        When there is not a file, It becomes open file.
        When there is not a pass, It becomes Windows folder.
    */
    bool save(string_t fname = _T(""))
    {
        if (fname.empty()) {
            fname = loadFileName;
        }
        iniRW(fname, 0);
        return true;
    }

public:

    // GENERAL
    struct _GENERAL
    {
        int      atsp;
    } GENERAL;

    // PANEL_MATS
    struct _PANEL_MATS
    {
        int      mats_brake;
    } PANEL_MATS;

    // PANEL_ATSP
    struct _PANEL_ATSP
    {
        int      platformDoor;
        int      platformDoor_break;
        int      atsp_enable;
        int      atsp_pattern;
        int      atsp_brake;
        int      atsp_break;
        int      atsp_power;
        int      atsp_release;
    } PANEL_ATSP;

    // SOUND
    struct _SOUND
    {
        int      patternBell;
    } SOUND;

protected:

    string_t initFileName;
    string_t loadFileName;

    bool iniRW(string_t f, int r)
    {
        string_t s;

        s = _T("GENERAL");
        inimoni::inirw(r, f, s, _T("atsp             "), GENERAL.atsp);

        s = _T("PANEL_MATS");
        inimoni::inirw(r, f, s, _T("mats_brake       "), PANEL_MATS.mats_brake);

        s = _T("PANEL_ATSP");
        inimoni::inirw(r, f, s, _T("platformDoor     "), PANEL_ATSP.platformDoor);
        inimoni::inirw(r, f, s, _T("platformDoor_break"), PANEL_ATSP.platformDoor_break);
        inimoni::inirw(r, f, s, _T("atsp_enable      "), PANEL_ATSP.atsp_enable);
        inimoni::inirw(r, f, s, _T("atsp_pattern     "), PANEL_ATSP.atsp_pattern);
        inimoni::inirw(r, f, s, _T("atsp_brake       "), PANEL_ATSP.atsp_brake);
        inimoni::inirw(r, f, s, _T("atsp_break       "), PANEL_ATSP.atsp_break);
        inimoni::inirw(r, f, s, _T("atsp_power       "), PANEL_ATSP.atsp_power);
        inimoni::inirw(r, f, s, _T("atsp_release     "), PANEL_ATSP.atsp_release);

        s = _T("SOUND");
        inimoni::inirw(r, f, s, _T("patternBell      "), SOUND.patternBell);
        return true;
    }

    void init()
    {
        GENERAL.atsp = 0;
        PANEL_MATS.mats_brake = -1;
        PANEL_ATSP.platformDoor = -1;
        PANEL_ATSP.platformDoor_break = -1;
        PANEL_ATSP.atsp_enable = -1;
        PANEL_ATSP.atsp_pattern = -1;
        PANEL_ATSP.atsp_brake = -1;
        PANEL_ATSP.atsp_break = -1;
        PANEL_ATSP.atsp_power = -1;
        PANEL_ATSP.atsp_release = -1;
        SOUND.patternBell = -1;
    }
};
typedef readIni AaaFile; //êVãåå›ä∑

//---------------------------------------------------------------------------
// Common method                                                             
//---------------------------------------------------------------------------
#ifndef INIMONI_INIRW                                                        
#define INIMONI_INIRW                                                        
namespace inimoni
{
    /*
    Read and Write INI file
      int     is_read  1=Read mode, 0=Write mode
      string  fname    Filename (The Windows folder when there is not path)
      string  sec      Section name
      string  key      Key name
      T       val_t    [Read]Init+Output, [Write]Input
    */
    template<class T>
    bool inirw(int is_read, string_t& fname, string_t sec, string_t key, T& val_t)
    {
        if (is_read) {
            inimoni::read(fname.c_str(), sec.c_str(), key.c_str(), val_t);
        }
        else {
            inimoni::write(fname.c_str(), sec.c_str(), key.c_str(), val_t);
        }
        return true;
    }

    bool read(string_t ifn, string_t sec, string_t key, int& dst)
    {
        dst = GetPrivateProfileInt(sec.c_str(), key.c_str(), dst, ifn.c_str());
        return true;
    }

    bool read(string_t ifn, string_t sec, string_t key, basic_string<TCHAR>& dst)
    {
        TCHAR buf[256];
        GetPrivateProfileString(
            sec.c_str(),
            key.c_str(),
            dst.c_str(),
            buf,
            sizeof(buf),
            ifn.c_str());
        dst = buf;
        return true;
    }

    bool read(string_t ifn, string_t sec, string_t key, double& dst)
    {
        string_t s;
        inimoni::read(ifn, sec, key, s);

        TCHAR* e;
        double x = _tcstod(s.c_str(), &e);

        dst = 0.0;
        if (!*e) {
            dst = x;
        }
        return true;
    }

    template<class T>
    bool write(string_t ifn, string_t sec, string_t key, T val_t)
    {
        TCHAR val[1024];
        inimoni::to_string(val, val_t);
        WritePrivateProfileString(sec.c_str(), key.c_str(), val, ifn.c_str());
        return true;
    }

    void to_string(TCHAR* str, int val)
    {
        _stprintf(str, _T("%d"), val);
    }

    void to_string(TCHAR* str, double val)
    {
        _stprintf(str, _T("%f"), val);
    }

    void to_string(TCHAR* str, basic_string<TCHAR> val)
    {
        _stprintf(str, _T("%s"), val.c_str());
    }
};

#endif                                                                  
#endif                                                                  

