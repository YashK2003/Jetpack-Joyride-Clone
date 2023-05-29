#include "avatar.h"

float gravity(float height)
{
    if (height >= -0.85 && height < -0.75)
        return 0.0015f;
    if (height >= -0.75 && height < -0.65)
        return 0.0015f;
    if (height >= -0.65 && height < -0.55)
        return 0.0025f;
    if (height >= -0.55 && height < -0.45)
        return 0.0025f;
    if (height >= -0.45 && height < -0.35)
        return 0.0030f;
    if (height >= -0.35 && height < -0.25)
        return 0.0030f;
    if (height >= -0.25 && height < -0.15)
        return 0.0030f;
    if (height >= -0.15 && height < -0.05)
        return 0.0040f;
    if (height >= -0.05 && height < 0.05)
        return 0.0040f;
    if (height >= 0.25 && height < 0.45)
        return 0.0050f;
    if (height >= 0.45 && height < 0.65)
        return 0.0052f;
    if (height >= 0.65 && height < 0.85)
        return 0.0052f;
    if (height >= 0.85 && height < 1.05)
        return 0.0056f;
    if (height >= 1.05 && height < 1.25)
        return 0.0056f;
    if (height >= 1.25 && height < 1.45)
        return 0.0060f;
    if (height >= 1.45)
        return 0.0060f;

    return 0.0030f;
}
