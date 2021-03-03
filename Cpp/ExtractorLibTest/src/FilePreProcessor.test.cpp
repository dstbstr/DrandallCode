#include "Extractor/FilePreProcessor.h"

#include "TestCommon.h"

#include <sstream>

class FilePreProcessorTest : public ::testing::Test {
protected:
    std::shared_ptr<std::stringstream> ss = std::make_shared<std::stringstream>();
    std::string m_FileName{"FileName.h"};

    template<typename... Args>
    Extractor::FilePreProcessor CreateProcessor(Args&&... knownDefines) {
        std::vector<std::string> defines(knownDefines...);
        return Extractor::FilePreProcessor(m_FileName, defines, ss);
    }
};

TEST_F(FilePreProcessorTest, ExecuteIgnoresHeaderFileDefine) {
    *ss << R"(#ifndef __FILENAME_H__
        #define __FILENAME_H__
        #endif
    )";

    auto result = CreateProcessor().Execute();
    ASSERT_TRUE(result.Defines.empty());
}