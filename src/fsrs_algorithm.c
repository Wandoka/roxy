#include "fsrs_algorithm.h"

#include <math.h>
#include <time.h>

#define SMIN 0.001
#define IMAX 36500

static const double W[21] = {
  0.212,   // [0]  initial S after Again (FSRS_FORGET)
  1.2931,  // [1]  initial S after Hard  (FSRS_RECALL_HARD)
  2.3065,  // [2]  initial S after Good  (FSRS_RECALL_GOOD)
  8.2956,  // [3]  initial S after Easy  (FSRS_RECALL_EASY)
  6.4133,  // [4]  initial D after Again
  0.8334,  // [5]  how much first grade lowers D
  3.0194,  // [6]  how fast D changes after a review
  0.001,   // [7]  pull D back toward Easy
  1.8722,  // [8]  S growth scale on success
  0.1666,  // [9]  large S grows slower
  0.796,   // [10] more growth if recalled late
  1.4835,  // [11] S scale after Again
  0.0614,  // [12] hard cards drop more after Again
  0.2629,  // [13] how old S carries over after Again
  1.6483,  // [14] late Again is penalized less 
  0.6014,  // [15] Hard grows S less than Good
  1.8729,  // [16] Easy grows S more than Good
  0.5425,  // [17] same-day grade strength
  0.0912,  // [18] same-day grade offset
  0.0658,  // [19] same-day change shrinks as S grows
  0.1542   // [20] forgetting-curve shape
};


static double clampf(double x, double low, double high) {
  if(x < low) return low;
  if(x > high) return high;
  return x;
}

static double decay(void)  { return -W[20]; } 
static double factor(void) { return pow(0.9, 1.0 / decay()) - 1; }


static double d0(int g, int clamp) {
  double d = W[4] - exp(W[5] * (g - 1)) + 1;
  return clamp ? clampf(d, 1, 10) : d;
}

static double next_d(double d, int g) {
  double delta = -W[6] * (g - 3);
  double d1 = d + (10 - d) * delta / 9;
  return clampf(W[7] * d0(FSRS_RECALL_EASY, 0) + (1 - W[7]) * d1, 1, 10);
}

static double next_s_recall(double d, double s, double r, int g) {
  double h = g == FSRS_RECALL_HARD ? W[15] : 1;
  double b = g == FSRS_RECALL_EASY ? W[16] : 1;
  return s * (1 + exp(W[8]) * (11 - d) * pow(s, -W[9]) * (exp((1 - r) * W[10]) - 1) * h * b);
}

static double next_s_forget(double d, double s, double r) {
  double lng = W[11] * pow(d, -W[12]) * (pow(s + 1, W[13]) - 1) * exp((1 - r) * W[14]);
  double sh  = s / exp(W[17] * W[18]);
  return lng < sh ? lng : sh;
}

static double next_s_short(double s, int g) {
  double inc = exp(W[17] * (g - 3 + W[18])) * pow(s, -W[19]);
  if (g >= FSRS_RECALL_HARD && inc < 1) inc = 1;
  return s * inc;
}

static double days_since_last_review(Card *c) {
  time_t unixtime = time(NULL);
  double days = (unixtime - c->last_FSRS_review_unix_time) / (60.0 * 60.0 * 24);
  return days;
}

static double fsrs_calc_R_recall_value_custom_time(double t, double s) {
  return pow(1.0 + factor() * t / s, decay());
}
//-------------------------------------------------------------------------------------------------

double fsrs_calc_R_recall_value(Card *c) {
  double t = days_since_last_review(c);
  double s = c->FSRS_Stability;
  return fsrs_calc_R_recall_value_custom_time(t, s);
}


void fsrs_review(Card *c, int g) {
  double elapsed_days = days_since_last_review(c);
  if (!c->has_FSRS_data) {
    c->FSRS_Stability = W[g - 1];
    c->FSRS_Difficulty = d0(g, 1);
    c->has_FSRS_data = 1;
  } else if (elapsed_days < 1) {
    c->FSRS_Stability = fmax(next_s_short(c->FSRS_Stability, g), SMIN);
    c->FSRS_Difficulty = next_d(c->FSRS_Difficulty, g);
  } else {
    double r = fsrs_calc_R_recall_value_custom_time(elapsed_days, c->FSRS_Stability);
    if(g == FSRS_FORGET) {
      c->FSRS_Stability = fmax(next_s_forget(c->FSRS_Difficulty, c->FSRS_Stability, r), SMIN);
    }
    else {
      c->FSRS_Stability = fmax(next_s_recall(c->FSRS_Difficulty, c->FSRS_Stability, r, g), SMIN);
    }
    c->FSRS_Difficulty = next_d(c->FSRS_Difficulty, g);  
  }
}
