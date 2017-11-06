#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <curl/curl.h>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
void fetchJson();
void getCityList();
 
const char *URL_BASE = "https://www.metaweather.com/api/location/search/?query=";
const char *CITY_LIST_FILE = "city_list.txt";
std::vector<std::string> cities;

int main(void){
	getCityList();
	std::cout << "Read " << cities.size() << " cities from file..." << std::endl;
	/*
	for(int i=0; i<cities.size(); ++i){
		std::cout << cities[i] << std::endl;
		std::cout << "Sample: " << URL_BASE + cities[i] << std::endl;	 
	}
	*/
	//fetchJson();
	return 0;
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

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
