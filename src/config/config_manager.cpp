#include "config_manager.h"

#include <fstream>
#include <stdexcept>

#include "config_file_path.h"

ConfigManager::ConfigManager() {
    m_config_file_path = CONFIG_FILE_PATH;
}

std::string trim(const std::string &str) {
    const char *whitespace = " \t\n\r\f\v";

    size_t start = str.find_first_not_of(whitespace);
    if (start == std::string::npos) {
        return "";
    }
    size_t end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}

void ConfigManager::parse() {
    std::fstream file;

    file.open(m_config_file_path);
    if (!file.is_open()) {
        throw std::runtime_error("failed to open file: " + m_config_file_path.string());
    }

    std::string line;
    std::string section;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        if (line.front() == '[' && line.back() == ']') {
            // Section header detected
            section = trim(line.substr(1, line.size() - 2));
        } else {
            auto equal_pos = line.find("=");
            if (equal_pos == std::string::npos) {
                continue;
            }
            std::string key   = trim(line.substr(0, equal_pos));
            std::string value = trim(line.substr(equal_pos + 1));

            m_result[section][key] = value;
        }
    }

    file.close();
}

std::string ConfigManager::getValue(const std::string &section, const std::string &key) const {
    auto sect = m_result.find(section);
    if (sect == m_result.end()) {
        return "";
    }

    auto kv = sect->second.find(key);
    if (kv == sect->second.end()) {
        return "";
    } else {
        return kv->second;
    }
}