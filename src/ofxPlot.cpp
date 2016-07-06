/**
 * @file ofxPlot.cpp
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

#include "ofxPlot.hpp"

#include <algorithm>

/**
 *  @brief Construct a default ofxPlot object
 */
ofxPlot::ofxPlot(){
	indAxis.setColor(axesColor);
	depAxis.setColor(axesColor);

	indAxis.setLineWidth(axesWidth);
	depAxis.setLineWidth(axesWidth);
}//====================================================

/**
 *  @brief Draw the plot
 */
void ofxPlot::draw(){
	ofDisableDepthTest();
	ofPushStyle();

	// Draw background
	ofSetColor(bgColor);
	ofDrawRectangle(viewport);

	// Plot window properties
	float x = viewport.getX();			// top-left corner, increases to the right
	float y = viewport.getY();			// top-left corner, increases to the bottom
	float w = viewport.getWidth();
	float h = viewport.getHeight();

    // Characteristics of the area that information is plotted in
    float plot_w = w - 2*padding;       // Width of the area within the axes
    float plot_h = h - 2*padding;       // Height of the area within the axes
    float plot_x = x + padding;         // Bottom-left corner of area within the axes
    float plot_y = y + h - padding;     // Bottom-left corner of the area within the axes
    
    // Get maximum data extents
    float maxX = 0, minX = 0, maxY = 0, minY = 0;
    for(size_t i = 0; i < data.size(); i++){
        if(i == 0){
            maxX = data[i].indVar;
            minX = maxX;
            maxY = data[i].depVar;
            minY = maxY;
        }else{
            maxX = data[i].indVar > maxX ? data[i].indVar : maxX;
            minX = data[i].indVar < minX ? data[i].indVar : minX;
            maxY = data[i].depVar > maxY ? data[i].depVar : maxY;
            minY = data[i].depVar < minY ? data[i].depVar : minY;
        }
    }
    
    // Determine scaling amounts so that data will fit in plot area
    float xScale = std::max(std::floor(plot_w/(maxX - minX)), 1.f);
    float yScale = std::max(std::floor(plot_h/(maxY - minY)), 1.f);
    
    // Place axes to use as much of the plot area as possible
    float dataOrigin_x = plot_x - minX*xScale;
    float dataOrigin_y = plot_y + minY*yScale;
    
    // Draw axes
	indAxis.draw(ofVec2f(plot_x, dataOrigin_y), ofVec2f(plot_x + plot_w, dataOrigin_y), 10);
	depAxis.draw(ofVec2f(dataOrigin_x, plot_y), ofVec2f(dataOrigin_x, plot_y - plot_h), 10);

    // Draw title, axes labels
	ofSetColor(textColor);
	ofDrawBitmapString(title, plot_x + 0.5*(w - 2*padding), y + padding - 5);
	ofDrawBitmapString(xlabel, plot_x + 0.5*(w - 2*padding), y + h - 5);
	
	ofPushMatrix();
	ofRotate(-90, 0, 0, 1);						// Rotate camera for vertical axis
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL);	// This allows the bitmapString to rotate
	ofDrawBitmapString(ylabel, -y - h + 0.5*(h-2*padding), x + padding - 5);
	ofPopMatrix();

	// Draw data
	if(data.size() > 0){

		ofPath dataPath;
		for(size_t i = 0; i < data.size(); i++){
			if(i == 0)
				dataPath.moveTo(ofVec2f(dataOrigin_x + xScale*data[i].indVar, dataOrigin_y - yScale*data[i].depVar));
			else
				dataPath.lineTo(ofVec2f(dataOrigin_x + xScale*data[i].indVar, dataOrigin_y - yScale*data[i].depVar));
		}
		dataPath.setColor(lineColor);
		
		dataPath.setFilled(fillPlot);
        if(!fillPlot){
			dataPath.setStrokeWidth(lineWidth);
        }else{
			dataPath.close();
        }
        
		dataPath.draw();
	}

	ofPopStyle();
	ofEnableDepthTest();
}//====================================================

/**
 *  @brief Add a data point to the plot
 * 
 *  @param ind independent variable value (horizontal axis)
 *  @param dep dependent variable value (vertical axis)
 */
void ofxPlot::addDataPt(double ind, double dep){
	data.push_back(dataPt(ind, dep));
}//====================================================

/**
 *  @brief Add a data point to th plot
 * 
 *  @param pt data point
 *  @see addDataPt(double, double)
 */
void ofxPlot::addDataPt(dataPt pt){
    data.push_back(pt);
}//====================================================

/**
 *  @brief Tell the plot whether or not to fill the plot area
 * 
 *  @param fill whether or not to fill the plot area, i.e., the
 *  area under the plotted curve
 */
void ofxPlot::setFillPlot(bool fill){ fillPlot = fill; }

/**
 *  @brief Set the position of the plot
 * 
 *  @param x pixels, window coordinates
 *  @param y pixels, window coordinates
 */
void ofxPlot::setPosition(float x, float y){
	viewport.setPosition(x, y);
}//====================================================

/**
 *  @brief Set the position of the plot area
 * 
 *  @param pos vector storing (x, y)
 *  @see setPosition(float, float)
 */
void ofxPlot::setPosition(ofVec2f pos){
	setPosition(pos.x, pos.y);
}//====================================================

/**
 *  @brief Set the size of the plot area
 * 
 *  @param w width, pixels
 *  @param h height, pixels
 */
void ofxPlot::setSize(float w, float h){
	viewport.setSize(w, h);
}//====================================================

/**
 *  @brief Set the size of the plot area
 * 
 *  @param size vector storing (width, height)
 *  @see setSize(float, float)
 */
void ofxPlot::setSize(ofVec2f size){
	setSize(size.x, size.y);
}//====================================================

/**
 *  @brief Set the horizontal axis label
 *  @param lbl
 */
void ofxPlot::setXLabel(std::string lbl){ xlabel = lbl; }

/**
 *  @brief Set the vertical axis label
 *  @param lbl
 */
void ofxPlot::setYLabel(std::string lbl){ ylabel = lbl; }

/**
 *  @brief Set the title string
 *  @param str
 */
void ofxPlot::setTitle(std::string str){ title = str; }

