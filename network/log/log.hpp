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
        bool noError();

    protected:
        void addError(std::string error);

    private:
        std::vector<std::string> errors;
    };
}