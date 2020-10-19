/*
** EPITECH PROJECT, 2020
** B-CPP-500-MAR-5-1-babel-thibault.schmitt
** File description:
** Audio
*/

#include "../include/Audio.hpp"

Audio::Audio()
{
    this->_isListening = false;
    this->_isRecording = false;
    this->_isStreaming = false;
    this->_stream = nullptr;
    _allFrames = 5 * 48000;
    _Data.max = _allFrames;
    _Data.idx = 0;
    _Data.rec = new float[_allFrames * sizeof(float)];
    if (_Data.rec == NULL) {
        std::exit(84);
    }
    for (int i = 0; i < _allFrames; i++)
        _Data.rec[i] = 0;
    init();
}

Audio::~Audio()
{
    this->stop();
}

void Audio::init()
{
    Pa_Initialize();
    init_inParam();
    init_outParam();
}
void Audio::stop()
{
    Pa_CloseStream(_stream);
    Pa_Terminate();
}

int Audio::recCallBa(const void* input, void* output, unsigned long frameCount, const PaStreamCallbackTimeInfo* time, PaStreamCallbackFlags status, void *usrData)
{
    paData* data = (paData*)usrData;
    const float* inputFloat = (const float*)input;
    float* outputFloat = &data->rec[data->idx];

    if (data->max - data->idx < 480)
        return paComplete;
    for (int i = 0; i < 480; i++) {
        if (input == NULL)
            *outputFloat++ = 0.0f;
        else
            *outputFloat++ = *inputFloat++;
    }
    data->idx += 480;
    return paContinue;
}

int Audio::lisCallBa(const void* input, void* output, unsigned long frameCount, const PaStreamCallbackTimeInfo* time, PaStreamCallbackFlags status, void *usrData)
{
    paData* data = (paData*)usrData;
    float* inputFloat = &data->rec[data->idx];
    float* outputFloat = (float*)output;

    if (data->max - data->idx < 480) {
        int i = 0;
        for (; i < data->max - data->idx; i++)
            *outputFloat++ = *inputFloat++;
        for (; i < 480; i++)
            *outputFloat++ = 0;
        data->idx += data->max - data->idx;
        return paComplete;
    }
    for (int i = 0; i < 480; i++)
        *outputFloat++ = *inputFloat++;
    data->idx += 480;
    return paContinue;
}

void Audio::startStreamInput()
{
    Pa_OpenStream(&_stream, &_inParam, NULL, 48000, 480, paClipOff, recCallBa, &(this->_Data));
    Pa_StartStream(_stream);
    _isStreaming = true;
}

void Audio::startStreamOutput(paData &data)
{
    Pa_OpenStream(&_stream, NULL, &_outParam, 48000, 480, paClipOff, lisCallBa, &(data));
    Pa_StartStream(_stream);
    _isStreaming = true;
}

void Audio::stopStream()
{
    Pa_CloseStream(_stream);
    _isStreaming = false;
}

void Audio::isStreaming(long time)
{
    while ((Pa_IsStreamActive(this->_stream)) == 1)
        Pa_Sleep(time);
}

paData &Audio::getData()
{
    return this->_Data;
}

void Audio::init_inParam()
{
    _inParam.device = Pa_GetDefaultInputDevice();
    _inParam.channelCount = 1;
    _inParam.sampleFormat = paFloat32;
    _inParam.suggestedLatency = Pa_GetDeviceInfo(_inParam.device)->defaultLowInputLatency;
    _inParam.hostApiSpecificStreamInfo = NULL;
}

void Audio::init_outParam()
{
    _outParam.device = Pa_GetDefaultOutputDevice();
    _outParam.channelCount = 1;
    _outParam.sampleFormat = paFloat32;
    _outParam.suggestedLatency = Pa_GetDeviceInfo(_outParam.device)->defaultLowOutputLatency;
    _outParam.hostApiSpecificStreamInfo = NULL;
}

int Audio::getAllFrames()
{
    return _allFrames;
}