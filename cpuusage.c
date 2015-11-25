/*
 * cpuusage.c - Simple CPU usage printer
 * Copyright (C) 2015 Emiliano Firmino <emiliano.firmino@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct cpuinfo {
    int user;
    int nice;
    int system;
    int idle;
};

int findCpuCount() {
    size_t nread = 256 * sizeof(char);
    char * line = (char*) malloc(nread + 1);

    int cpuCount = 0;

    FILE * f = fopen("/proc/stat", "r");
    while (1) {
        getline(&line, &nread, f);
        line[3] = '\0';
        if (strcmp(line, "cpu") == 0) {
            cpuCount += 1;
        } else {
            break;
        }
    }
    fclose(f);
    free(line);

    return cpuCount;
}

int main() {
    int count = 0;
    size_t nread = 256 * sizeof(char);
    char * line = (char*) malloc(nread + 1);

    int cpuCount = findCpuCount();
    struct cpuinfo * lastSample = (struct cpuinfo*) malloc(cpuCount * sizeof(struct cpuinfo));
    struct cpuinfo * currSample = (struct cpuinfo*) malloc(cpuCount * sizeof(struct cpuinfo));

    while (1) {
        FILE * f = fopen("/proc/stat", "r");
        if (f == NULL) {
            fprintf(stderr, "failed to open /proc/stat.");
            return -1;
        }

        for (int i = 0; i < cpuCount; i++) {
            getline(&line, &nread, f);
            sscanf(line, "%*s %d %d %d %d",
                    &currSample[i].user,
                    &currSample[i].nice,
                    &currSample[i].system,
                    &currSample[i].idle);
        }
        fclose(f);

        if (count % 16 == 0) {
            printf("\ncpu");
            for (int i = 0; i < cpuCount - 1; i++) {
                printf("\tcpu%d", i);
            }
            printf("\n");
        }

        if (count > 0) {
            for (int i = 0; i < cpuCount; i++) {
                int total0 = lastSample[i].user;
                total0 += lastSample[i].nice;
                total0 += lastSample[i].system;
                total0 += lastSample[i].idle;

                int total1 = currSample[i].user;
                total1 += currSample[i].nice;
                total1 += currSample[i].system;
                total1 += currSample[i].idle;

                int idle = currSample[i].idle - lastSample[i].idle;

                int deltaTotal = total1 - total0;
                if (deltaTotal > 0) {
                    float loadTime = 1.0f - (idle / (deltaTotal * 1.0f));
                    loadTime *= 100.0f;
                    printf("%.3f\t", loadTime);
                }
            }
            printf("\n");
        }

        count += 1;
        memcpy(lastSample, currSample, cpuCount * sizeof(struct cpuinfo));
        sleep(1);
    }

    free(line);

    return 0;
}

