/*
** EPITECH PROJECT, 2020
** B-CPP-500-MAR-5-1-babel-thibault.schmitt
** File description:
** ISound
*/

#ifndef ISOUND_HPP_
#define ISOUND_HPP_

#include <opus.h>
#include <portaudio.h>
#include <iostream>
#include <vector>

class ISound
{
    public:
        virtual ~ISound() {}
        virtual void init() = 0;
        virtual void stop() = 0;
};

#endif /* !ISOUND_HPP_ */
