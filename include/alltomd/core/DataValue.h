#ifndef ALLTOMD_CORE_DATAVALUE_H
#define ALLTOMD_CORE_DATAVALUE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <variant>

namespace alltomd {

    namespace core {

        class DataValue;

        using DataObject = std::unordered_map<std::string, DataValue>;
        using DataArray = std::vector<DataValue>;
        using DataPrimitive = std::variant<
            std::nullptr_t, 
            bool,           
            int64_t,         
            double,          
            std::string     
        >;

        class DataValue {
        public:
            // Constructors
            DataValue() : data_(nullptr) {}
            DataValue(std::nullptr_t) : data_(nullptr) {}
            DataValue(bool value) : data_(value) {}
            DataValue(int value) : data_(static_cast<int64_t>(value)) {}
            DataValue(int64_t value) : data_(value) {}
            DataValue(double value) : data_(value) {}
            DataValue(const std::string& value) : data_(value) {}
            DataValue(const char* value) : data_(std::string(value)) {}
            DataValue(const DataObject& value) : data_(value) {}
            DataValue(const DataArray& value) : data_(value) {}
            
            // Type checking
            bool isNull() const { return std::holds_alternative<std::nullptr_t>(data_); }
            bool isBool() const { return std::holds_alternative<bool>(data_); }
            bool isInt() const { return std::holds_alternative<int64_t>(data_); }
            bool isDouble() const { return std::holds_alternative<double>(data_); }
            bool isString() const { return std::holds_alternative<std::string>(data_); }
            bool isArray() const { return std::holds_alternative<DataArray>(data_); }
            bool isObject() const { return std::holds_alternative<DataObject>(data_); }
            
            // Getters (with default values)
            bool asBool(bool default_value = false) const;
            int64_t asInt(int64_t default_value = 0) const;
            double asDouble(double default_value = 0.0) const;
            std::string asString(const std::string& default_value = "") const;
            DataArray asArray() const;
            DataObject asObject() const;
            
            // Array operations
            size_t size() const;
            DataValue& operator[](size_t index);
            const DataValue& operator[](size_t index) const;
            void pushBack(const DataValue& value);
            
            // Object operations
            bool contains(const std::string& key) const;
            DataValue& operator[](const std::string& key);
            const DataValue& operator[](const std::string& key) const;
            
            // Iterators for objects
            auto begin() { return std::get<DataObject>(data_).begin(); }
            auto end() { return std::get<DataObject>(data_).end(); }
            auto begin() const { return std::get<DataObject>(data_).begin(); }
            auto end() const { return std::get<DataObject>(data_).end(); }
            
            // Conversion to string (for debugging)
            std::string toString() const;
            
        private:
            std::variant<std::nullptr_t, bool, int64_t, double, std::string, DataArray, DataObject> data_;
        };

    } // namespace core

} // namespace alltomd

#endif