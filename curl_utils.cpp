#include <curl/curl.h>
#include <iostream>
#include "curl_utils.h"

using namespace std;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

string get_response_from_ai_us(string url, string token, string question)
{
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    string data = "{\"question\": \" " + question + "\", \"token\": \"" + token.c_str() +"\"}";
    string readbuffer;

    curl = curl_easy_init();
    if (curl) {
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readbuffer);
        res = curl_easy_perform(curl);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        if (res != CURLE_OK) {
           // std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            return "Failed to send POST request. Error code: ";
        }
    }

    return readbuffer;
}
