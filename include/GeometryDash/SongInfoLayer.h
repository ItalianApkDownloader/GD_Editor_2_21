#pragma once

#include "FLAlertLayer.h"

class SongInfoLayer : public FLAlertLayer
{
public:


    void onMenuMusicFix(CCObject*) 
    {
        extern int eSongID;
    	int songID = eSongID;
        int& menuMusicSongID = MBO(int, GM, 0x32C);
        
        if(songID == menuMusicSongID)
            menuMusicSongID = 0;
        else
            menuMusicSongID = songID;
        GM->fadeInMenuMusic();

    }
    void onPracticeMusicFix(CCObject*)
    {
        extern int eSongID;
    	int songID = eSongID;
        int& practiceMusicSongID = MBO(int, GM, 0x330);
        
        if(songID == practiceMusicSongID)
            practiceMusicSongID = 0;
        else
            practiceMusicSongID = songID;
    }
};