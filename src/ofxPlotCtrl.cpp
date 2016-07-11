#include "ofxPlotCtrl.hpp"
#include "dataSelectedEvent.hpp"

//-----------------------------------------------------------------------------
// -- *structors --
//-----------------------------------------------------------------------------

ofxPlotCtrl::ofxPlotCtrl(){
	init();
}//====================================================

void ofxPlotCtrl::init(){
	update();	// Compute scale, size the timeline, etc

	limitLower = ofxPlotCtrlLimiter(false);
	limitLower.setSize(15, 15);	// height is the only important variable
	limitUpper.setSize(15, 15);

	if(indVars.size() > 1 && link_limitValLower && link_limitValUpper){
		limitLower.setPosition( (*link_limitValLower - indVars.front())*scale + timelineRect.x, timelineRect.y + timelineRect.height);
		limitUpper.setPosition( (*link_limitValUpper - indVars.front())*scale + timelineRect.x, timelineRect.y + timelineRect.height);
	}
}//====================================================

//-----------------------------------------------------------------------------
// -- Graphics and Event Loop --
//-----------------------------------------------------------------------------

void ofxPlotCtrl::update(){
	timelineRect.setPosition(viewport.x + 15, viewport.y + 20);
	timelineRect.setSize(viewport.width - 2*15, viewport.height - 2*20);

	if(indVars.size() > 1 && link_limitValLower && link_limitValUpper){
		scale = std::abs( timelineRect.width/(indVars.back() - indVars.front()) );
		
		// If the user is not dragging the limit marker, update the position in case the size
		// of the timeline widget has changed; else, update the limit time variable
		if(!limitLower.isHovered()){
			limitLower.moveToLimiterCoord( (*link_limitValLower - indVars.front())*scale + timelineRect.x);
			limitLower.setY(timelineRect.y + timelineRect.height);
		}else{
			*link_limitValLower = (limitLower.getLimiterCoord() - timelineRect.x)/scale + indVars.front();
		}

		// Ditto for the upper limit marker
		if(!limitUpper.isHovered()){
			limitUpper.moveToLimiterCoord( (*link_limitValUpper - indVars.front())*scale + timelineRect.x);
			limitUpper.setY(timelineRect.y + timelineRect.height);
		}else{
			*link_limitValUpper = (limitUpper.getLimiterCoord() - timelineRect.x)/scale + indVars.front();
		}
	}

	// Don't allow the limit markers to move past one another or the ends of the timeline
	limitLower.setXBounds(timelineRect.x, limitUpper.getLimiterCoord());
	limitUpper.setXBounds(limitLower.getLimiterCoord(), timelineRect.x + timelineRect.width);

	limitLower.update();
	limitUpper.update();
}//====================================================

void ofxPlotCtrl::draw(){
	InteractiveObj::draw();

	ofDisableDepthTest();
	ofPushStyle();

	// Draw highlighted points, if any
	if(indVars.size() > 1){
		ofSetColor(ofColor::yellow);
		for(size_t i = 0; i < highlightPtIxs.size(); i++){
			float x = (indVars[highlightPtIxs[i]] - indVars[0])*scale + timelineRect.x;
			ofDrawCircle(ofVec2f(x, timelineRect.y + 0.5*timelineRect.height), 2);
		}
	}

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

void ofxPlotCtrl::addDataPt(double val){ indVars.push_back(val); }

void ofxPlotCtrl::setData(std::vector<double> vals){ indVars = vals; }

void ofxPlotCtrl::setHighlightedPts(std::vector<int> pts){ highlightPtIxs = pts; }

void ofxPlotCtrl::setLink_lowerLimit(float *ptr){ link_limitValLower = ptr; }

void ofxPlotCtrl::setLink_upperLimit(float *ptr){ link_limitValUpper = ptr; }

// /**
//  *  @brief Set the value of the lower limit slider (same units as independent data variables)
//  *  @param val value of the lower limit slider (same units as independent data variables)
//  */
// void ofxPlotCtrl::setLowerLimitVal(float val){
// 	limitValLower = std::max(val, (float)indVars.front());
// }

// /**
//  *  @brief Set the value of the upper limit slider (same units as independent data variables)
//  *  @param val value of the upper limit slider (same units as independent data variables)
//  */
// void ofxPlotCtrl::setUpperLimitVal(float val){
// 	limitValUpper = std::min(val, (float)indVars.back());
// }

//-----------------------------------------------------------------------------
// -- Event Handlers --
//-----------------------------------------------------------------------------

void ofxPlotCtrl::dataSelected(DataSelectedEventArgs &args){
	setHighlightedPts(args.indices);
}//====================================================

void ofxPlotCtrl::dataDeselected(DataSelectedEventArgs &args){
	std::vector<int> empty;
	setHighlightedPts(empty);
}//====================================================