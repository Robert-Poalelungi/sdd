#pragma once

typedef struct {
    char* country;
    char group;
    int matchesPlayed;
} FootballTeam;

void printTeam(const FootballTeam* team);
