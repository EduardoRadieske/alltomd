#ifndef ALLTOMD_CONVERTER_CONVERTER_H
#define ALLTOMD_CONVERTER_CONVERTER_H

#include <string>
#include <alltomd/core/DataValue.h>

namespace alltomd {

    namespace converter {

        using DataValue = alltomd::core::DataValue;
        using DataObject = alltomd::core::DataObject;
        using DataArray = alltomd::core::DataArray;

        class Converter {
        public:
            virtual ~Converter() = default;
            
            virtual bool canHandle(const std::string& filename) const = 0;
            
            virtual DataValue parse(const std::string& content) = 0;
            
            virtual std::string name() const = 0;
            
            virtual bool validate(const std::string& content) const = 0;
        };

    } // namespace converter
    
} // namespace alltomd

#endif
