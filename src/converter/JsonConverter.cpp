#include <alltomd/converter/JsonConverter.h>

namespace alltomd {

    namespace converter {

        bool JsonConverter::canHandle(const std::string& filename) const {
            // Check extension
            if (filename.length() >= 5) {
                std::string ext = filename.substr(filename.length() - 5);
                std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
                if (ext == ".json") {
                    return true;
                }
            }
            
            // Also check for .json (4 chars)
            if (filename.length() >= 4) {
                std::string ext = filename.substr(filename.length() - 4);
                std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
                if (ext == "json") {
                    return true;
                }
            }
            
            return false;
        }

        bool JsonConverter::validate(const std::string& content) const {
            try {
                nlohmann::json::parse(content);
                return true;
            } catch (const nlohmann::json::parse_error&) {
                return false;
            }
        }

        DataValue JsonConverter::convertFromJson(const nlohmann::json& json_data) const {
            if (json_data.is_null()) {
                return DataValue(nullptr);
            }
            else if (json_data.is_boolean()) {
                return DataValue(json_data.get<bool>());
            }
            else if (json_data.is_number_integer()) {
                return DataValue(json_data.get<int64_t>());
            }
            else if (json_data.is_number_float()) {
                return DataValue(json_data.get<double>());
            }
            else if (json_data.is_string()) {
                return DataValue(json_data.get<std::string>());
            }
            else if (json_data.is_array()) {
                DataArray arr;
                for (const auto& item : json_data) {
                    arr.push_back(convertFromJson(item));
                }
                return DataValue(arr);
            }
            else if (json_data.is_object()) {
                DataObject obj;
                for (auto it = json_data.begin(); it != json_data.end(); ++it) {
                    obj[it.key()] = convertFromJson(it.value());
                }
                return DataValue(obj);
            }
            
            return DataValue(nullptr);
        }

        DataValue JsonConverter::parse(const std::string& content) {
            auto json_data = nlohmann::json::parse(content);
            return convertFromJson(json_data);
        }

        // Debug
        std::string JsonConverter::format(const DataValue& data, int indent) const {

            if (indent <= 0) {
                return data.toString();  // Sem indentação
            }

            auto json_data = nlohmann::json::parse(data.toString());
            return json_data.dump(indent);
        }

    } // namespace converter

} // namespace alltomd