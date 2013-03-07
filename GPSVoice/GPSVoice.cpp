#include "GPSVoice.hpp"

GPSVoice::GPSVoice()
{
    this->_language = EN;
}

GPSVoice::~GPSVoice()
{
}

void    GPSVoice::setLanguage(LanguageCountry language)
{
    this->_language = language;
}

void    GPSVoice::updateVoice(DirectionNext direction)
{
    if (_language == EN )
    {
        if ( direction == STRAIGHT_150)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_EN_Straight150M.wav");
        if ( direction == STRAIGHT_100)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_EN_Straight100M.wav");
        if ( direction == STRAIGHT_50)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_EN_Straight50M.wav");
        if ( direction == STRAIGHT)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_EN_Straight.wav");
        if ( direction == TURN_LEFT_150)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_EN_TurnLeft150M.wav");
        if ( direction == TURN_LEFT_100)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_EN_TurnLeft100M.wav");
        if ( direction == TURN_LEFT_50)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_EN_TurnLeft50M.wav");
        if ( direction == TURN_LEFT)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_EN_TurnLeft.wav");
        if ( direction == TURN_RIGHT_150)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_EN_TurnRight150M.wav");
        if ( direction == TURN_RIGHT_100)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_EN_TurnRight100M.wav");
        if ( direction == TURN_RIGHT_50)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_EN_TurnRight50M.wav");
        if ( direction == TURN_RIGHT)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_EN_TurnRight.wav");
        if ( direction == TURN_BACK)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_EN_TurnBack.wav");
        if ( direction == ARRIVE)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_EN_Arrive.wav");
    }
    if (_language == CN )
    {
        if ( direction == STRAIGHT_150)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_CN_Straight150M.wav");
        if ( direction == STRAIGHT_100)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_CN_Straight100M.wav");
        if ( direction == STRAIGHT_50)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_CN_Straight50M.wav");
        if ( direction == STRAIGHT)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_CN_Straight.wav");
        if ( direction == TURN_LEFT_150)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_CN_TurnLeft150M.wav");
        if ( direction == TURN_LEFT_100)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_CN_TurnLeft100M.wav");
        if ( direction == TURN_LEFT_50)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_CN_TurnLeft50M.wav");
        if ( direction == TURN_LEFT)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_CN_TurnLeft.wav");
        if ( direction == TURN_RIGHT_150)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_CN_TurnRight150M.wav");
        if ( direction == TURN_RIGHT_100)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_CN_TurnRight100M.wav");
        if ( direction == TURN_RIGHT_50)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_CN_TurnRight50M.wav");
        if ( direction == TURN_RIGHT)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_CN_TurnRight.wav");
        if ( direction == TURN_BACK)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_CN_TurnBack.wav");
        if ( direction == ARRIVE)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_CN_Arrive.wav");
    }
    if (_language == FR )
    {
        if ( direction == STRAIGHT_150)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_FR_Straight150M.wav");
        if ( direction == STRAIGHT_100)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_FR_Straight100M.wav");
        if ( direction == STRAIGHT_50)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_FR_Straight50M.wav");
        if ( direction == STRAIGHT)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_FR_Straight.wav");
        if ( direction == TURN_LEFT_150)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_FR_TurnLeft150M.wav");
        if ( direction == TURN_LEFT_100)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_FR_TurnLeft100M.wav");
        if ( direction == TURN_LEFT_50)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_FR_TurnLeft50M.wav");
        if ( direction == TURN_LEFT)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_FR_TurnLeft.wav");
        if ( direction == TURN_RIGHT_150)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_FR_TurnRight150M.wav");
        if ( direction == TURN_RIGHT_100)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_FR_TurnRight100M.wav");
        if ( direction == TURN_RIGHT_50)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_FR_TurnRight50M.wav");
        if ( direction == TURN_RIGHT)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_FR_TurnRight.wav");
        if ( direction == TURN_BACK)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_FR_TurnBack.wav");
        if ( direction == ARRIVE)
            QSound::play("../GPSVoice/GPSVoice/GPSVoice_FR_Arrive.wav");
    }
}
