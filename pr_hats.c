#define PI_QROOT 1.331325547571923
#define SAMPLING_RATE 10.0
#define NUM_HATS 170
#define PTS_PER_HAT 21
#define HAT_PTS NUM_HATS * PTS_PER_HAT
#define PTS_PER_FILTER 5

__device__ __host__ float mexican_hat(float sigma, float t) {
  float sigma_sq = sigma * sigma;
  float t_sq = t * t;
  float term1 = 2.0 / (PI_QROOT * sqrt(3.0 * sigma));
  float term2 = 1.0 - (t_sq / sigma_sq);
  float term3 = expf(-1.0 * (t_sq/(2.0 * sigma_sq)));
  return term1 * term2 * term3;
}

__device__ __host__ float RR_hat(float rr, float t) {
  // Generates a point at time +t+ of the mexican hat wavelet that will
  // correlate best with a QRS complex for a particular heartrate +RR+
  // Assumptions: 7% of RR is the QRS complex, RR is in BPM
  float sigma = SAMPLING_RATE * (0.07*0.125*60.0)/rr;
  return mexican_hat(sigma, t);
}

__global__ void calculate_hats(float * hats) {
  // Convert the thread ID (which is the discrete T value) into the t value for
  // the hat function. For our purposes, we care about the hat between the
  // values -1 and 1, since it holds the value of 0 (with float precision)
  // outside that range for heartrates of interest
  float t = -1.0 + (((float) threadIdx.x)/SAMPLING_RATE);
  int discreteT = threadIdx.x;
  int which_hat = blockIdx.x;
  int idx = which_hat * blockDim.x + discreteT;
  // Since heartrates under 30 are a good sign of a dead (or near dead) patient
  // we will ignore those in our analysis, and start at heartrate of 30
  int rr = which_hat + 30;
  hats[idx] = RR_hat((float) rr, t);
}

// TODO test median_filtering

__device__ __host__ float
median(float * in_signal) {
  float sorted[PTS_PER_FILTER];
  int i, j;
  float x;
  // Populate the sorted array
  for (int i = 0; i < PTS_PER_FILTER; i++) {
    sorted[i] = in_signal[i];
  }
  // Sort using insertion sort (algorithm from Wikipedia
  // https://en.wikipedia.org/wiki/Insertion_sort)
  for (i = 1; i < PTS_PER_FILTER; i++) {    
     x = sorted[i];
     j = i - 1;
     while (j >= 0 && sorted[j] > x) {       
         sorted[j+1] = sorted[j];
         j = j - 1;
     }
     sorted[j+1] = x;
  }
  // Integer shifts and divisions seem to run at the same speed...
  return sorted[PTS_PER_FILTER >> 1];
}

__device__ __host__ void
median_filter(
  float * out_signal,
  float * in_signal,
  int start)
{
  out_signal[start/PTS_PER_FILTER] = median(&in_signal[start]);
}

__global__ void
median_filter_signal(
  float * in_signal,
  float * out_signal)
{
  int idx = threadIdx.x + blockIdx.x * blockDim.x;
  median_filter(out_signal,
                in_signal,
                idx * PTS_PER_FILTER);
}

__device__ __host__ void
cross_correlate_point_with_hat(
  float * result_point,
  float * signal,
  float * hat_signal,
  int point,
  int signal_size)
{
  float result = 0.0;
  for (int i = -PTS_PER_HAT / 2; i < PTS_PER_HAT / 2; i++) {
    if (point + i >= signal_size) {
      break;
    } else if (point + i < 0) {
      continue;
    }
    result += signal[point + i] * hat_signal[i + PTS_PER_HAT / 2];
  }
  * result_point = result;
}

__global__ void
cross_correlate_with_hat(
  float * out_signal,
  float * in_signal,
  float * hat_signal,
  int signal_size)
{
  int idx = threadIdx.x + blockIdx.x * blockDim.x;
  cross_correlate_point_with_hat(&out_signal[idx], in_signal, hat_signal, idx, signal_size);
}
