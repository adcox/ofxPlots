#include "ofxPlotTimeline.hpp"


//-----------------------------------------------------------------------------
// -- *structors --
//-----------------------------------------------------------------------------

ofxPlotTimeline::ofxPlotTimeline(){
	init();
}//====================================================

ofxPlotTimeline::ofxPlotTimeline(float t0, float tf){
	firstTime = t0;
	lastTime = tf;
	init();
}//====================================================

void ofxPlotTimeline::init(){
	update();	// Compute scale, size the timeline, etc

	limitLower = ofxPlotTimelineLimiter(false);
	limitLower.setSize(15, 15);	// height is the only important variable
	limitUpper.setSize(15, 15);

	limitLower.setPosition( (limitTimeLower - firstTime)*scale + timelineRect.x, timelineRect.y + timelineRect.height);
	limitUpper.setPosition( (limitTimeUpper - firstTime)*scale + timelineRect.x, timelineRect.y + timelineRect.height);
}//====================================================

//-----------------------------------------------------------------------------
// -- Graphics and Event Loop --
//-----------------------------------------------------------------------------

void ofxPlotTimeline::update(){
	timelineRect.setPosition(viewport.x + 15, viewport.y + 20);
	timelineRect.setSize(viewport.width - 2*15, viewport.height - 2*20);

	scale = std::abs( viewport.width/(lastTime - firstTime) );
	
	// If the user is not dragging the limit marker, update the position in case the size
	// of the timeline widget has changed; else, update the limit time variable
	if(!limitLower.isHovered()){
		limitLower.moveToLimiterCoord( (limitTimeLower - firstTime)*scale + timelineRect.x);
		limitLower.setY(timelineRect.y + timelineRect.height);
	}else{
		limitTimeLower = (limitLower.getLimiterCoord() - timelineRect.x)/scale + firstTime;
	}

	// Ditto for the upper limit marker
	if(!limitUpper.isHovered()){
		limitUpper.moveToLimiterCoord( (limitTimeUpper - firstTime)*scale + timelineRect.x);
		limitUpper.setY(timelineRect.y + timelineRect.height);
	}else{
		limitTimeUpper = (limitUpper.getLimiterCoord() - timelineRect.x)/scale + firstTime;
	}

	// Don't allow the limit markers to move past one another or the ends of the timeline
	limitLower.setXBounds(timelineRect.x, limitUpper.getLimiterCoord());
	limitUpper.setXBounds(limitLower.getLimiterCoord(), timelineRect.x + timelineRect.width);

	limitLower.update();
	limitUpper.update();
}//====================================================

void ofxPlotTimeline::draw(){
	InteractiveObj::draw();

	ofDisableDepthTest();
	ofPushStyle();

	// Get positions of sliders
	float lowerX = limitLower.getLimiterCoord();
	float upperX = limitUpper.getLimiterCoord();

	// Draw inactive and active areas
	ofRectangle inactiveLower(timelineRect.x, timelineRect.y, lowerX - timelineRect.x, timelineRect.height);
	ofRectangle inactiveUpper(upperX, timelineRect.y, timelineRect.x + timelineRect.width - upperX, timelineRect.height);

	ofSetColor(inactiveColor);
	if(inactiveLower.width > 0)
		ofDrawRectangle(inactiveLower);

	if(inactiveUpper.width > 0)
		ofDrawRectangle(inactiveUpper);

	// Draw timeline outline
	ofNoFill();
	ofSetColor(edgeColor);
	ofDrawRectangle(timelineRect);

	// Draw vertical lines at the slider coordinates
	ofSetLineWidth(2);
	ofDrawLine(lowerX, timelineRect.y, lowerX, timelineRect.y + timelineRect.height);
	ofDrawLine(upperX, timelineRect.y, upperX, timelineRect.y + timelineRect.height);

	ofPopStyle();
	ofEnableDepthTest();

	limitLower.draw();
	limitUpper.draw();
}//====================================================

//-----------------------------------------------------------------------------
// -- Set and Get Functions --
//-----------------------------------------------------------------------------

void ofxPlotTimeline::setFirstTime(float t){ firstTime = t; }

void ofxPlotTimeline::setLastTime(float t){ lastTime = t; }

void ofxPlotTimeline::setLink_slotTime0(float* ptr){ }

void ofxPlotTimeline::setLink_slotTimef(float*){ }