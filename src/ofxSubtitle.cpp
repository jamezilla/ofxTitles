/*
 *  ofxSubtitle.h
 *
 *  Created by James Hughes on 2012/04/04.
 *  Copyright 2012 James Hughes. All rights reserved.
 *
 */
#include "ofxSubtitle.h"
#include <iomanip>
#include <strstream>
#include <iostream>
#include <fstream>
#include <vector>

#include "ofxSubtitle.h"
#include <assert.h>
#include "ofMain.h"
#include "ofEvents.h"

using Poco::FastMutex;

ofxSubtitle::ofxSubtitle() 
	: loop_type(OF_LOOP_NONE),
	  base_timestamp(ofGetElapsedTimeMillis()),
	  frame_timestamp(ofGetElapsedTimeMillis()),
	  playback_state(SUBTITLE_STOPPED)
{
	ofAddListener(ofEvents.update, this, &ofxSubtitle::update);
}

void ofxSubtitle::add(std::string _text, int _number, int _start_time, int _end_time)
{
	if (_start_time > _end_time) {
		ofLog(OF_LOG_ERROR, "Start time should come before end time for subtitle number: " + ofToString(_number));
	}

	// construct the frame
	SubtitlePtr sub = SubtitlePtr(new Subtitle);
	sub->text       = _text;
	sub->number     = _number;
	sub->start_time = _start_time;
	sub->end_time   = _end_time;
	sub->duration   = _end_time - _start_time;

	// stick it in the buffer
	subtitles.insert(sub);
}

void ofxSubtitle::clear(void)
{
	subtitles.clear();
}

void ofxSubtitle::draw(void)
{
	draw(0, 0);
}

void ofxSubtitle::draw(float x, float y)
{
	// can't draw if there's nuthin' ta draw
	if (playback_state != SUBTITLE_DISPLAYING)
		return;

	//font.drawString(play_head->get()->text, x, y);
	ofPushStyle();
	ofSetHexColor(0x00ff00);
	ofDrawBitmapString((*play_head)->text, x, y);
	ofPopStyle();
}

bool ofxSubtitle::empty(void)
{
	return subtitles.empty();
}

void ofxSubtitle::firstFrame(void)
{
	// we can't rewind if the play_buffer is empty
	if (subtitles.empty()) {
		ofLog(OF_LOG_WARNING, "Subtitle::firstFrame() called on an empty subtitle container");
		return;
	}

	// start back at the beggining
	play_head = subtitles.begin();

	// reset our base timestamp
	base_timestamp = ofGetElapsedTimeMillis();
}

bool ofxSubtitle::isPlaying(void)
{
	switch (playback_state) {
	case SUBTITLE_STOPPED:
	case SUBTITLE_PAUSED:
		return false;
	case SUBTITLE_DISPLAYING:
	case SUBTITLE_WAITING:
		return true;
	default:
		// batshit crazy stuff going on
		ofLog(OF_LOG_ERROR, "Subtitle::isPlaying() called with some unknown playback state");
		return false;
	}
}

bool ofxSubtitle::_advancePlayHead(void)
{
	if (subtitles.empty()) {
		ofLog(OF_LOG_WARNING, "Subtitle::_advancePlayHead() called on an empty subtitle container");
		return false;
	}
		
	// advance the play head to the next frame
	++play_head;

	// see if we're at the end of the play_buffer
	if (play_head == subtitles.end()) {
		switch(loop_type) {
		case OF_LOOP_NORMAL:
			// start back at the beggining
			firstFrame();
			return true;
		case OF_LOOP_PALINDROME:
			assert(false); // not implemented
			break;
		default:
			playback_state = SUBTITLE_STOPPED;
			return false;
		}
	}

	return true;
}

void ofxSubtitle::_updateDisplayState(void)
{
	int time_left = (base_timestamp + (*play_head)->end_time) - ofGetElapsedTimeMillis();
	if (time_left <= 0) {
		// advance the head_frame iterator
		if (_advancePlayHead())
			_checkPlayState();
	}
}

void ofxSubtitle::_checkPlayState(void)
{
	assert(playback_state != SUBTITLE_STOPPED);
	assert(play_head != subtitles.end());

	// set the playback state based on the start time of the next subtitle
	int start = base_timestamp + (*play_head)->start_time;

	// is start in the future?
	if (start <= ofGetElapsedTimeMillis()) {
		playback_state = SUBTITLE_DISPLAYING;
	} else {
		playback_state = SUBTITLE_WAITING;
	}
}

void ofxSubtitle::play(void)
{
	// TODO: find out whether this causes an off-by-one problem. 
	//       Need to figure out whether the "update" event listener 
	//       fires before update() in the sketch, or after
	if (playback_state == SUBTITLE_STOPPED) {
		firstFrame();
		playback_state = SUBTITLE_WAITING;
	}
	
	_checkPlayState();
}

void ofxSubtitle::setLoopState(ofLoopType state)
{
	loop_type = state;
}

int ofxSubtitle::size(void)
{
	return subtitles.size();
}


void ofxSubtitle::update(ofEventArgs& args)
{
	// figure out how much time elapsed since the last update
	int last_frame_time = ofGetElapsedTimeMillis() - frame_timestamp;

	// update the frame timestamp
	frame_timestamp = ofGetElapsedTimeMillis();

	switch(playback_state) {
	case SUBTITLE_STOPPED:
		// stick a fork in it, we're done
		return;
	case SUBTITLE_PAUSED:
		// keep the base_timestamp rolling forward so the relative
		// offsets of our subtitles stay in the right place
		base_timestamp += last_frame_time;
		break;
	case SUBTITLE_DISPLAYING:
		_updateDisplayState();
		break;
	case SUBTITLE_WAITING:
		_checkPlayState();
		break;
	default:
		// batshit crazy stuff going on
		ofLog(OF_LOG_ERROR, "Subtitle::update() called with some unknown playback state");
		break;
	}
}

void ofxSubtitle::stop(void)
{
	playback_state = SUBTITLE_PAUSED;
}



