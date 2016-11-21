#include <Eigen/Dense>
#include <complex>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <vector>

#include "bloch_system.h"
#include "configuration.h"
#include "interaction_table.h"
#include "lagrange_set.h"
#include "math_utils.h"
#include "quantum_dot.h"

using namespace std;

int main(int argc, char *argv[])
{
  try {
    auto vm = parse_configs(argc, argv);

    vector<QuantumDot> qds(2);
    qds[0] = QuantumDot(Eigen::Vector3d(), 0, std::pair<double, double>(10, 20),
                        Eigen::Vector3d(0, 0, 1));

    qds[1] =
        QuantumDot(Eigen::Vector3d(0.005, 0.005, 0.005), 0,
                   std::pair<double, double>(10, 20), Eigen::Vector3d(0, 0, 1));

    Weights rpc(18, 22, 3.15, 1e-12);

    BlochSystem sys(rpc, qds, config.interpolation_order, 2048);

    for(int i = -22; i < 2048; ++i) {
      const double g = skew_gaussian(5, ((i - 1024) / 256.));

      sys.history[0][i][0][1] = g;
      sys.history[1][i][0][1] = 0;
    }

    for(int i = 0; i < 2048; ++i) {
      sys.convolve_currents(sys.now++);

      cout << setprecision(12);
      cout << i*config.dt << " " << sys.rabi_freqs[0] << " " << sys.rabi_freqs[1] << endl;
    }

  } catch(CommandLineException &e) {
    // User most likely queried for help or version info, so we can silently
    // move on
  }

  return 0;
}
