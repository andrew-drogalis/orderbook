// Andrew Drogalis Copyright (c) 2024, GNU 3.0 Licence
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#ifndef DRO_BIT_CAST_HPP
#define DRO_BIT_CAST_HPP

#include <cstdint>

namespace dro
{

uint64_t convertToSymbol(const void* buf)
{
  return __builtin_bswap64(*static_cast<const uint64_t*>(buf));
}

uint8_t convertToInt8(const void* bits)
{
  return *static_cast<const uint8_t*>(bits);
}

uint32_t convertToInt16(const void* bits)
{
  return *static_cast<const uint16_t*>(bits);
}

uint32_t convertToInt32(const void* bits)
{
  return *static_cast<const uint32_t*>(bits);
}

uint64_t convertToInt64(const void* bits)
{
  return *static_cast<const uint64_t*>(bits);
}

}// namespace dro
#endif
