#ifndef __RESULTGENERATOR_H__
#define __RESULTGENERATOR_H__

#include <ostream>
#include <vector>

struct FileData;

class ResultGenerator {
public:
    enum SortOrder { FILE_NAME, DEPENDENCIES, BLAST_RADIUS };

    ResultGenerator(const std::vector<FileData>& results) : m_Results(results) {}
    void PrintResultToStream(std::ostream& targetStream, SortOrder sortBy = SortOrder::DEPENDENCIES, bool descending = false) const;

private:
    const std::vector<FileData>& m_Results;
};

#endif // __RESULTGENERATOR_H__