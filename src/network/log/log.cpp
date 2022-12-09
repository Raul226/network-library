#include "log.hpp"

std::vector<std::string> logs::error::getErrors()
{
    return this->errors;
}

std::string logs::error::getLastError()
{
    std::string error = this->errors[this->errors.size() - 1];
    this->errors.pop_back();
    return error;
}

void logs::error::addError(std::string error)
{
    this->errors.push_back(error);
}

bool logs::error::noError()
{
    return !(this->errors.size() > 0);
}