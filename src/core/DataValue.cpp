#include <alltomd/core/DataValue.h>
#include <sstream>

namespace alltomd {
    
    namespace core {

        bool DataValue::asBool(bool default_value) const {
            if (isBool()) return std::get<bool>(data_);
            return default_value;
        }

        int64_t DataValue::asInt(int64_t default_value) const {
            if (isInt()) return std::get<int64_t>(data_);
            if (isDouble()) return static_cast<int64_t>(std::get<double>(data_));
            return default_value;
        }

        double DataValue::asDouble(double default_value) const {
            if (isDouble()) return std::get<double>(data_);
            if (isInt()) return static_cast<double>(std::get<int64_t>(data_));
            return default_value;
        }

        std::string DataValue::asString(const std::string& default_value) const {
            if (isString()) return std::get<std::string>(data_);
            if (isBool()) return std::get<bool>(data_) ? "true" : "false";
            if (isNull()) return "null";
            if (isInt()) return std::to_string(std::get<int64_t>(data_));
            if (isDouble()) return std::to_string(std::get<double>(data_));
            return default_value;
        }

        DataArray DataValue::asArray() const {
            if (isArray()) return std::get<DataArray>(data_);
            return DataArray{};
        }

        DataObject DataValue::asObject() const {
            if (isObject()) return std::get<DataObject>(data_);
            return DataObject{};
        }

        size_t DataValue::size() const {
            if (isArray()) return std::get<DataArray>(data_).size();
            if (isObject()) return std::get<DataObject>(data_).size();
            return 0;
        }

        DataValue& DataValue::operator[](size_t index) {
            if (!isArray()) {
                data_ = DataArray{};
            }
            auto& arr = std::get<DataArray>(data_);
            if (index >= arr.size()) {
                arr.resize(index + 1);
            }
            return arr[index];
        }

        const DataValue& DataValue::operator[](size_t index) const {
            static DataValue null_value;
            if (isArray() && index < std::get<DataArray>(data_).size()) {
                return std::get<DataArray>(data_)[index];
            }
            return null_value;
        }

        void DataValue::pushBack(const DataValue& value) {
            if (!isArray()) {
                data_ = DataArray{};
            }
            std::get<DataArray>(data_).push_back(value);
        }

        bool DataValue::contains(const std::string& key) const {
            if (isObject()) {
                const auto& obj = std::get<DataObject>(data_);
                return obj.find(key) != obj.end();
            }
            return false;
        }

        DataValue& DataValue::operator[](const std::string& key) {
            if (!isObject()) {
                data_ = DataObject{};
            }
            return std::get<DataObject>(data_)[key];
        }

        const DataValue& DataValue::operator[](const std::string& key) const {
            static DataValue null_value;
            if (isObject()) {
                const auto& obj = std::get<DataObject>(data_);
                auto it = obj.find(key);
                if (it != obj.end()) {
                    return it->second;
                }
            }
            return null_value;
        }

        std::string DataValue::toString() const {
            if (isNull()) return "null";
            if (isBool()) return asBool() ? "true" : "false";
            if (isInt()) return std::to_string(asInt());
            if (isDouble()) return std::to_string(asDouble());
            if (isString()) return "\"" + asString() + "\"";
            if (isArray()) {
                std::stringstream ss;
                ss << "[";
                const auto& arr = asArray();
                for (size_t i = 0; i < arr.size(); ++i) {
                    if (i > 0) ss << ", ";
                    ss << arr[i].toString();
                }
                ss << "]";
                return ss.str();
            }
            if (isObject()) {
                std::stringstream ss;
                ss << "{";
                const auto& obj = asObject();
                size_t i = 0;
                for (const auto& [key, value] : obj) {
                    if (i++ > 0) ss << ", ";
                    ss << "\"" << key << "\": " << value.toString();
                }
                ss << "}";
                return ss.str();
            }
            return "unknown";
        }

    } // namespace core

} // namespace alltomd