#pragma once
#include <mutex>
#include <vector>
#include <string>

enum ConsoleEntryType
{
    SuccesEntry,
    LogEntry,
    WarningEntry,
    ErrorEntry
};

struct ConsoleLine
{
    std::string text;
    ConsoleEntryType entry_type;
};

class Console
{
private:
    static Console *console_instance;
    static std::mutex singleton_mutex;

    std::vector<ConsoleLine> lines;
    std::mutex text_mutex;

    // Append line to console
    Console *appendLine(const std::string &line, ConsoleEntryType type);

protected:
    Console();
    ~Console();

public:
    Console(Console &other) = delete;
    void operator=(const Console &) = delete;

    static Console *GetInstance();

    static void DestroyInstance();

    Console *addSuccesEntry(const std::string &text);
    Console *addLogEntry(const std::string &text);
    Console *addWarningEntry(const std::string &text);
    Console *addErrorEntry(const std::string &text);
    Console *addEmptyLine();

    // Retrieve the console's contents
    std::vector<ConsoleLine> getContents();
};