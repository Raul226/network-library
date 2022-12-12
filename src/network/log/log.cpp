#include "log.hpp"

/**
 * @brief Used to get the vector containing all the errors
 *
 * @return A vector containing all the errors
 */
std::vector<std::string> logs::error::getErrors()
{
    return this->errors;
}

/**
 * @brief Gets the last error that was added to the error vector,
 * @brief The error gets then removed from the vector
 *
 * @return The last error message
 */
std::string logs::error::getLastError()
{
    std::string error = this->errors[this->errors.size() - 1];
    this->errors.pop_back();
    return error;
}

/**
 * @brief Add an error to the vector
 *
 * @param error The error description
 */
void logs::error::addError(std::string error)
{
    this->errors.push_back(error);
}

/**
 * @brief Checks if there are any errors in the vector
 *
 * @return true there are no errors
 * @return false there are errors saved in the vector
 */
bool logs::error::noError()
{
    return !(this->errors.size() > 0);
}