#include <stdio.h>
#include "FootballTeam.h"

void printTeam(const FootballTeam* team) {
    printf("Group %c | %s - Matches played: %d\n", team->group, team->country, team->matchesPlayed);
}
