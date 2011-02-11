#ifndef WX_PCH_H_INCLUDED
#define WX_PCH_H_INCLUDED

#if ( !defined(WX_PRECOMP) )
    #define WX_PRECOMP
#endif

// basic wxWidgets headers
#include <wx/wxprec.h>
#include <wx/file.h>
#include <wx/dir.h>
#include <wx/imaglist.h>
#include <wx/progdlg.h>

// STL
#include <vector>
#include <map>
#include <string>
#include <algorithm>

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/file.h>
	#include <wx/dir.h>
	#include <wx/imaglist.h>
    #include <vector>
    #include <map>
#endif

// #include other rarely changing headers here

#endif // WX_PCH_H_INCLUDED
