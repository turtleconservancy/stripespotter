#include "wx_pcx.h"
#include "EditDistanceCostVisualizer.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(EditDistanceCostVisualizer)
	#include <wx/string.h>
	#include <wx/intl.h>
	//*)
#endif
//(*InternalHeaders(EditDistanceCostVisualizer)
//*)

//(*IdInit(EditDistanceCostVisualizer)
const long EditDistanceCostVisualizer::ID_PANEL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(EditDistanceCostVisualizer,wxDialog)
	//(*EventTable(EditDistanceCostVisualizer)
	//*)
END_EVENT_TABLE()

EditDistanceCostVisualizer::EditDistanceCostVisualizer(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(EditDistanceCostVisualizer)
	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(56,48), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	//*)
}

EditDistanceCostVisualizer::~EditDistanceCostVisualizer()
{
	//(*Destroy(EditDistanceCostVisualizer)
	//*)
}

