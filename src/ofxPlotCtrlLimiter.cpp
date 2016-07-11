#include "ofxPlotCtrlLimiter.hpp"

//-----------------------------------------------------------------------------
// -- *structors --
//-----------------------------------------------------------------------------

ofxPlotCtrlLimiter::ofxPlotCtrlLimiter(){
	init();
}//====================================================

ofxPlotCtrlLimiter::ofxPlotCtrlLimiter(bool isUpper){
	this->isUpper = isUpper;
	init();
}//====================================================

void ofxPlotCtrlLimiter::init(){
	bgColor = ofColor(204, 163, 0, 0.9*255);
}//====================================================

//-----------------------------------------------------------------------------
// -- Graphics and Event Loop --
//-----------------------------------------------------------------------------

void ofxPlotCtrlLimiter::update(){
	// Compute the width of the triangle using trig
	viewport.width = (isUpper ? -1 : 1)*0.5*std::sqrt(3)*viewport.height;

	// Set the X or Y bounds if the limiter's movement is constrained
	if(moveLR)
		setYBounds(viewport.y, viewport.y + viewport.height);
	else
		setXBounds(std::min(viewport.x, viewport.x + viewport.width),
			std::max(viewport.x, viewport.x + viewport.width));
}//====================================================

void ofxPlotCtrlLimiter::draw(){
	// Do not call the base-class draw function; completely override it
	ofPath outline;
	outline.moveTo(viewport.x, viewport.y);
	outline.lineTo(viewport.x, viewport.y + viewport.height);
	outline.lineTo(viewport.x + viewport.width, viewport.y + 0.5*viewport.height);
	outline.close();

	ofDisableDepthTest();
	ofPushStyle();
	outline.setColor(isMouseInside ? hoverColor : bgColor);
	outline.draw();

	outline.setFilled(false);
	outline.setColor(edgeColor);
	outline.draw();
	ofPopStyle();
	ofEnableDepthTest();
}//====================================================

//-----------------------------------------------------------------------------
// -- Set and Get Functions --
//-----------------------------------------------------------------------------

/**
 *  @brief Retrieve the coordinate that locates the limiter
 *  @details This coordinate represents the x-coordinate of
 *  the limiter when moveLR is TRUE, or the y-coordinate of
 *  the limiter when moveLR is FALSE (i.e., move up/down).
 *  Use this coordinate rather than the viewport position.
 *  
 *  @return the coordinate that locates the limiter in screen coordinates
 */
float ofxPlotCtrlLimiter::getLimiterCoord() const{
	return moveLR ? viewport.x + viewport.width : viewport.y + viewport.height;
}//====================================================

/**
 *  @brief Position the limiter at the desired coordinate value
 *  @details The coordinate represents the x-coordinate of
 *  the limiter when moveLR is TRUE, or the y-coordinate of
 *  the limiter when moveLR is FALSE (i.e., move up/down).
 * 
 *  @param coord desired coordinate value, screen pixel coordinates
 */
void ofxPlotCtrlLimiter::moveToLimiterCoord(float coord){
	if(moveLR)
		viewport.x = coord - viewport.width;
	else
		viewport.y = coord - viewport.height;
}//====================================================

/**
 *  @brief Set horizontal bounds of motion
 * 
 *  @param min lowest (left-most) x-value the limiter can move to
 *  @param max highest (right-most) x-value the limiter can move to
 */
void ofxPlotCtrlLimiter::setXBounds(float min, float max){
	bounds[0] = min;
	bounds[1] = max;
}//====================================================

/**
 *  @brief Set vertical bounds of motion
 * 
 *  @param min lowest (top-most) y-value the limiter can move to
 *  @param max highest (bottom-most) y-value the limiter can move to
 */
void ofxPlotCtrlLimiter::setYBounds(float min, float max){
	bounds[2] = min;
	bounds[3] = max;
}//====================================================


//-----------------------------------------------------------------------------
// -- Event Handlers --
//-----------------------------------------------------------------------------

/**
 *  @brief Handle mouse dragged events
 *  @details Clicking and dragging the limiter moves it, so long
 *  as the mouse remains within the relevant bounds
 * 
 *  @param mouse contains information relevant to the mouse event
 */
void ofxPlotCtrlLimiter::mouseDragged(ofMouseEventArgs &mouse){
	if(isMousePressedInside){
		
		if(moveLR && mouse.x >= bounds[0] && mouse.x <= bounds[1])
			moveToLimiterCoord(mouse.x);
		else if(!moveLR && mouse.y >= bounds[2] && mouse.y <= bounds[3])
			moveToLimiterCoord(mouse.y);
	}
}//====================================================