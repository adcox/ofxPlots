/**
 * @author Andrew Cox
 * @version July 7, 2016
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

#include "interactiveObj.hpp"

#include "DataSelectedEvent.hpp"

//-----------------------------------------------------------------------------
// -- *structors --
//-----------------------------------------------------------------------------

InteractiveObj::InteractiveObj(){
	if(!areEventsSet)
		setEvents(ofEvents());

	ofAddListener(DataSelectedEvent::selected, this, &InteractiveObj::dataSelected);
	ofAddListener(DataSelectedEvent::deselected, this, &InteractiveObj::dataDeselected);

	enableMouseInput();
	enableKeyInput();
}//====================================================

InteractiveObj::~InteractiveObj(){
	ofRemoveListener(DataSelectedEvent::selected, this, &InteractiveObj::dataSelected);
	ofRemoveListener(DataSelectedEvent::deselected, this, &InteractiveObj::dataDeselected);
	
	disableKeyInput();
	disableMouseInput();
}//=====================================================

//-----------------------------------------------------------------------------
// -- Graphics & Event Loop --
//-----------------------------------------------------------------------------

void InteractiveObj::setup(){}

void InteractiveObj::update(){}

void InteractiveObj::draw(){
	ofDisableDepthTest();
	ofPushStyle();
	ofSetColor(bgColor);
	ofDrawRectangle(viewport);

	if(isDrawingOutline){
		ofNoFill();
		ofSetColor(edgeColor);
		ofDrawRectangle(viewport);
	}
	ofPopStyle();
	ofEnableDepthTest();
}//====================================================


//-----------------------------------------------------------------------------
// -- Get and Set --
//-----------------------------------------------------------------------------
/**
 *  @brief Enable mouse input for the plot.
 *  @details This allows the user to interact with the plot area
 */
void InteractiveObj::enableMouseInput(){
	if(!isMouseInputEnabled && events){
		ofAddListener(events->mouseMoved, this, &InteractiveObj::mouseMoved);
		ofAddListener(events->mousePressed, this, &InteractiveObj::mousePressed);
		ofAddListener(events->mouseReleased, this, &InteractiveObj::mouseReleased);
		ofAddListener(events->mouseDragged, this, &InteractiveObj::mouseDragged);
	}
	isMouseInputEnabled = true;
}//====================================================

/**
 *  @brief Disable mouse input for the plot.
 */
void InteractiveObj::disableMouseInput(){
	if(isMouseInputEnabled && events){
		ofRemoveListener(events->mouseMoved, this, &InteractiveObj::mouseMoved);
		ofRemoveListener(events->mousePressed, this, &InteractiveObj::mousePressed);
		ofRemoveListener(events->mouseReleased, this, &InteractiveObj::mouseReleased);
		ofRemoveListener(events->mouseDragged, this, &InteractiveObj::mouseDragged);
	}
	isMouseInputEnabled = false;
}//====================================================

void InteractiveObj::enableKeyInput(){
	if(!isKeyInputEnabled && events){
		ofAddListener(events->keyPressed, this, &InteractiveObj::keyPressed);
		ofAddListener(events->keyReleased, this, &InteractiveObj::keyReleased);
	}
	isKeyInputEnabled = true;
}//====================================================

void InteractiveObj::disableKeyInput(){
	if(isKeyInputEnabled && events){
		ofRemoveListener(events->keyPressed, this, &InteractiveObj::keyPressed);
		ofRemoveListener(events->keyReleased, this, &InteractiveObj::keyReleased);
	}
	isKeyInputEnabled = false;
}//====================================================

/**
 *  @brief Set the background color
 *  @param c background color
 */
void InteractiveObj::setBGColor(ofColor c){ bgColor = c;}

/**
 *  @brief Set the edge/outline color
 *  @param c edge color
 */
void InteractiveObj::setEdgeColor(ofColor c){ edgeColor = c;}

/**
 *  @brief Set the font for the plot area
 * 
 *  @param f font to use for the plot
 */
void InteractiveObj::setFont(ofTrueTypeFont f){ font = f; }

/**
 *  @brief Set the position of the viewport
 * 
 *  @param x pixels, screen coordinates
 *  @param y pixels, screen coordinates
 */
void InteractiveObj::setPosition(float x, float y){
	viewport.setPosition(x, y);
}//====================================================

/**
 *  @brief Set the position of the viewport area
 * 
 *  @param pos vector storing (x, y)
 *  @see setPosition(float, float)
 */
void InteractiveObj::setPosition(ofVec2f pos){
	setPosition(pos.x, pos.y);
}//====================================================

/**
 *  @brief Set the size of the viewport area
 * 
 *  @param w width, pixels
 *  @param h height, pixels
 */
void InteractiveObj::setSize(float w, float h){
	viewport.setSize(w, h);
}//====================================================

/**
 *  @brief Set the size of the viewport area
 * 
 *  @param size vector storing (width, height)
 *  @see setSize(float, float)
 */
void InteractiveObj::setSize(ofVec2f size){
	setSize(size.x, size.y);
}//====================================================


//-----------------------------------------------------------------------------
// -- Event Handlers --
//-----------------------------------------------------------------------------

void InteractiveObj::dataSelected(DataSelectedEventArgs &args){}
void InteractiveObj::dataDeselected(DataSelectedEventArgs &args){}

/**
 *  @brief Handle keyPressed events
 * 
 *  @param evt Describes the keyPressed Event
 */
void InteractiveObj::keyPressed(ofKeyEventArgs &evt){
	heldKey = evt.key;
}//====================================================

/**
 *  @brief Handle keyReleased events
 * 
 *  @param evt Describes the keyReleased Event
 */
void InteractiveObj::keyReleased(ofKeyEventArgs &evt){
	heldKey = 0;
}//====================================================

void InteractiveObj::mousePressed(ofMouseEventArgs &mouse){
	if(viewport.inside(mouse.x, mouse.y)){
		mousePressedPt.x = mouse.x;
		mousePressedPt.y = mouse.y;
		isMousePressedInside = true;
	}
}//====================================================

void InteractiveObj::mouseReleased(ofMouseEventArgs &mouse){
	isMousePressedInside = false;
	isMouseDragged = false;
}//====================================================

void InteractiveObj::mouseDragged(ofMouseEventArgs &mouse){
	isMouseDragged = true;
}//====================================================

void InteractiveObj::mouseMoved(ofMouseEventArgs &mouse){
	mouseX = mouse.x;
	mouseY = mouse.y;
	
	if(viewport.inside(mouse.x, mouse.y)){
		isDrawingOutline = true;
	}else{
		isDrawingOutline = false;
	}
}//====================================================

//-----------------------------------------------------------------------------
// -- Miscellaneous --
//-----------------------------------------------------------------------------

/**
 *  @brief Give the camera the event object
 *  @param evts reference to the window's events object
 */
void InteractiveObj::setEvents(ofCoreEvents & evts){
	// If en/disableMouseInput were called within ofApp::setup(),
	// mouseInputEnabled will tell us about whether the camera
	// mouse input needs to be initialised as enabled or disabled.
	// we will still set `events`, so that subsequent enabling
	// and disabling can work.

	// we need a temporary copy of bMouseInputEnabled, since it will 
	// get changed by disableMouseInput as a side-effect.
	bool wasMouseInputEnabled = isMouseInputEnabled;
	disableMouseInput();
	events = &evts;
	if (wasMouseInputEnabled) {
		// note: this will set bMouseInputEnabled to true as a side-effect.
		enableMouseInput();
	}
	areEventsSet = true;
}//====================================================