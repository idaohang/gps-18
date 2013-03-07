#ifndef GPSVOICE_HPP
#define GPSVOICE_HPP

#include "RoadReading.hpp"
#include <QtMultimedia/qsound.h>

enum LanguageCountry {
    EN,
    CN,
    FR
};

class GPSVoice
{
public:
    GPSVoice();
    ~GPSVoice();
    void    setLanguage(LanguageCountry);
    void    updateVoice(DirectionNext);
private:
    LanguageCountry _language;
};

#endif // GPSVOICE_HPP
