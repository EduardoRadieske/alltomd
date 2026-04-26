#ifndef ALLTOMD_WRITER_MARKDOWNWRITER_H
#define ALLTOMD_WRITER_MARKDOWNWRITER_H

#include <string>
#include <alltomd/core/DataValue.h>

namespace alltomd {

    namespace writer {

        struct WriterOptions {
            std::string format = "auto";     // auto, table, list, code
            int maxDepth = -1;               // -1 = ilimitado
            bool showNull = true;
            int previewLines = -1;           // -1 = todas as linhas
            std::string title = "";
            std::string author = "";
        };

        class MarkdownWriter {
        public:
            // Converte DataValue para Markdown
            std::string convert(const core::DataValue& data, const WriterOptions& options);
            
        private:
            // Métodos de formatação
            std::string convertToTable(const core::DataValue& data, const WriterOptions& options);
            std::string convertToList(const core::DataValue& data, const WriterOptions& options);
            std::string convertToCodeBlock(const core::DataValue& data, const WriterOptions& options);
            std::string convertAuto(const core::DataValue& data, const WriterOptions& options);
            
            // Métodos auxiliares
            std::string formatValue(const core::DataValue& value, bool showNull);
            std::string generateHeader(const WriterOptions& options, const std::string& filename);
            std::string generateFooter();
            std::string generateSummary(const core::DataValue& data);
            
            // Para arrays grandes
            bool needsPreview(size_t size, int previewLines) const;
            core::DataArray getPreview(const core::DataArray& arr, int previewLines);
            
            // Recursão para objetos aninhados
            std::string convertObjectRecursive(const core::DataValue& data, int currentDepth, int maxDepth);
        };

    } // namespace writer
} // namespace alltomd

#endif