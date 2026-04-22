#pragma once

typedef struct {
    int year;
    char* title;
    int hasOscar;
} Movie;

void printMovie(const Movie* movie);
