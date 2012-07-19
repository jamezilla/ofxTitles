#include "TitlesApp.h"

TitlesApp::TitlesApp(int ac, char* av[]){}


//--------------------------------------------------------------
void TitlesApp::setup(){
   ofBackground(0);

   mVid.loadMovie("fingers.mov");
   mVid.play();

   mTitles.bindVideo(&mVid);
   mTitles.add("Subtitle 1", 1, 1000, 3000);
   

   // subtitles are automatically sorted by their start
   // time so they can be added in any order
   //mTitles.add("Subtitle 6", 6, 9500, 11500);
   //mTitles.add("Subtitle 5", 5, 9000, 8500);  // this won't display
   //mTitles.add("Subtitle 3", 3, 6000, 8000);
   //mTitles.add("Subtitle 2", 7, 3500, 5500);
   //mTitles.add("Subtitle 4", 4, 8500, 8500);  // this won't display

   mTitles.loadFont("Inconsolata.ttf", 32);
   mTitles.setLoopState(OF_LOOP_NORMAL);
   mTitles.play();
}

//--------------------------------------------------------------
void TitlesApp::update(){
	mVid.update();
}

//--------------------------------------------------------------
void TitlesApp::draw(){
	mVid.draw(10, 10);
	// draw inside a bounding box - good for subtitles
	mTitles.draw(10, 10, mVid.width, mVid.height);

	// you can adjust the vertical layout using a percentage (0.0 - 1.0)
	// default is 0.9
	mVid.draw(10,  mVid.height + 20);
	mTitles.draw(10, mVid.height + 20, mVid.width, mVid.height, 0.5);

	// shaky title, absolutely positioned
	int x = (rand() % 5) + 20;
	int y = (rand() % 5)  + (mVid.height * 2) + 80;
	mTitles.setDisplayNumber(true);
	mTitles.draw(x, y);
	mTitles.setDisplayNumber(false);
}

//--------------------------------------------------------------
void TitlesApp::keyPressed(int key){
   
}

//--------------------------------------------------------------
void TitlesApp::keyReleased(int key){

}

//--------------------------------------------------------------
void TitlesApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void TitlesApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void TitlesApp::mousePressed(int x, int y, int button){
   
}

//--------------------------------------------------------------
void TitlesApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void TitlesApp::windowResized(int w, int h){

}

