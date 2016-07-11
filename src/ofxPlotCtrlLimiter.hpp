#pragma once

#include "interactiveObj.hpp"

class ofxPlotCtrlLimiter : public InteractiveObj{
	public:
		ofxPlotCtrlLimiter();
		ofxPlotCtrlLimiter(bool);

		void update();
		void draw();

		float getLimiterCoord() const;
		void moveToLimiterCoord(float);

		void setXBounds(float, float);
		void setYBounds(float, float);

		void mouseDragged(ofMouseEventArgs&);
	protected:

		bool isUpper = true;		//!< Whether or not this limitter represents the upper bound
		bool moveLR = true;			//!< Whether or not the limitter moves Left/Right or Up/Down

		/**
		 * Bounds on the motion of the limitter. Elements [0] and [1]
		 * represent the left/right limits in screen coordinates;
		 * Elements [2] and [3] similarly represent the up/down limits
		 * in screen coordinates
		 */
		float bounds[4] = {0,0,10,10};

		ofColor hoverColor = ofColor(255, 83, 26, 0.9*255);		//!< Hover color

		void init();

};