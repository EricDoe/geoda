//
//  MapLayer.hpp
//  GeoDa
//
//  Created by Xun Li on 8/24/18.
//

#ifndef MapLayer_hpp
#define MapLayer_hpp

#include <wx/wx.h>

#include "../GdaShape.h"
#include "../ShapeOperations/OGRLayerProxy.h"

class BackgroundMapLayer
{
    Shapefile::ShapeType shape_type;
    
    wxColour pen_color;
    wxColour brush_color;
    int point_radius;
    int opacity;
    int pen_size;
    bool show_boundary;
    bool is_hide;
    
public:
    GdaPolygon* map_boundary;
    vector<GdaShape*> shapes;
    vector<OGRGeometry*> geoms;
    
    BackgroundMapLayer(OGRLayerProxy* layer_proxy, OGRSpatialReference* sr);
    ~BackgroundMapLayer();
    
    void SetHide(bool flag);
    bool IsHide();
    void SetPenColour(wxColour& color);
    void SetBrushColour(wxColour& color);
    void SetPointRadius(int radius);
    void SetOpacity(int opacity);
    void SetPenSize(int size);
    void ShowBoundary(bool show);
    wxColour GetBrushColour();
    wxColour GetPenColour();
    int GetPenSize();
    int GetPointRadius();
    int GetOpacity();
    bool IsShowBoundary();
    
    vector<GdaShape*>& GetShapes();
    Shapefile::ShapeType GetShapeType();
    
    void drawLegend(wxDC& dc, int x, int y, int w, int h);
};

class GdaShapeLayer : public GdaShape  {
    wxString name;
    BackgroundMapLayer* ml;
    vector<GdaShape*> shapes;
    GdaPolygon* map_boundary;
    
public:
    GdaShapeLayer(wxString name, BackgroundMapLayer* ml);
    ~GdaShapeLayer();
    
    virtual GdaShape* clone();
    virtual void Offset(double dx, double dy);
    virtual void Offset(int dx, int dy);
    virtual void applyScaleTrans(const GdaScaleTrans& A);
    virtual void projectToBasemap(GDA::Basemap* basemap, double scale_factor = 1.0);
    virtual void paintSelf(wxDC& dc);
    virtual void paintSelf(wxGraphicsContext* gc);
};


#endif /* MapLayer_hpp */
