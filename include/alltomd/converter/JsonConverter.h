#ifndef ALLTOMD_CONVERTER_JSONCONVERTER_H
#define ALLTOMD_CONVERTER_JSONCONVERTER_H

#include <alltomd/converter/Converter.h>
#include <nlohmann/json.hpp>

namespace alltomd {

    namespace converter {

        class JsonConverter : public Converter {
        public:
            bool canHandle(const std::string& filename) const override;
            
            DataValue parse(const std::string& content) override;
            
            std::string name() const override { return "JSON"; }
            
            bool validate(const std::string& content) const override;
            
            std::string format(const DataValue& data, int indent = 4) const;

        private:
            // Conversão interna de nlohmann::json para DataValue
            DataValue convertFromJson(const nlohmann::json& json_data) const;

        };

    } // namespace converter

} // namespace alltomd

#endif