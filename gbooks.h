#include <string>
#include <functional>

#ifndef __GBOOKS_H__
#define __GBOOKS_H__

std::string gbooks_error_message(int code);

std::string gbooks_search(const std::string& query, const int& startIndex, const int& maxResults, std::function<std::string(std::string, int code)> callback);

#endif // __GBOOKS_H__
