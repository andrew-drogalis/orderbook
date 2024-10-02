// Andrew Drogalis Copyright (c) 2024, GNU 3.0 Licence
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#include "include/dro/data-feed.hpp"

#include <algorithm>                             // for sort
#include <boost/iostreams/device/mapped_file.hpp>// for mapped_file
#include <chrono>                                // for duration, duration...
#include <cstdint>                               // for uint16_t, uint32_t
#include <iostream>                              // for basic_ostream, ope...
#include <numeric>                               // for accumulate
#include <string>                                // for char_traits, basic...
#include <vector>                                // for vector

int main(int argc, char* argv[])
{
  std::string fileName = "sample-data/ITCH50Data";
  if (argc == 2)
  {
    fileName = argv[1];
  }
  boost::iostreams::mapped_file file(fileName);

  dro::DataFeed feed(1'000, 10);
  feed.Subscribe("AAPL");

  char buffer[64];
  size_t iter {};
  size_t messageCount {};
  std::vector<uint32_t> samples;

  while (iter < file.size())
  {
    // Endian swap
    int length = __builtin_bswap16(
        *reinterpret_cast<const uint16_t*>(file.data() + iter));
    memcpy(buffer, file.data() + iter + 2, length);

    // Benchmark
    auto start = std::chrono::high_resolution_clock::now();
    feed.Parse(buffer);
    auto stop = std::chrono::high_resolution_clock::now();

    samples.emplace_back(
        std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start)
            .count());

    messageCount++;
    iter += length + 2;
    if (! (messageCount % 1'000'000))
    {
      std::cout << "~";
      std::cout.flush();
    }
  }
  std::sort(samples.begin(), samples.end());
  auto mean =
      std::accumulate(samples.begin(), samples.end(), 0.0) / samples.size();
  auto min = samples.empty() ? 0 : samples.front();
  auto max = samples.empty() ? 0 : samples.back();
  auto p99 = samples.empty() ? 0 : samples[0.99 * samples.size()];

  std::cout << "\nMessage Count: " << messageCount << "\n";
  std::cout << "Symbols Count: " << feed.symbolCount() << "\n";

  std::cout << "Min:    " << min << std::endl;
  std::cout << "Mean:   " << mean << std::endl;
  std::cout << "99%:    " << p99 << std::endl;
  std::cout << "Max:    " << max << std::endl;

  return 0;
}
