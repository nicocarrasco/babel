/*
** EPITECH PROJECT, 2020
** B-CPP-500-MAR-5-1-babel-thibault.schmitt
** File description:
** Compressor
*/

#include "../include/Compressor.hpp"

Compressor::Compressor()
{
    init();
}

Compressor::~Compressor()
{
    stop();
}

void Compressor::init()
{
    int err;

    enc = opus_encoder_create(48000, 1, OPUS_APPLICATION_VOIP, &err);
    dec = opus_decoder_create(48000, 1, &err);
}

void Compressor::stop()
{
    opus_encoder_destroy(enc);
    opus_decoder_destroy(dec);
}
int Compressor::encodeData(const float *buf, int size, unsigned char *buffer)
{
    int len;
    len = opus_encode_float(enc, buf, 480, buffer, size);
    return (len);
}

int Compressor::decodeData(unsigned char *buffer, int size, float *data)
{
    int len;
    len = opus_decode_float(dec, buffer, size, data, 480, 0);
    return (len);
}