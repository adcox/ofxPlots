/**
 * @file ofxPlot.hpp
 * 
 * @author Andrew Cox
 * @version July 5, 2016
 * 
 * Copyright (c) 2016 Andrew Cox
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in 
 * the Software without restriction, including without limitation the rights to use, 
 * copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the 
 * Software, and to permit persons to whom the Software is furnished to do so, subject 
 * to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies 
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR 
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include "ofxPlotArrow.hpp"

#include "ofMain.h"

#include <string>
#include <vector>


/**
 *  @brief Structure that contains values for a single data point
 */
struct dataPt{
	
	/**
	 *  @brief Construct a default data point
	 */
	dataPt(){}

	/**
	 *  @brief Construct a data point
	 * 
	 *  @param i independent variable value
	 *  @param d dependent variable value
	 */
	dataPt(double i, double d) : indVar(i), depVar(d) {}

	double indVar = 0;		//!< Independent Variable (e.g., time)
	double depVar = 0;		//!< Dependent variable (e.g., altitude)
};

/**
 *  @brief Plot object
 */
class ofxPlot{

public:
	ofxPlot();

	void draw();

	void addDataPt(double, double);
	void addDataPt(dataPt);

	void setFillPlot(bool);
	void setPosition(float, float);
	void setPosition(ofVec2f);
	void setSize(float, float);
	void setSize(ofVec2f);
	void setXLabel(std::string);
	void setYLabel(std::string);
	void setTitle(std::string);
	
protected:
	std::vector<dataPt> data;		//!< Stores all data values

	std::string xlabel = "";		//!< Horizontal axis label
	std::string ylabel = "";		//!< Vertical axis label
	std::string title = "";			//!< Plot title

	bool fillPlot = false;			//!< Whether or not to fill the plot area

	ofColor lineColor = ofColor(125, 125, 255, 255);		//!< Color of plotted line
	ofColor fillColor = ofColor(200, 200, 255, 0.9*255);	//!< Color of fill under plotted line
	ofColor bgColor = ofColor(50, 50, 50, 0.95*255);		//!< Plot area background color
	ofColor axesColor = ofColor(200, 200, 200, 0.9*255);	//!< Axes color
	ofColor textColor = ofColor(200, 200, 200, 255);		//!< Axes label and title color

	float axesWidth = 3;	//!< Axes line width
	float lineWidth = 2;	//!< Plotted data line width
	float padding = 25;		//!< Distance between outer rectangle and plot (i.e., with data) area

	ofRectangle viewport = ofRectangle(0, 0, 350, 350);		//!< Describes the area the plot occupies in screen space
	ofxPlotArrow indAxis, depAxis;	//!< Axes arrows
};