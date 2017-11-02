#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <curl/curl.h>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
void fetchJson();
void getWoeidList();
    
const char *URL_BASE = "https://www.metaweather.com/api/location/44418/";
const std::string WOEID_FILE = "id_list.txt";
std::vector<std::string> woeids;

int main(void){
	getWoeidList();
	for(int i=0; i<woeids.size(); ++i)
		std::cout << woeids[i] << ' ';	
	//fetchJson();
	return 0;
}

void getWoeidList(){
	std::ifstream idFile(WOEID_FILE.c_str());
	std::string currentLine;

	if(idFile.is_open()){
		if(! idFile.eof()){
			getline (idFile, currentLine);
			woeids.push_back(currentLine);	
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
