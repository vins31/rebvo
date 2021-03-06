#include "v4lcam.h"
#include "util.h"

v4lCam::v4lCam(const char *dev_name,Size2D frame_size,int f_per_sec,const char *log_name)
    :VideoCam(log_name,frame_size)
{
    if(CamaraInit(dev_name, this,frame_size,f_per_sec)){
        printf("\nv4lCam: No puedo iniciar la camara %s\n",dev_name);
        error=true;
        return;
    }
    error=false;
    init=false;
}

v4lCam::~v4lCam()
{
    if(error)
        return;
    CamaraClose(this);
}

int v4lCam::WaitFrame(bool drop_frames){

    if(error)
        return -1;
    return CamaraWaitFrame(this);
}

int v4lCam::GrabFrame(RGB24Pixel *data, double &tstamp,bool drop_frames)
{

    timeval tv;
    if(error)
        return -1;
    int r=CamaraGrabFrame(this,data,&tv);

    if(init){
        tstamp=util::dift(tv,T0);
    }else{
        T0=tv;
        tstamp=0;
        init=true;
    }

    return r;
}

RGB24Pixel* v4lCam::GrabBuffer(double &tstamp, bool drop_frames)
{

    timeval tv;
    if(error)
        return NULL;

    RGB24Pixel* data=CamaraGrabBuffer(this,&tv);

    if(init){
        tstamp=util::dift(tv,T0);
    }else{
        T0=tv;
        tstamp=0;
        init=true;
    }
    return data;
}

int v4lCam::ReleaseBuffer()
{

    if(error)
        return -1;
    return CamaraReleaseBuffer(this);
}
