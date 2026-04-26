#include <alltomd/writer/MarkdownWriter.h>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace alltomd {

    namespace writer {

        std::string MarkdownWriter::convert(const core::DataValue& data, const WriterOptions& options) {
            std::string result;
            
            // Header
            result += generateHeader(options, "");
            result += "\n";
            
            // Summary
            result += generateSummary(data);
            result += "\n";
            
            // Content based on format
            if (options.format == "table") {
                result += convertToTable(data, options);
            } else if (options.format == "list") {
                result += convertToList(data, options);
            } else if (options.format == "code") {
                result += convertToCodeBlock(data, options);
            } else { // auto
                result += convertAuto(data, options);
            }
            
            // Footer
            result += "\n";
            result += generateFooter();
            
            return result;
        }

        std::string MarkdownWriter::convertToTable(const core::DataValue& data, const WriterOptions& options) {
            // Verifica se é um array de objetos (ideal para tabela)
            if (!data.isArray()) {
                // Se não for array, tenta converter para lista
                return convertToList(data, options);
            }
            
            auto arr = data.asArray();
            if (arr.empty()) {
                return "Empty array\n";
            }
            
            // Verifica se o primeiro elemento é um objeto
            if (!arr[0].isObject()) {
                return convertToList(data, options);
            }
            
            // Coleta todas as chaves (headers)
            auto firstObj = arr[0].asObject();
            std::vector<std::string> headers;
            for (const auto& [key, _] : firstObj) {
                headers.push_back(key);
            }
            
            // Aplica preview se necessário
            size_t totalRows = arr.size();
            bool preview = needsPreview(totalRows, options.previewLines);
            size_t maxRows = preview ? options.previewLines : totalRows;
            
            // Constrói a tabela
            std::string table;
            
            // Header row
            table += "| ";
            for (const auto& header : headers) {
                table += header + " | ";
            }
            table += "\n";
            
            // Separator row
            table += "|";
            for (size_t i = 0; i < headers.size(); ++i) {
                table += "---|";
            }
            table += "\n";
            
            // Data rows
            for (size_t i = 0; i < maxRows && i < arr.size(); ++i) {
                if (arr[i].isObject()) {
                    auto obj = arr[i].asObject();
                    table += "| ";
                    for (const auto& header : headers) {
                        if (obj.contains(header)) {
                            table += formatValue(obj[header], options.showNull) + " | ";
                        } else {
                            table += "*missing* | ";
                        }
                    }
                    table += "\n";
                }
            }
            
            // Preview notice
            if (preview && totalRows > options.previewLines) {
                table += "\n*Showing first " + std::to_string(options.previewLines) + 
                        " of " + std::to_string(totalRows) + " rows*\n";
            }
            
            return table;
        }

        std::string MarkdownWriter::convertToList(const core::DataValue& data, const WriterOptions& options) {
            std::string result;
            
            if (data.isArray()) {
                auto arr = data.asArray();
                size_t totalItems = arr.size();
                bool preview = needsPreview(totalItems, options.previewLines);
                size_t maxItems = preview ? options.previewLines : totalItems;
                
                for (size_t i = 0; i < maxItems && i < arr.size(); ++i) {
                    result += "- " + formatValue(arr[i], options.showNull) + "\n";
                }
                
                if (preview && totalItems > options.previewLines) {
                    result += "\n*Showing first " + std::to_string(options.previewLines) + 
                            " of " + std::to_string(totalItems) + " items*\n";
                }
            } else if (data.isObject()) {
                auto obj = data.asObject();
                for (const auto& [key, value] : obj) {
                    result += "- **" + key + "**: " + formatValue(value, options.showNull) + "\n";
                }
            } else {
                result += "- " + formatValue(data, options.showNull) + "\n";
            }
            
            return result;
        }

        std::string MarkdownWriter::convertToCodeBlock(const core::DataValue& data, const WriterOptions& options) {
            std::string result = "```json\n";
            result += data.toString();
            
            if (options.previewLines > 0) {
                // Limita número de linhas
                std::stringstream ss(result);
                std::string line;
                result = "```json\n";
                int lines = 0;
                while (std::getline(ss, line) && lines < options.previewLines) {
                    result += line + "\n";
                    lines++;
                }
                if (lines >= options.previewLines) {
                    result += "... (truncated)\n";
                }
            }
            
            result += "```\n";
            return result;
        }

        std::string MarkdownWriter::convertAuto(const core::DataValue& data, const WriterOptions& options) {
            // Auto-detecta o melhor formato
            if (data.isArray()) {
                auto arr = data.asArray();
                if (!arr.empty() && arr[0].isObject()) {
                    // Array de objetos → tabela
                    return convertToTable(data, options);
                } else {
                    // Array simples → lista
                    return convertToList(data, options);
                }
            } else if (data.isObject()) {
                // Objeto → lista de propriedades
                return convertToList(data, options);
            } else {
                // Valor primitivo → código
                return convertToCodeBlock(data, options);
            }
        }

        std::string MarkdownWriter::formatValue(const core::DataValue& value, bool showNull) {
            if (value.isNull()) {
                return showNull ? "*null*" : "";
            }
            if (value.isBool()) {
                return value.asBool() ? "true" : "false";
            }
            if (value.isInt()) {
                return std::to_string(value.asInt());
            }
            if (value.isDouble()) {
                return std::to_string(value.asDouble());
            }
            if (value.isString()) {
                return value.asString();
            }
            if (value.isArray()) {
                return "[Array of " + std::to_string(value.size()) + " items]";
            }
            if (value.isObject()) {
                return "{Object with " + std::to_string(value.size()) + " keys}";
            }
            return "?";
        }

        std::string MarkdownWriter::generateHeader(const WriterOptions& options, const std::string& filename) {
            std::string header;
            
            if (!options.title.empty()) {
                header += "# " + options.title + "\n";
            } else if (!filename.empty()) {
                header += "# " + filename + "\n";
            } else {
                header += "# Data Report\n";
            }
            
            header += "\n*Generated by alltomd v1.0*";
            
            if (!options.author.empty()) {
                header += " by " + options.author;
            }
            
            header += "\n";
            
            return header;
        }

        std::string MarkdownWriter::generateFooter() {
            auto now = std::chrono::system_clock::now();
            auto now_time_t = std::chrono::system_clock::to_time_t(now);
            
            std::stringstream ss;
            ss << "---\n";
            ss << "*Conversion time: " << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d %H:%M:%S") << "*\n";
            
            return ss.str();
        }

        std::string MarkdownWriter::generateSummary(const core::DataValue& data) {
            std::string summary = "## Summary\n";
            
            if (data.isArray()) {
                summary += "- Records: " + std::to_string(data.size()) + "\n";
                
                if (!data.asArray().empty() && data.asArray()[0].isObject()) {
                    auto firstObj = data.asArray()[0].asObject();
                    std::string fields;
                    for (const auto& [key, _] : firstObj) {
                        if (!fields.empty()) fields += ", ";
                        fields += key;
                    }
                    summary += "- Fields: " + fields + "\n";
                }
            } else if (data.isObject()) {
                summary += "- Keys: " + std::to_string(data.size()) + "\n";
            } else {
                summary += "- Type: " + std::string(data.isString() ? "string" : 
                                                data.isBool() ? "boolean" :
                                                data.isInt() ? "integer" :
                                                data.isDouble() ? "float" : "primitive") + "\n";
            }
            
            return summary;
        }

        bool MarkdownWriter::needsPreview(size_t size, int previewLines) const {
            return previewLines > 0 && size > static_cast<size_t>(previewLines);
        }

        core::DataArray MarkdownWriter::getPreview(const core::DataArray& arr, int previewLines) {
            core::DataArray preview;
            size_t limit = std::min(static_cast<size_t>(previewLines), arr.size());
            for (size_t i = 0; i < limit; ++i) {
                preview.push_back(arr[i]);
            }
            return preview;
        }

        std::string MarkdownWriter::convertObjectRecursive(const core::DataValue& data, int currentDepth, int maxDepth) {
            if (maxDepth > 0 && currentDepth >= maxDepth) {
                return "... (max depth reached)";
            }
            
            // Implementação para objetos aninhados (opcional)
            std::string result;
            if (data.isObject()) {
                result += "\n";
                auto obj = data.asObject();
                for (const auto& [key, value] : obj) {
                    std::string indent(currentDepth * 2, ' ');
                    result += indent + "- **" + key + "**: ";
                    
                    if (value.isObject() || value.isArray()) {
                        result += "\n";
                        result += convertObjectRecursive(value, currentDepth + 1, maxDepth);
                    } else {
                        result += formatValue(value, true) + "\n";
                    }
                }
            }
            return result;
        }

    } // namespace writer
    
} // namespace alltomd