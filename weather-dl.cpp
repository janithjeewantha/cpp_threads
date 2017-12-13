#include <iostream>
#include <ctime>
#include <typeinfo>
#include <thread>
#include <mutex>
#include <vector>
#include <fstream>
#include <string>
#include <curl/curl.h>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
void fetchJson(int);
void getCityList();
void fetch_weather_async();
 
const char *URL_BASE = "https://www.metaweather.com/api/location/search/?query=";
const char *CITY_LIST_FILE = "city_list.txt";
std::vector<std::string> cities;
int current_city = -1;
std::mutex city_counter_lock;
std::time_t start;
std::time_t finish;

int main(void){
	getCityList();
	std::cout << "Read " << cities.size() << " cities from file..." << std::endl;
	/*
	for(int i=0; i<cities.size(); ++i){
		std::cout << cities[i] << std::endl;
		std::cout << "Sample: " << URL_BASE + cities[i] << std::endl;	 
	}
	*/
	//fetchJson(1);

    std::cout << "Using 2 threads to fetch..." << std::endl;
	std::thread thread_1(&fetch_weather_async);
	std::thread thread_2(&fetch_weather_async);
	
	start = std::time(nullptr);
	thread_1.join();
	thread_2.join();
	finish = std::time(nullptr);
	
	std::cout << "Duration " << (finish - start) << "s" << std::endl;
	
	return 0;
}

void fetch_weather_async(){
	while (current_city + 1 < (int) cities.size()){
		city_counter_lock.lock();
		int city_index = 0;
		if (current_city + 1 < (int) cities.size()) {
			city_index = ++current_city;
		}
		city_counter_lock.unlock();
	
		std::cout << "requesting " << city_index << std::endl;	
		fetchJson(city_index);
		std::cout << "acquired " << city_index << std::endl;	
	}
}

void getCityList(){
	std::ifstream idFile(CITY_LIST_FILE);
	std::string currentLine;

	if(idFile.is_open()){
		while(! idFile.eof()){
			getline (idFile, currentLine);
			cities.push_back(currentLine);	
		}
	}
	idFile.close();
}

void fetchJson(int i){    
	CURL *curl;
	CURLcode res;
	std::string readBuffer;

	curl = curl_easy_init();
	if(curl) {
    	curl_easy_setopt(curl, CURLOPT_URL, (URL_BASE + cities[i]).c_str());
    	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    	res = curl_easy_perform(curl);
    	curl_easy_cleanup(curl);

    	std::cout << i << " : " << readBuffer << std::endl;
	}
}
/*
void fetchJson(){    
	CURL *curl;
	CURLcode res;
	std::string readBuffer;

	curl = curl_easy_init();
	if(curl) {
    	curl_easy_setopt(curl, CURLOPT_URL, URL_BASE);
    	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    	res = curl_easy_perform(curl);
    	curl_easy_cleanup(curl);

    	std::cout << readBuffer << std::endl;
	}
}
*/
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
