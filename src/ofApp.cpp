/**
 *  @brief A sample application that creates a plot
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

#include "ofApp.h"

void ofApp::setup(){

	ofEnableSmoothing();

	plot.setSize(600, 200);
	plot.setPosition(270, 270);
	plot.setXLabel("Time");
	plot.setYLabel("Amplitude");
	plot.setTitle("Amplitude Time History");
	plot.enableMouseInput();
	
	for(double t = -15; t < 50; t+= 0.05){
		plot.addDataPt(t, 1*sin(t*PI) + 3*sin(t*PI/5));
	}
}//====================================================

void ofApp::update(){

}//====================================================

void ofApp::draw(){
	plot.draw();
}//====================================================

void ofApp::keyPressed(int key){

}//====================================================

void ofApp::keyReleased(int key){

}//====================================================

void ofApp::mouseMoved(int x, int y ){

}//====================================================

void ofApp::mouseDragged(int x, int y, int button){

}//====================================================

void ofApp::mousePressed(int x, int y, int button){

}//====================================================

void ofApp::mouseReleased(int x, int y, int button){

}//====================================================

void ofApp::mouseEntered(int x, int y){

}//====================================================

void ofApp::mouseExited(int x, int y){

}//====================================================

void ofApp::windowResized(int w, int h){

}//====================================================

void ofApp::gotMessage(ofMessage msg){

}//====================================================

void ofApp::dragEvent(ofDragInfo dragInfo){ 

}//====================================================
