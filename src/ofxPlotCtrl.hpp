#pragma once

#include "interactiveObj.hpp"
#include "ofxPlotCtrlLimiter.hpp"

class ofxPlotCtrl : public InteractiveObj{
public:
	ofxPlotCtrl();
	ofxPlotCtrl(float, float);

	void update();
	void draw();

	void addDataPt(double);

	void setData(std::vector<double>);

	void setHighlightedPts(std::vector<int>);

	void setLink_lowerLimit(float*);
	void setLink_upperLimit(float*);

	// void setLowerLimitVal(float);
	// void setUpperLimitVal(float);

	void dataDeselected(DataSelectedEventArgs&);
	void dataSelected(DataSelectedEventArgs&);
protected:
	std::vector<double> indVars;		//!< vector of all the independent variable values
	std::vector<int> highlightPtIxs;	//!< Data points to highlight

	float limitValLower = 0;			//!< Lower limit on the independent variable, same units as independent variable
	float limitValUpper = 1;			//!< Upper limit on the independent variable, same units as independent variable

	float *link_limitValLower = nullptr;
	float *link_limitValUpper = nullptr;

	float scale = 1.0;					//!< Ratio of pixels:data

	ofRectangle timelineRect = ofRectangle(0,0,10,10);
	ofColor activeColor = ofColor(225, 225, 225, 0.8*255);
	ofColor inactiveColor = ofColor(125, 125, 125, 0.8*255);

	ofxPlotCtrlLimiter limitLower, limitUpper;

	void init();
};