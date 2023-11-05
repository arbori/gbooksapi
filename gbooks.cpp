#include <iostream>
#include <string>
#include <regex>
#include <curl/curl.h>

std::string gbooks_error_message(int code) {
    return std::string(curl_easy_strerror((CURLcode) code));
}

const char* gbooks_query(const std::string& query, const int& startIndex, const int& maxResults) {
    std::string urlstr = "https://www.googleapis.com/books/v1/volumes?q=_q_&maxResults=_maxResults_&startIndex=_startIndex_";

    urlstr = std::regex_replace(urlstr, std::regex("_q_"), query);
    urlstr = std::regex_replace(urlstr, std::regex("_maxResults_"), std::to_string(maxResults));
    urlstr = std::regex_replace(urlstr, std::regex("_startIndex_"), std::to_string(startIndex));
    urlstr = std::regex_replace(urlstr, std::regex(" "), "%20");

    char* url = new char[urlstr.length() + 1];
    strcpy(url, urlstr.c_str());

    return url;
}

static size_t gbooks_write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

std::string gbooks_search(const std::string& query, const int& startIndex, const int& maxResults, std::function<std::string(std::string, int res)> callback) {
    CURL *curl;
    CURLcode res;
    std::string content;

    curl = curl_easy_init();

    if (curl) {
        const char* url = gbooks_query(query, startIndex, maxResults);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, gbooks_write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
 
        res = curl_easy_perform(curl);
 
        curl_easy_cleanup(curl);
    }

    return callback(content, res);
}

// std::string gbooks_callback(std::string content, int code) {
//     if(code == 0) {
//         return content;
//     }
//     else {
//         return gbooks_error_message(code);
//     }
// } 

// int main(void) {
//     const std::string& query = "philosopher's stone harry potter";
//     const int& startIndex = 0;
//     const int& maxResults = 5;

//     std::cout << gbooks_search(query, startIndex, maxResults, gbooks_callback) << std::endl;

//     return 0;
// }
