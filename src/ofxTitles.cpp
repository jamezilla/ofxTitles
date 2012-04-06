/*
 *  ofxTitles.h
 *
 *  Created by James Hughes on 2012/04/04.
 *  Copyright 2012 James Hughes. All rights reserved.
 *
 */
#include "ofxTitles.h"
#include <iomanip>
#include <strstream>
#include <iostream>
#include <fstream>
#include <vector>

#include "ofxTitles.h"
#include <assert.h>
#include "ofMain.h"
#include "ofEvents.h"

using Poco::FastMutex;

ofxTitles::ofxTitles() 
	: loop_type(OF_LOOP_NONE),
	  base_timestamp(ofGetElapsedTimeMillis()),
	  frame_timestamp(ofGetElapsedTimeMillis()),
	  playback_state(TITLE_STOPPED)
{
	ofAddListener(ofEvents.update, this, &ofxTitles::update);
}

void ofxTitles::add(std::string _text, int _number, int _start_time, int _end_time)
{
	if (_start_time > _end_time) {
		ofLog(OF_LOG_ERROR, "Start time should come before end time for title number: " + ofToString(_number));
	}

	// construct the frame
	TitlePtr sub = TitlePtr(new Title);
	sub->text       = _text;
	sub->number     = _number;
	sub->start_time = _start_time;
	sub->end_time   = _end_time;
	sub->duration   = _end_time - _start_time;

	// stick it in the buffer
	titles.insert(sub);
}

void ofxTitles::clear(void)
{
	titles.clear();
}

void ofxTitles::draw(void)
{
	draw(0, 0);
}

void ofxTitles::draw(float x, float y)
{
	// can't draw if there's nuthin' ta draw
	if (playback_state != TITLE_DISPLAYING)
		return;

	ofPushStyle();
	ofSetHexColor(0x00ff00);
	if (font.isLoaded())
		font.drawString((*play_head)->text, x, y);
	else
		ofDrawBitmapString((*play_head)->text, x, y);
	ofPopStyle();
}

void ofxTitles::draw(float x, float y, float w, float h, float percent)
{
	// can't draw if there's nuthin' ta draw
	if (playback_state != TITLE_DISPLAYING)
		return;

	ofPushStyle();
	ofSetHexColor(0x00ff00);
	if (font.isLoaded()) {
		x = x + ((w - font.stringWidth((*play_head)->text)) * 0.5);
		y = y + (h * percent) + (font.stringHeight((*play_head)->text) * 0.5);
		font.drawString((*play_head)->text, x, y);
	} else {
		ofDrawBitmapString((*play_head)->text, x, y);
	}
	ofPopStyle();
}

bool ofxTitles::empty(void)
{
	return titles.empty();
}

void ofxTitles::firstFrame(void)
{
	// we can't rewind if the play_buffer is empty
	if (titles.empty()) {
		ofLog(OF_LOG_WARNING, "Title::firstFrame() called on an empty title container");
		return;
	}

	// start back at the beggining
	play_head = titles.begin();

	// reset our base timestamp
	base_timestamp = ofGetElapsedTimeMillis();
}

bool ofxTitles::isPlaying(void)
{
	switch (playback_state) {
	case TITLE_STOPPED:
	case TITLE_PAUSED:
		return false;
	case TITLE_DISPLAYING:
	case TITLE_WAITING:
		return true;
	default:
		// batshit crazy stuff going on
		ofLog(OF_LOG_ERROR, "Title::isPlaying() called with some unknown playback state");
		return false;
	}
}

void ofxTitles::loadFont(std::string path, int fontsize, bool _bAntiAliased, bool _bFullCharacterSet, bool makeContours, float simplifyAmt, int dpi)
{
	font.loadFont(path, fontsize, _bAntiAliased, _bFullCharacterSet, makeContours, simplifyAmt, dpi);
}

bool ofxTitles::_advancePlayHead(void)
{
	if (titles.empty()) {
		ofLog(OF_LOG_WARNING, "Title::_advancePlayHead() called on an empty title container");
		return false;
	}
		
	// advance the play head to the next frame
	++play_head;

	// see if we're at the end of the play_buffer
	if (play_head == titles.end()) {
		switch(loop_type) {
		case OF_LOOP_NORMAL:
			// start back at the beggining
			firstFrame();
			return true;
		case OF_LOOP_PALINDROME:
			assert(false); // not implemented
			break;
		default:
			playback_state = TITLE_STOPPED;
			return false;
		}
	}

	return true;
}

void ofxTitles::_updateDisplayState(void)
{
	int time_left = (base_timestamp + (*play_head)->end_time) - ofGetElapsedTimeMillis();
	if (time_left <= 0) {
		// advance the head_frame iterator
		if (_advancePlayHead())
			_checkPlayState();
	}
}

void ofxTitles::_checkPlayState(void)
{
	assert(playback_state != TITLE_STOPPED);
	assert(play_head != titles.end());

	// set the playback state based on the start time of the next title
	int start = base_timestamp + (*play_head)->start_time;

	// is start in the future?
	if (start <= ofGetElapsedTimeMillis()) {
		playback_state = TITLE_DISPLAYING;
	} else {
		playback_state = TITLE_WAITING;
	}
}

void ofxTitles::play(void)
{
	// TODO: find out whether this causes an off-by-one problem. 
	//       Need to figure out whether the "update" event listener 
	//       fires before update() in the sketch, or after
	if (playback_state == TITLE_STOPPED) {
		firstFrame();
		playback_state = TITLE_WAITING;
	}
	
	_checkPlayState();
}

void ofxTitles::setLoopState(ofLoopType state)
{
	loop_type = state;
}

int ofxTitles::size(void)
{
	return titles.size();
}


void ofxTitles::update(ofEventArgs& args)
{
	// figure out how much time elapsed since the last update
	int last_frame_time = ofGetElapsedTimeMillis() - frame_timestamp;

	// update the frame timestamp
	frame_timestamp = ofGetElapsedTimeMillis();

	switch(playback_state) {
	case TITLE_STOPPED:
		// stick a fork in it, we're done
		return;
	case TITLE_PAUSED:
		// keep the base_timestamp rolling forward so the relative
		// offsets of our titles stay in the right place
		base_timestamp += last_frame_time;
		break;
	case TITLE_DISPLAYING:
		_updateDisplayState();
		break;
	case TITLE_WAITING:
		_checkPlayState();
		break;
	default:
		// batshit crazy stuff going on
		ofLog(OF_LOG_ERROR, "Title::update() called with some unknown playback state");
		break;
	}
}

void ofxTitles::stop(void)
{
	playback_state = TITLE_PAUSED;
}
