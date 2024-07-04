#include "Console.hpp"

Console *Console::console_instance{nullptr};
std::mutex Console::singleton_mutex;

Console::Console() {}

Console::~Console() {}

Console *Console::GetInstance()
{
    std::lock_guard<std::mutex> lock(singleton_mutex);
    if (console_instance == nullptr)
    {
        console_instance = new Console();
    }
    return console_instance;
}

void Console::DestroyInstance()
{
    delete console_instance;
    console_instance = NULL;
}

Console *Console::appendLine(const std::string &text, ConsoleEntryType type)
{
    std::string final_text = ">  " + text;

    ConsoleLine line;
    line.text = final_text;
    line.entry_type = type;

    std::lock_guard<std::mutex> lock(text_mutex); // Acquire lock
    lines.push_back(line);                        // Append line to the array
    return this;
}

Console *Console::addSuccesEntry(const std::string &text)
{
    appendLine(text, SuccesEntry);
    return this;
}

Console *Console::addLogEntry(const std::string &text)
{
    appendLine(text, LogEntry);
    return this;
}

Console *Console::addWarningEntry(const std::string &text)
{
    appendLine(text, WarningEntry);
    return this;
}

Console *Console::addErrorEntry(const std::string &text)
{
    appendLine(text, ErrorEntry);
    return this;
}

Console *Console::addEmptyLine()
{
    ConsoleLine line;
    line.text = "";
    line.entry_type = LogEntry;

    std::lock_guard<std::mutex> lock(text_mutex); // Acquire lock
    lines.push_back(line);                        // Append line to the array
    return this;
}

std::vector<ConsoleLine> Console::getContents()
{
    std::lock_guard<std::mutex> lock(text_mutex); // Acquire lock
    return lines;                                 // Return a copy of the array
}