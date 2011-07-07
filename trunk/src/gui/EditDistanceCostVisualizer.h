#ifndef EDITDISTANCECOSTVISUALIZER_H
#define EDITDISTANCECOSTVISUALIZER_H

#ifndef WX_PRECOMP
	//(*HeadersPCH(EditDistanceCostVisualizer)
	#include <wx/dialog.h>
	#include <wx/panel.h>
	//*)
#endif
//(*Headers(EditDistanceCostVisualizer)
//*)

class EditDistanceCostVisualizer: public wxDialog
{
	public:

		EditDistanceCostVisualizer(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~EditDistanceCostVisualizer();

		//(*Declarations(EditDistanceCostVisualizer)
		wxPanel* Panel1;
		//*)

	protected:

		//(*Identifiers(EditDistanceCostVisualizer)
		static const long ID_PANEL1;
		//*)

	private:

		//(*Handlers(EditDistanceCostVisualizer)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
