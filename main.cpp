/////////////////////////////////////////////////////////////////////////////
// Name:        p2psocket
// Purpose:     Minimal p2p wxWidgets sample
// Author:      marco_84
// Modified by:
// Created:     04/04/2020
// Copyright:   marco_84 (c) lgpl
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

// compiling on Ubuntu
// g++ main.cpp -o p2psocket.run $(wx-config --cxxflags wx-config --libs) -no-pie

// compiling on msw
// g++ main.cpp -o p2psocket.exe $(wx-config --cxxflags wx-config --libs)

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

//#include <vector>    // for stl::vector
//#include <list>    // for stl::list


//#include <wx/socket.h>
#include "main.h"

#include "p2psocket.h"



IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	mysocket * wxMysocket = new mysocket(wxT("p2p socket sample"));
	wxMysocket->Show(true);
    return true;
}


