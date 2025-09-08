#ifndef CLOCK_H
#define CLOCK_H

class Clock {
  public:
    void Init();
    void RTCset();
    String GetTimeAsString();
    tm GetTime();
    void SetTime(tm t);
};

#endif