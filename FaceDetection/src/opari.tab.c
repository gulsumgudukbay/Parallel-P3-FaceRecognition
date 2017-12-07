#include "pomp_lib.h"


extern struct ompregdescr omp_rd_45;
extern struct ompregdescr omp_rd_46;
extern struct ompregdescr omp_rd_47;
extern struct ompregdescr omp_rd_48;
extern struct ompregdescr omp_rd_49;
extern struct ompregdescr omp_rd_50;
extern struct ompregdescr omp_rd_51;

int POMP_MAX_ID = 52;

struct ompregdescr* pomp_rd_table[52] = {
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  &omp_rd_45,
  &omp_rd_46,
  &omp_rd_47,
  &omp_rd_48,
  &omp_rd_49,
  &omp_rd_50,
  &omp_rd_51,
};
