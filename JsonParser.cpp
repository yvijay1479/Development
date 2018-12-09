// JsonParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include "stack"
#include "string"


using namespace std;

class JsonHelper {
	
	std::string jsonElementStr;
	std::string jsonFinalDoc;
	std::stack<std::string> jStack;

public: 
	JsonHelper();	
	string ParseJsonString(string json_str);
	void RecoverJsonString(string key);
	void ClearJsonFinalDoc();
	string JsonFinalString();
	~JsonHelper();


};

JsonHelper::JsonHelper() {}

JsonHelper::~JsonHelper() {}

void JsonHelper::ClearJsonFinalDoc() {
	jsonFinalDoc = "";
}
string JsonHelper::JsonFinalString() {
	return jsonFinalDoc;
}

string JsonHelper::ParseJsonString(string json_str) {
	if (json_str.empty()) {
		throw "Json String is Empty";
	}

	long strLen = json_str.length();
	long pos = 0;
	try {
		while (pos < strLen) {
			if (json_str[pos] == '}' && jStack.empty()) {
				break;
			}

			if (json_str[pos] == ',' && jStack.empty()) {
				break;
			}

			if (json_str[pos] == '{') {
				jStack.push("{");
				jsonFinalDoc += json_str[pos];
			}

			if (json_str[pos] == '}') {
				if (jStack.top().find("{") == string::npos) {
					RecoverJsonString("}");
					//throw "Invalid Json";
				}

				jStack.pop();
				jsonElementStr = "";
				jsonFinalDoc += json_str[pos];
			}

			if (json_str[pos] == '[') {
				jStack.push("[");
				jsonFinalDoc += json_str[pos];
			}
			if (json_str[pos] == ']') {
				if (jStack.top().find("[") == string::npos) {
					RecoverJsonString("]");
					//throw "Invalid Json";
				}
				jStack.pop();
				jsonElementStr = "";
				jsonFinalDoc += json_str[pos];
			}

			if (json_str[pos] == '"') {
				if (jsonElementStr.empty()) {
					jStack.push("\"");
					jsonFinalDoc += json_str[pos];
				}
				else if (!jsonElementStr.empty()) {
					if (jStack.top().find("\"") == string::npos) {
						RecoverJsonString("\"");
						//throw "Invalid Json";
					}
					else {
						jsonFinalDoc += json_str[pos];
					}
					jStack.pop();					
					jsonElementStr = "";
				}
			}

			if (json_str[pos] == '\'') {
				if (jsonElementStr.empty()) {
					jStack.push("\'");
					jsonFinalDoc += "\"";
				}
				else if (!jsonElementStr.empty()) {
					if (jStack.top().find("\'") == string::npos) {
						RecoverJsonString("\"");
						//throw "Invalid Json";
					}
					jStack.pop();
					jsonFinalDoc += "\"";
					jsonElementStr = "";
				}
			}

			if (json_str[pos] == '.') {
				if (json_str[pos - 1] != '0') {
					jsonElementStr += "0.";
					jsonFinalDoc += "0.";
				}
			}

			if (json_str[pos] == ':') {
				if (json_str[pos + 2] == ',') {
					std::size_t found = jsonFinalDoc.rfind(",");
					if (found != std::string::npos) {
						jsonFinalDoc.erase(found, std::string::npos);
					}
				}
				else if (json_str[pos + 1] == ' ') {
					jsonFinalDoc += json_str[pos];
					jsonFinalDoc += json_str[pos + 1];
				}
			}

			if (json_str[pos] >= '0' && json_str[pos] <= '9') {
				jsonElementStr += json_str[pos];
				jsonFinalDoc += json_str[pos];
			}

			if ((json_str[pos] >= 'A' && json_str[pos] <= 'Z') || (json_str[pos] >= 'a' && json_str[pos] <= 'z')) {
				jsonElementStr += json_str[pos];
				jsonFinalDoc += json_str[pos];
			}
			pos++;
		}
	}
	catch (exception &e) {
		cout << "Inside ParseJson" << e.what() << endl;
	}
	
	return jsonFinalDoc;
}

void JsonHelper::RecoverJsonString(string key) {
	std::size_t found = jsonFinalDoc.rfind(key);
	
	if (found != std::string::npos) {
		jsonFinalDoc.erase(found,1);
	}
	
}

int main()
{
	std::string jsonStr;
	string finalJsonStr;
	JsonHelper oJson;

	try {
		while (std::cin.good() && !std::cin.eof()) {
			std::getline(std::cin, jsonStr);
			finalJsonStr = oJson.ParseJsonString(jsonStr);
			oJson.ClearJsonFinalDoc();
			cout << finalJsonStr << endl;
		}
	}
	catch (exception &e) {
		finalJsonStr = oJson.JsonFinalString();
		cout << "Main Loop" << finalJsonStr << endl;
		cout << e.what() << endl;
	}
	


    return 0;
}

