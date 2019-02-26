/**
 * GeoDa TM, Copyright (C) 2011-2015 by Luc Anselin - all rights reserved
 *
 * This file is part of GeoDa.
 * 
 * GeoDa is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GeoDa is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __GEODA_CENTER_TABLE_BASE_H__
#define __GEODA_CENTER_TABLE_BASE_H__

#include <vector>
#include <wx/grid.h>
#include "../HighlightStateObserver.h"
#include "TableStateObserver.h"
#include "TimeStateObserver.h"

class Project;
class TableInterface;
class TableState;
class TimeState;
class HighlightState;
class TemplateFrame;

class wxGridCellInt64Renderer : public wxGridCellStringRenderer
{
    // formatting parameters
    int m_width;
    wxString m_format;
public:
    wxGridCellInt64Renderer(int width=19);

    int GetWidth() const { return m_width; }
    void SetWidth(int width) { m_width = width; }

    // draw the string right aligned
    virtual void Draw(wxGrid& grid,
                      wxGridCellAttr& attr,
                      wxDC& dc,
                      const wxRect& rect,
                      int row, int col,
                      bool isSelected) wxOVERRIDE;

    virtual wxSize GetBestSize(wxGrid& grid,
                               wxGridCellAttr& attr,
                               wxDC& dc,
                               int row, int col) wxOVERRIDE;

    virtual wxGridCellRenderer *Clone() const wxOVERRIDE;

protected:
    wxString GetString(const wxGrid& grid, int row, int col);
};

class wxGridCellInt64Editor : public wxGridCellTextEditor
{
public:
    wxGridCellInt64Editor(int width=19);

    virtual void Create(wxWindow* parent,
                        wxWindowID id,
                        wxEvtHandler* evtHandler) wxOVERRIDE;

    virtual bool IsAcceptedKey(wxKeyEvent& event) wxOVERRIDE;
    virtual void BeginEdit(int row, int col, wxGrid* grid) wxOVERRIDE;
    virtual bool EndEdit(int row, int col, const wxGrid* grid,
                         const wxString& oldval, wxString *newval) wxOVERRIDE;
    virtual void ApplyEdit(int row, int col, wxGrid* grid) wxOVERRIDE;

    virtual void Reset() wxOVERRIDE;
    virtual void StartingKey(wxKeyEvent& event) wxOVERRIDE;
    virtual wxGridCellEditor *Clone() const wxOVERRIDE
    { return new wxGridCellInt64Editor(m_width); }
    // added GetValue so we can get the value which is in the control
    virtual wxString GetValue() const wxOVERRIDE;

protected:
    // string representation of our value
    wxString GetString()
    { return wxString::Format(wxT("%lld"), m_value); }

private:
    int m_width;
    long long m_value;
};


class TableBase : public TableStateObserver, public TimeStateObserver,
public HighlightStateObserver,  public wxGridTableBase
{
public:
	TableBase(Project* _project, TemplateFrame* t_frame);
	virtual ~TableBase();
	
	virtual bool FromGridIsSelectedRow(int row);
	virtual void FromGridSelectOnlyRow(int row);
	virtual void FromGridSelectRowRange(int first_row, int last_row);
	virtual void FromGridSelectRow(int row);
	virtual void FromGridDeselectRow(int row);
	virtual void DeselectAllRows();
    //virtual void PushAnchorRow(int row);
	//virtual void RemoveAnchorRow(int row);
	//virtual int GetAnchorRow();
	
	virtual void SortByDefaultAscending();
	virtual void SortByDefaultDecending();
	virtual void SortByCol(int col, bool ascending);
	virtual void MoveSelectedToTop();
	virtual bool IsSortedAscending() { return sorting_ascending; }
	virtual int GetSortingCol() { return sorting_col; }

	virtual bool FromGridIsSelectedCol(int col);
	virtual void FromGridSelectCol(int col);
	virtual void FromGridDeselectCol(int col);
	virtual void DeselectAllCols(); 
    virtual void GetSelectedCols(std::vector<int>& sel_cols);
	//virtual void PushAnchorCol(int col);
	//virtual void RemoveAnchorCol(int col);
	//virtual int GetAnchorCol();
	
	// pure virtual method implementation for wxGridTableBase
	virtual int GetNumberRows();
	virtual int GetNumberCols();
	virtual wxString GetValue(int row, int col);
	virtual void SetValue(int row, int col, const wxString &value);
	virtual bool IsEmptyCell(int row, int col) { return false; }
	
	// methods called by wxGrid
	virtual wxString GetRowLabelValue(int row);
	virtual wxString GetColLabelValue(int col);	
	
	virtual void update(HLStateInt* o);
	virtual void update(TableState* o);
	virtual void update(TimeState* o);
	virtual bool AllowTimelineChanges() { return true; }
	virtual bool AllowGroupModify(const wxString& grp_nm) { return true; }
	virtual bool AllowObservationAddDelete() { return true; }
	
	virtual void notifyColMove();
	virtual TableInterface* GetTableInt();
	
    void UpdateStatusBar();
   
    template <class T>
    void SortColumn(int col, int tm, bool ascending);
    
    std::vector<int> GetRowOrder();
    
protected:
	HighlightState* highlight_state;
	std::vector<bool>& hs; //shortcut to HighlightState::highlight, read only!
    std::vector<bool> hs_col;
	TableState* table_state;
	TableInterface* table_int;
	TimeState* time_state;
    Project* project;
	TemplateFrame* template_frame;
    
	int rows;
    int cols;
	std::vector<int> row_order;
	int sorting_col;
	bool sorting_ascending;
	std::list<int> anchor_row_stack;
	std::list<int> anchor_col_stack;
};

#endif
