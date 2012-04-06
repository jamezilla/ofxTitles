/*
 *  ofxTitles.h
 *
 *  Created by James Hughes on 2012/04/04.
 *  Copyright 2012 James Hughes. All rights reserved.
 *
 */
#pragma once

#include <stdint.h>
#include <set>
#include "ofMain.h"

class ofxTitles
{
public:
	ofxTitles();

	void          add(std::string _text, int _number, int _start_time, int _end_time);
	void          clear(void);
	void          draw(void);
	void          draw(float x, float y);
	void          draw(float x, float y, float w, float h, float percent=0.9);
	bool          empty(void);
	void          firstFrame(void);
	bool          isPlaying(void);
	void		  loadFont(std::string path, int fontsize=14, bool _bAntiAliased=true, bool _bFullCharacterSet=false, bool makeContours=false, float simplifyAmt=0.3, int dpi=0);
	//void          nextFrame(void);
	void          play(void);
	void          setLoopState(ofLoopType state);
	int size(void);
	void          stop(void);
	void          update(ofEventArgs& args);

private:
	enum PlaybackState {
		TITLE_STOPPED,
		TITLE_PAUSED,
		TITLE_WAITING,
		TITLE_DISPLAYING
	};

	struct Title {
		//bool operator<(const Title& other) const { return start_time < other.start_time; }
		std::string text;
		int         number;
		int         start_time;
		int         end_time;
		int         duration;
	};

	typedef ofPtr<Title> TitlePtr;

	struct classcomp {
		bool operator() (const TitlePtr& lhs, const TitlePtr& rhs) const
		{return lhs->start_time < rhs->start_time;}
	};

	typedef std::set<TitlePtr, classcomp> Titles;
	
	void                _checkPlayState(void);
	bool                _advancePlayHead(void);
	void                _updateDisplayState(void);

	PlaybackState       playback_state;
	int                 base_timestamp;
	int                 frame_timestamp;
	ofLoopType          loop_type;
	Titles              titles;
	Titles::iterator    play_head;
	ofTrueTypeFont	    font;
};
