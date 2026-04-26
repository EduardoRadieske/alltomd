#ifndef ALLTOMD_CORE_DATAARRAY_H
#define ALLTOMD_CORE_DATAARRAY_H

#include <alltomd/core/DataValue.h>

namespace alltomd {

    namespace core {

        using DataArray = std::vector<DataValue>;
        using DataObject = std::unordered_map<std::string, DataValue>;

    } // namespace core

} // namespace alltomd

#endif