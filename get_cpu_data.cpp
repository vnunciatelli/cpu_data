#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <map>
#include <json/json.h>

#ifdef _WIN32
#include <Windows.h>
#elif __linux__
#include <cstdlib>
#elif __APPLE__
#include <sys/sysctl.h>
#endif

std::string get_os() {
    #ifdef _WIN32
    return "windows";
    #elif __linux__
    return "linux";
    #elif __APPLE__
    return "darwin";
    #else
    return "unknown";
    #endif
}

std::string get_cpu_info() {
    if (get_os() == "windows") {
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        return std::to_string(si.dwProcessorType);
    }
    else if (get_os() == "linux") {
        std::string result = "";
        FILE* fp = popen("cat /proc/cpuinfo | grep 'model name' | uniq", "r");
        if (fp != nullptr) {
            char buffer[256];
            while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
                result += buffer;
            }
            fclose(fp);
        }
        return result;
    }
    else if (get_os() == "darwin") {
        std::string result = "";
        FILE* fp = popen("sysctl -n machdep.cpu.brand_string", "r");
        if (fp != nullptr) {
            char buffer[256];
            while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
                result += buffer;
            }
            fclose(fp);
        }
        return result;
    }
    return "N/A";
}

std::string get_cpu_temperature() {
    if (get_os() == "linux") {
        std::string result = "";
        FILE* fp = popen("sensors | grep 'Package id 0' | awk '{print $4}'", "r");
        if (fp != nullptr) {
            char buffer[256];
            while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
                result += buffer;
            }
            fclose(fp);
        }
        return result;
    }
    else if (get_os() == "darwin") {
        std::string result = "";
        FILE* fp = popen("osx-cpu-temp", "r");
        if (fp != nullptr) {
            char buffer[256];
            while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
                result += buffer;
            }
            fclose(fp);
        }
        return result;
    }
    return "N/A";
}

std::map<std::string, std::string> get_memory_info() {
    std::map<std::string, std::string> memInfo;
    if (get_os() == "linux") {
        FILE* fp = popen("free -m | grep Mem", "r");
        if (fp != nullptr) {
            char buffer[256];
            while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
                int total, used, free;
                sscanf(buffer, "%*s %d %d %d", &total, &used, &free);
                memInfo["total"] = std::to_string(total) + " MB";
                memInfo["used"] = std::to_string(used) + " MB";
                memInfo["free"] = std::to_string(free) + " MB";
            }
            fclose(fp);
        }
    }
    return memInfo;
}

std::map<std::string, std::string> get_disk_info() {
    std::map<std::string, std::string> diskInfo;
    if (get_os() == "linux") {
        FILE* fp = popen("df -h / | awk 'NR==2 {print $2, $3, $4}'", "r");
        if (fp != nullptr) {
            char buffer[256];
            while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
                std::string total, used, free;
                sscanf(buffer, "%s %s %s", &total[0], &used[0], &free[0]);
                diskInfo["total"] = total;
                diskInfo["used"] = used;
                diskInfo["free"] = free;
            }
            fclose(fp);
        }
    }
    return diskInfo;
}

int main() {
    std::map<std::string, std::string> data;

    data["os"] = get_os();
    data["cpu"] = get_cpu_info();
    data["temperature"] = get_cpu_temperature();
    
    std::map<std::string, std::string> memoryInfo = get_memory_info();
    for (auto& mem : memoryInfo) {
        data[mem.first] = mem.second;
    }

    std::map<std::string, std::string> diskInfo = get_disk_info();
    for (auto& disk : diskInfo) {
        data[disk.first] = disk.second;
    }

    // Salvar no arquivo JSON
    Json::Value root;
    for (const auto& item : data) {
        root[item.first] = item.second;
    }

    std::ofstream file("system_info.json");
    file << root;
    file.close();

    std::cout << "Dados salvos em system_info.json" << std::endl;

    return 0;
}