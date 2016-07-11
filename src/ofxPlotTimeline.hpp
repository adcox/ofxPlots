#pragma once

#include "interactiveObj.hpp"
#include "ofxPlotTimelineLimiter.hpp"

class ofxPlotTimeline : public InteractiveObj{
public:
	ofxPlotTimeline();
	ofxPlotTimeline(float, float);

	void update();
	void draw();

	void setFirstTime(float);
	void setLastTime(float);

	void setLink_slotTime0(float*);
	void setLink_slotTimef(float*);

protected:
	float firstTime = 0;
	float lastTime = 1;
	float limitTimeLower = 0.25;
	float limitTimeUpper = 0.75;

	float *link_limitTimeLower = nullptr;
	float *link_limitTimeUpper = nullptr;

	float scale = 1.0;

	ofRectangle timelineRect = ofRectangle(0,0,10,10);
	ofColor activeColor = ofColor(225, 225, 225, 0.8*255);
	ofColor inactiveColor = ofColor(125, 125, 125, 0.8*255);

	ofxPlotTimelineLimiter limitLower, limitUpper;

	void init();
};