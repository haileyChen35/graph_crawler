#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <curl/curl.h>
#include "rapidjson/document.h"
#include <chrono>

const std::string BASE_URL = "http://hollywood-graph-crawler.bridgesuncc.org/neighbors/";

// Callback function for handling API responses
size_t write_callback(char *contents, size_t size, size_t nmemb, std::string *response) {
    size_t total_size = size * nmemb;
    response->append(contents, total_size);
    return total_size;
}

// Fetch neighbors for a given node from the API
std::vector<std::string> fetch_neighbors(const std::string &node) {
    //libcurl
    CURL *curl;
    CURLcode res;
    std::string response_data;


    // Initialize libcurl
    curl = curl_easy_init();
    std::vector<std::string> neighbors;

    if (curl) {

        // Url encode 
        char *output = curl_easy_escape(curl, node.c_str(), node.length());
        std::string url = BASE_URL + output;
        curl_free(output);

        // Set the API endpoint URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set the write callback function to handle the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        // Perform the GET request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            //Parse Json 
            rapidjson::Document document;
            document.Parse(response_data.c_str());

            if (document.HasMember("neighbors") && document["neighbors"].IsArray()) {
                for (auto &neighbor : document["neighbors"].GetArray()) {
                    if (neighbor.IsString()) {
                        neighbors.push_back(neighbor.GetString());
                    }
                }
            }
        }

        // Clean up libcurl
        curl_easy_cleanup(curl);
    }

    return neighbors;
}

// BFS Traversal
std::queue<std::pair<std::string, int>> q;
std::set<std::string> visited;

void bfs_traverse(const std::string &start_node, int depth) {

    auto start_time = std::chrono::high_resolution_clock::now();


    q.push({start_node, 0});
    visited.insert(start_node);

    std::cout << "BFS Traversal starting from: " << start_node << " up to depth: " << depth << std::endl;

    while (!q.empty()) {
        auto [current_node, current_depth] = q.front();
        q.pop();

        std::cout << "Visiting: " << current_node << " (Depth: " << current_depth << ")" << std::endl;

        if (current_depth < depth) {
            std::vector<std::string> neighbors = fetch_neighbors(current_node);
            for (const std::string &neighbor : neighbors) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    q.push({neighbor, current_depth + 1});
                }
            }
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Execution time: " << duration.count() << " ms" << std::endl;
}


// Main function
int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <start_node> <depth>" << std::endl;
        return 1;
    }
    std::string start_node = argv[1];
    int depth = std::stoi(argv[2]);

    bfs_traverse(start_node, depth);

    return 0;
}

