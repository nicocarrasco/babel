/*
** EPITECH PROJECT, 2020
** B-CPP-500-MAR-5-1-babel-thibault.schmitt
** File description:
** Audio
*/

#include "ISound.hpp"

typedef struct
{
    int idx;
    int max;
    float* rec;
} paData;

class Audio : public ISound
{
    public:
        Audio();
        ~Audio();
        void startStreamInput();
        void startStreamOutput(paData &data);
        static int recCallBa(const void* input, void* output, unsigned long frameCount, const PaStreamCallbackTimeInfo* time, PaStreamCallbackFlags status, void *usrData);
        static int lisCallBa(const void* input, void* output, unsigned long frameCount, const PaStreamCallbackTimeInfo* time, PaStreamCallbackFlags status, void *usrData);
        void init();
        void stop();
        void stopStream();
        paData &getData();
        void isStreaming(long time);
        int getAllFrames();

    private:
        PaStream* _stream;
        paData _Data;
        PaStreamParameters _inParam;
        PaStreamParameters _outParam;
        bool _isRecording;
        bool _isStreaming;
        bool _isListening;
        void init_inParam();
        void init_outParam();
        int _allFrames;
};