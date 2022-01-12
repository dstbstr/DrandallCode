#include "Todo/TodoReader.h"

#include "TestCommon.h"
#include "Utilities/StringUtils.h"

#include <sstream>

namespace TodoLib {
    struct TodoReaderTest : public ::testing::Test {
        std::shared_ptr<std::stringstream> ss = std::make_shared<std::stringstream>();
        TodoSettings Settings{};
        Todo DefaultTodo{};

        std::vector<Todo> Run() {
            TodoSettings settings;
            TodoReader reader{settings, ss};
            return reader.Execute();
        }
    };

    TEST_F(TodoReaderTest, Execute_WithNoInput_ProducesEmptyVector) {
        auto todos = Run();
        ASSERT_EQ(0, todos.size());
    }

    TEST_F(TodoReaderTest, Execute_WithoutTodo_ProducesEmptyVector) {
        *ss << "int i = 0;";
        ASSERT_EQ(0, Run().size());
    }

    TEST_F(TodoReaderTest, Execute_WithBlankTodo_ProducesTodo) {
        *ss << "//TODO";
        auto todos = Run();

        ASSERT_EQ(1, todos.size());
        auto todo = todos[0];

        ASSERT_EQ(todo.Classification, DefaultTodo.Classification);
        ASSERT_EQ(todo.Status, DefaultTodo.Status);
        ASSERT_EQ(1, todo.LineNumber);
    }

    TEST_F(TodoReaderTest, Execute_WithASpace_ProducesTodo) {
        *ss << "// TODO";
        ASSERT_EQ(1, Run().size());
    }

    TEST_F(TodoReaderTest, Execute_WithASpecialSymbol_ProducesTodo) {
        *ss << "//@TODO";
        ASSERT_EQ(1, Run().size());
    }

    TEST_F(TodoReaderTest, Execute_WithColonAtEnd_ProducesTodo) {
        *ss << "//TODO:";
        ASSERT_EQ(1, Run().size());
    }

    TEST_F(TodoReaderTest, Execute_WithSpaceSpecialCharacterAndColon_ProducesTodo) {
        *ss << "// @TODO:";
        ASSERT_EQ(1, Run().size());
    }

    TEST_F(TodoReaderTest, Execute_WithSlashStarTodo_ProducesTodo) {
        *ss << "/*TODO*/";
        ASSERT_EQ(1, Run().size());
    }

    TEST_F(TodoReaderTest, Execute_WithHumanWrittenTodo_PutsTextInNotes) {
        *ss << "//TODO:My Notes";
        auto todo = Run()[0];

        ASSERT_EQ("My Notes", todo.Notes);
    }

    TEST_F(TodoReaderTest, Execute_WithHumanWrittenTodo_TrimsNotes) {
        *ss << "//TODO: My Notes ";
        auto todo = Run()[0];

        ASSERT_EQ("My Notes", todo.Notes);
    }

    TEST_F(TodoReaderTest, Execute_WithTodoAtEnd_ProducesTodo) {
        *ss << "Some random text ending with //TODO";
        ASSERT_EQ(1, Run().size());
    }

    TEST_F(TodoReaderTest, Execute_WithClassification_ExtractsClassification) {
        *ss << "//TODO" + Settings.TodoSeparator + "MyClassification";
        auto todo = Run()[0];

        ASSERT_EQ(todo.Classification, "MyClassification");
    }

    TEST_F(TodoReaderTest, Execute_WithClassificationAndStatus_ExtractsStatus) {
        *ss << StrUtil::Join(Settings.TodoSeparator, "//TODO", "MyClassification", "MyStatus");
        auto todo = Run()[0];

        ASSERT_EQ(todo.Status, "MyStatus");
    }

    TEST_F(TodoReaderTest, Execute_WithClassificationAndStatusAndNotes_ExtractsNotes) {
        *ss << StrUtil::Join(Settings.TodoSeparator, "//TODO", "Classification", "Status", "Some extra notes");
        auto todo = Run()[0];

        ASSERT_EQ(todo.Notes, "Some extra notes");
    }

    TEST_F(TodoReaderTest, Execute_WithTooManySeparators_PutsExtrasIntoNotes) {
        *ss << StrUtil::Join(Settings.TodoSeparator, "//TODO", "c", "s", "notes", "one", "two");
        auto todo = Run()[0];

        ASSERT_EQ(todo.Notes, StrUtil::Join(Settings.TodoSeparator, "notes", "one", "two"));
    }

    TEST_F(TodoReaderTest, Execute_AtEndOfLine_CapturesPrefix) {
        *ss << "int i = 0; //TODO: Use Auto";
        auto todo = Run()[0];

        ASSERT_EQ(todo.RelevantLine, "int i = 0;");
    }

    TEST_F(TodoReaderTest, Execute_AboveALine_CapturesSuffix) {
        *ss << "//TODO: Use Auto\n"
            << "int i = 0;";
        auto todo = Run()[0];

        ASSERT_EQ(todo.RelevantLine, "int i = 0;");
    }

    TEST_F(TodoReaderTest, Execute_AtEndOfLine_TrimsPrefix) {
        *ss << "\t  int i = 0;  //TODO";
        auto todo = Run()[0];

        ASSERT_EQ(todo.RelevantLine, "int i = 0;");
    }

    TEST_F(TodoReaderTest, Execute_AboveALine_TrimsSuffix) {
        *ss << "//TODO\n"
            << "\t\t   int i = 0;   ";
        auto todo = Run()[0];

        ASSERT_EQ(todo.RelevantLine, "int i = 0;");
    }

    TEST_F(TodoReaderTest, Execute_AboveATodo_CapturesSuffixForBoth) {
        *ss << "//TODO: One\n"
            << "//TODO: Two\n"
            << "Line";
        auto todos = Run();

        ASSERT_EQ(todos[0].RelevantLine, "Line");
        ASSERT_EQ(todos[1].RelevantLine, "Line");
    }

    TEST_F(TodoReaderTest, Execute_WithInlineThenAboveLine_CapturesPrefixThenSuffix) {
        *ss << "Prefix //TODO\n"
            << "//TODO\n"
            << "Suffix";
        auto todos = Run();

        ASSERT_EQ(todos[0].RelevantLine, "Prefix");
        ASSERT_EQ(todos[1].RelevantLine, "Suffix");
    }

    TEST_F(TodoReaderTest, Execute_AboveLineThenInline_CapturesPrefixForBoth) {
        *ss << "//TODO\n"
            << "Prefix //TODO";
        auto todos = Run();

        ASSERT_EQ(todos[0].RelevantLine, "Prefix");
        ASSERT_EQ(todos[1].RelevantLine, "Prefix");
    }
} // namespace TodoLib