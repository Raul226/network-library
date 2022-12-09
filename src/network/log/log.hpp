#pragma once

#include <vector>
#include <string>

namespace logs
{
    class error
    {
    public:
        std::vector<std::string> getErrors();
        std::string getLastError();
        void addError(std::string error);
        bool noError();

    private:
        std::vector<std::string> errors;
    };
}