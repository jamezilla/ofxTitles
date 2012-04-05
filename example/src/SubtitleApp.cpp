#include "SubtitleApp.h"

SubtitleApp::SubtitleApp(int ac, char* av[]){}


//--------------------------------------------------------------
void SubtitleApp::setup(){
   ofBackground(0);
	mSubtitle.add("Subtitle 1", 1, 2000, 7000);
	mSubtitle.add("Subtitle 2", 2, 9000, 14000);
	mSubtitle.add("Subtitle 3", 3, 14500, 17000);
	mSubtitle.add("Subtitle 4", 4, 20000, 20000);
	mSubtitle.add("Subtitle 5", 5, 21000, 20000);
	mSubtitle.play();
}

//--------------------------------------------------------------
void SubtitleApp::update(){

}

//--------------------------------------------------------------
void SubtitleApp::draw(){
	mSubtitle.draw(10, 120);
	ofDrawBitmapString("1, 2000, 7000", 10, 20);
	ofDrawBitmapString("2, 9000, 14000", 10, 40);
	ofDrawBitmapString("3, 14500, 17000", 10, 60);
	ofDrawBitmapString("4, 20000, 20000", 10, 80);
	ofDrawBitmapString("5, 21000, 20000", 10, 100);
}

//--------------------------------------------------------------
void SubtitleApp::keyPressed(int key){
   
}

//--------------------------------------------------------------
void SubtitleApp::keyReleased(int key){

}

//--------------------------------------------------------------
void SubtitleApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void SubtitleApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void SubtitleApp::mousePressed(int x, int y, int button){
   
}

//--------------------------------------------------------------
void SubtitleApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void SubtitleApp::windowResized(int w, int h){

}

