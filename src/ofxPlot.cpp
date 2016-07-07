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

#include "dataSelectedEvent.hpp"

#include <algorithm>

/**
 *  @brief Construct a default ofxPlot object
 */
ofxPlot::ofxPlot(){
	indAxis.setColor(axesColor);
	depAxis.setColor(axesColor);

	indAxis.setLineWidth(axesWidth);
	depAxis.setLineWidth(axesWidth);

	ofAddListener(DataSelectedEvent::selected, this, &ofxPlot::dataSelected);
	ofAddListener(DataSelectedEvent::deselected, this, &ofxPlot::dataDeselected);

	if(!eventsSet){
		setEvents(ofEvents());
	}

	enableMouseInput();
	enableKeyInput();
}//====================================================

/**
 *  @brief Destructor; free allocated resources and listeners
 */
ofxPlot::~ofxPlot(){
	ofRemoveListener(DataSelectedEvent::selected, this, &ofxPlot::dataSelected);
	ofRemoveListener(DataSelectedEvent::deselected, this, &ofxPlot::dataDeselected);
	
	disableKeyInput();
	disableMouseInput();
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

	if(isDrawingOutline){
		ofNoFill();
		ofSetColor(axesColor);
		ofDrawRectangle(viewport);
	}
	
	// Plot window properties
	float win_x = viewport.getX();			// top-left corner, increases to the right
	float win_y = viewport.getY();			// top-left corner, increases to the bottom
	float win_w = viewport.getWidth();
	float win_h = viewport.getHeight();

    // Characteristics of the area that information is plotted in
    float plot_w = win_w - 2*padding;       // Width of the area within the axes
    float plot_h = win_h - 2*padding;       // Height of the area within the axes
    float plot_x = win_x + padding;         // Bottom-left corner of area within the axes
    float plot_y = win_y + win_h - padding;     // Bottom-left corner of the area within the axes
    
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
	
	// Zero MUST be part of the data set
	// ToDo (perhaps): allow axes to represent values other than x = 0 or y = 0
    maxX = maxX > 0 ? maxX : 0;
    minX = minX < 0 ? minX : 0;
    maxY = maxY > 0 ? maxY : 0;
    minY = minY < 0 ? minY : 0;
    
    // Determine scaling amounts so that data will fit in plot area
    double xScale = plot_w/(maxX - minX);
    double yScale = plot_h/(maxY - minY);
    
    // Place axes to use as much of the plot area as possible
    float dataOrigin_x = plot_x - minX*xScale;
    float dataOrigin_y = plot_y + minY*yScale;
    
    // Draw axes
	indAxis.draw(ofVec2f(plot_x, dataOrigin_y), ofVec2f(plot_x + plot_w, dataOrigin_y), 10);
	depAxis.draw(ofVec2f(dataOrigin_x, plot_y), ofVec2f(dataOrigin_x, plot_y - plot_h), 10);

    // Draw title, axes labels
	ofSetColor(textColor);
	if(font.isLoaded()){
		ofRectangle titleBox = font.getStringBoundingBox(title, 0,0);
		font.drawString(title, plot_x + 0.5*(plot_w - titleBox.width), plot_y - plot_h - 5);
		ofRectangle xLblBox = font.getStringBoundingBox(xlabel, 0,0);
		font.drawString(xlabel, plot_x + 0.5*(plot_w - xLblBox.width), plot_y + padding - 5);
	}else{
		ofDrawBitmapString(title, plot_x + 0.5*plot_w, win_y + padding - 5);
		ofDrawBitmapString(xlabel, plot_x + 0.5*plot_w, win_y + win_h - 5);
	}

	ofPushMatrix();
	ofRotate(-90, 0, 0, 1);						// Rotate camera for vertical axis
	if(font.isLoaded()){
		ofRectangle yLblBox = font.getStringBoundingBox(ylabel, 0,0);
		font.drawString(ylabel, -plot_y - padding + 0.5*(plot_h - 2*yLblBox.height), plot_x - 5);
	}else{
		ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL);	// This allows the bitmapString to rotate
		ofDrawBitmapString(ylabel, -plot_y - padding + 0.5*(plot_h - 2*padding), plot_x - 5);
	}
	ofPopMatrix();

	// Draw data
	displayData.clear();
	displayData.assign(data.size(), ofVec2f(0,0));

	if(data.size() > 0){

		ofPath dataPath;
		for(size_t i = 0; i < data.size(); i++){
			displayData[i] = ofVec2f(dataOrigin_x + xScale*data[i].indVar, dataOrigin_y - yScale*data[i].depVar);
			if(i == 0)
				dataPath.moveTo(displayData[i]);
			else
				dataPath.lineTo(displayData[i]);
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

	if(!highlightPtIxs.empty()){
		ofNoFill();

		size_t ix;
		for(size_t i = 0; i < highlightPtIxs.size(); i++){
			ix = highlightPtIxs[i];
			ofSetColor(ofColor::yellow);
			ofSetLineWidth(2);
			ofDrawCircle(displayData[ix], 4);
			ofSetLineWidth(1);
		}

        // Draw big cross hairs on the last selected point
		ofSetColor(axesColor);
		ofSetLineWidth(1);
        ofDrawLine(ofVec2f(plot_x, displayData[ix].y), ofVec2f(plot_x + plot_w, displayData[ix].y));
        ofDrawLine(ofVec2f(displayData[ix].x, plot_y), ofVec2f(displayData[ix].x, plot_y - plot_h));

        // Print out data value
        char dataStr[128];
        sprintf(dataStr, "(%.4f, %.4f)", data[ix].indVar, data[ix].depVar);
        if(font.isLoaded()){
        	ofRectangle dataBox = font.getStringBoundingBox(dataStr, 0,0);
        	font.drawString(dataStr, plot_x + plot_w - padding - dataBox.width, plot_y + padding - 5);
        }else{
        	ofDrawBitmapString(dataStr, plot_x + plot_w - 125 - padding, plot_y + padding - 5);
        }
	}

	if(!selectedArea.isZero()){
		ofSetColor(axesColor);
		ofNoFill();
		ofSetLineWidth(1);
		ofDrawRectangle(selectedArea);
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
 *  @brief Enable mouse input for the plot.
 *  @details This allows the user to interact with the plot area
 */
void ofxPlot::enableMouseInput(){
	if(!isMouseInputEnabled && events){
		ofAddListener(events->mouseMoved, this, &ofxPlot::mouseMoved);
		ofAddListener(events->mousePressed, this, &ofxPlot::mousePressed);
		ofAddListener(events->mouseReleased, this, &ofxPlot::mouseReleased);
		ofAddListener(events->mouseDragged, this, &ofxPlot::mouseDragged);
	}
	isMouseInputEnabled = true;
}//====================================================

/**
 *  @brief Disable mouse input for the plot.
 */
void ofxPlot::disableMouseInput(){
	if(isMouseInputEnabled && events){
		ofRemoveListener(events->mouseMoved, this, &ofxPlot::mouseMoved);
		ofRemoveListener(events->mousePressed, this, &ofxPlot::mousePressed);
		ofRemoveListener(events->mouseReleased, this, &ofxPlot::mouseReleased);
		ofRemoveListener(events->mouseDragged, this, &ofxPlot::mouseDragged);
	}
	isMouseInputEnabled = false;
}//====================================================

void ofxPlot::enableKeyInput(){
	if(!isKeyInputEnabled && events){
		ofAddListener(events->keyPressed, this, &ofxPlot::keyPressed);
		ofAddListener(events->keyReleased, this, &ofxPlot::keyReleased);
	}
	isKeyInputEnabled = true;
}//====================================================

void ofxPlot::disableKeyInput(){
	if(isKeyInputEnabled && events){
		ofRemoveListener(events->keyPressed, this, &ofxPlot::keyPressed);
		ofRemoveListener(events->keyReleased, this, &ofxPlot::keyReleased);
	}
	isKeyInputEnabled = false;
}//====================================================

/**
 *  @brief Give the camera the event object
 *  @param _events reference to the window's events object
 */
void ofxPlot::setEvents(ofCoreEvents & _events){
	// If en/disableMouseInput were called within ofApp::setup(),
	// mouseInputEnabled will tell us about whether the camera
	// mouse input needs to be initialised as enabled or disabled.
	// we will still set `events`, so that subsequent enabling
	// and disabling can work.

	// we need a temporary copy of bMouseInputEnabled, since it will 
	// get changed by disableMouseInput as a side-effect.
	bool wasMouseInputEnabled = isMouseInputEnabled;
	disableMouseInput();
	events = &_events;
	if (wasMouseInputEnabled) {
		// note: this will set bMouseInputEnabled to true as a side-effect.
		enableMouseInput();
	}
	eventsSet = true;
}//====================================================

/**
 *  @brief Tell the plot whether or not to fill the plot area
 * 
 *  @param fill whether or not to fill the plot area, i.e., the
 *  area under the plotted curve
 */
void ofxPlot::setFillPlot(bool fill){ fillPlot = fill; }

void ofxPlot::setHighlightedPts(std::vector<int> ixs){
	highlightPtIxs = ixs;
}

/**
 *  @brief Set the font for the plot area
 * 
 *  @param f font to use for the plot
 */
void ofxPlot::setFont(ofTrueTypeFont f){ font = f; }

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

/**
 *  @brief Set the axes color
 *  @param c axes color
 */
void ofxPlot::setAxesColor(ofColor c){ axesColor = c;}

/**
 *  @brief Set the background color
 *  @param c background color
 */
void ofxPlot::setBGColor(ofColor c){ bgColor = c;}

/**
 *  @brief Set the plot fill color, i.e., the color of the 
 *  fill under the curve
 *  @param c fill color
 */
void ofxPlot::setFillColor(ofColor c){ fillColor = c;}

/**
 *  @brief Set the line color
 *  @param c line color
 */
void ofxPlot::setLineColor(ofColor c){ lineColor = c;}

/**
 *  @brief Set the text color
 *  @param c text color
 */
void ofxPlot::setTextColor(ofColor c){ textColor = c;}

/**
 *  @brief Handle keyPressed events
 * 
 *  @param evt Describes the keyPressed Event
 */
void ofxPlot::keyPressed(ofKeyEventArgs &evt){
	heldKey = evt.key;
}//====================================================

/**
 *  @brief Handle keyReleased events
 * 
 *  @param evt Describes the keyReleased Event
 */
void ofxPlot::keyReleased(ofKeyEventArgs &evt){
	heldKey = 0;
}//====================================================

/**
 *  @brief Handle mouse movement events
 *  @details This function is called every time the mouse is moved
 *  (assuming mouse input has been enabled)
 * 
 *  @param mouse mouse event arguments
 */
void ofxPlot::mouseMoved(ofMouseEventArgs &mouse){
	if(viewport.inside(mouse.x, mouse.y)){
		isDrawingOutline = true;

		// Only highlight points if the mouse is inside the view AND the mouse isn't being dragged
		if(!isMouseDragged && heldKey == 'i'){
			// Find nearest point
			float minDist;
			int minIx;
			for(size_t i = 0; i < displayData.size(); i++){
				ofVec2f dist = displayData[i] - mouse;

				if(i == 0 || dist.length() < minDist){
					minDist = dist.length();
					minIx = i;
				}
			}


			DataSelectedEventArgs args(minIx);
			if(minDist < maxSelectDist){
				ofNotifyEvent(DataSelectedEvent::selected, args);
			}else{
				ofNotifyEvent(DataSelectedEvent::deselected, args);
			}
		}
	}else{
		isDrawingOutline = false;
	}
}//====================================================

void ofxPlot::mousePressed(ofMouseEventArgs &mouse){
	if(viewport.inside(mouse.x, mouse.y)){
		mousePressedPt.x = mouse.x;
		mousePressedPt.y = mouse.y;
		isMousePressedInside = true;
	}
}//====================================================

void ofxPlot::mouseReleased(ofMouseEventArgs &mouse){
	selectedArea = ofRectangle(0,0,0,0);
	isMousePressedInside = false;
	isMouseDragged = false;
}//====================================================

void ofxPlot::mouseDragged(ofMouseEventArgs &mouse){
	if(isMousePressedInside){

		// Limit mouse coordinates to the bounds of the viewport rectangle
		float mX = mouse.x > viewport.x ? mouse.x : viewport.x;
		float mY = mouse.y > viewport.y ? mouse.y : viewport.y;
		
		if(mX > viewport.x + viewport.width)
			mX = viewport.x + viewport.width;

		if(mY > viewport.y + viewport.height)
			mY = viewport.y + viewport.height;

		// update rectangle
		selectedArea.setPosition(std::min(mX, mousePressedPt.x), std::min(mY, mousePressedPt.y));	// top-left
		selectedArea.setSize(std::max(mX, mousePressedPt.x) - selectedArea.x,
			std::max(mY, mousePressedPt.y) - selectedArea.y);

		// Update selection and send event
		DataSelectedEventArgs args;
		for(size_t i = 0; i < displayData.size(); i++){
			if(selectedArea.inside(displayData[i].x, displayData[i].y)){
				args.indices.push_back(i);
			}
		}
		if(!args.indices.empty())
			ofNotifyEvent(DataSelectedEvent::selected, args);

	}
	isMouseDragged = true;
}//=====================================================

void ofxPlot::dataSelected(DataSelectedEventArgs &args){
	setHighlightedPts(args.indices);
}//====================================================

void ofxPlot::dataDeselected(DataSelectedEventArgs &args){
	std::vector<int> empty;
	setHighlightedPts(empty);
}//====================================================



