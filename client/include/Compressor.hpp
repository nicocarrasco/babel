/*
** EPITECH PROJECT, 2020
** B-CPP-500-MAR-5-1-babel-thibault.schmitt
** File description:
** Compressor
*/

#ifndef COMPRESSOR_HPP_
#define COMPRESSOR_HPP_

#include "ISound.hpp"

class Compressor : public ISound
{
    public:
        Compressor();
        ~Compressor();
        void init();
        void stop();
        int encodeData(const float *buf, int size, unsigned char *buffer);
        int decodeData(unsigned char *buffer, int size, float *data);

    private:
        OpusEncoder *enc;
        OpusDecoder *dec;
};

#endif /* !COMPRESSOR_HPP_ */
