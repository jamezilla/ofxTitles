/*
 *  ofxSubtitle.h
 *
 *  Created by James Hughes on 2012/04/04.
 *  Copyright 2012 James Hughes. All rights reserved.
 *
 */
#pragma once

#include <stdint.h>
#include <set>
#include "ofMain.h"

class ofxSubtitle
{
public:
	ofxSubtitle();

	void          add(std::string _text, int _number, int _start_time, int _end_time);
	void          clear(void);
	void          draw(void);
	void          draw(float x, float y);
	void          draw(float x, float y, float w, float h);
	bool          empty(void);
	void          firstFrame(void);
	bool          isPlaying(void);
	void		  loadFont(const std::string& path){font.loadFont(path,13,true,true,true);};
	//void          nextFrame(void);
	void          play(void);
	void          setLoopState(ofLoopType state);
	int size(void);
	void          stop(void);
	void          update(ofEventArgs& args);

private:
	enum PlaybackState {
		SUBTITLE_STOPPED,
		SUBTITLE_PAUSED,
		SUBTITLE_WAITING,
		SUBTITLE_DISPLAYING
	};

	struct Subtitle {
		bool operator<(const Subtitle& other) const { return start_time < other.start_time; }
		std::string text;
		int         number;
		int         start_time;
		int         end_time;
		int         duration;
	};

	typedef ofPtr<Subtitle> SubtitlePtr;
	typedef std::set<SubtitlePtr> Subtitles;
	
	void                _checkPlayState(void);
	bool                _advancePlayHead(void);
	void                _updateDisplayState(void);

	PlaybackState       playback_state;
	int                 base_timestamp;
	int                 frame_timestamp;
	ofLoopType          loop_type;
	Subtitles           subtitles;
	Subtitles::iterator play_head;
	ofTrueTypeFont	    font;
};